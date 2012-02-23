/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "config.h"
#include "PageCachePolicy.h"

#include "ApplicationCacheHost.h"
#include "BackForwardController.h"
#include "DeviceMotionController.h"
#include "DeviceOrientationController.h"
#include "Document.h"
#include "DocumentLoader.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "FrameLoaderClient.h"
#include "Page.h"
#include "Logging.h"
#include "Settings.h"
#include "SharedWorkerRepository.h"

using namespace std;

namespace WebCore {

PageCachePolicy::FactoryFunction* PageCachePolicy::s_factory = PageCachePolicy::DefaultFactoryFunction;

PageCachePolicy::PageCachePolicy(Page* page) 
    : m_backForwardIsActive(page->backForward()->isActive())
    , m_settingsAllowPageCache(page->settings()->usesPageCache())
#if ENABLE(DEVICE_ORIENTATION)
    , m_deviceMotionIsActive(DeviceMotionController::isActiveAt(page))
    , m_deviceOrientationIsActive(DeviceMotionController::isActiveAt(page))
#endif
    , m_loadType(page->mainFrame()->loader()->loadType())
    , m_page(page)
    , m_logIndentLevel(0)
{
}

PageCachePolicy::~PageCachePolicy()
{
}

PassOwnPtr<PageCachePolicy> PageCachePolicy::DefaultFactoryFunction(Page* page)
{
    return adoptPtr(new PageCachePolicy(page));
}

PageCachePolicy::FactoryFunction* PageCachePolicy::GetFactory()
{
    return s_factory;
}

void PageCachePolicy::SetFactory(FactoryFunction* newfactory)
{
    s_factory = newfactory;
}

bool PageCachePolicy::CanCachePage()
{
    PolicyLog("--------\n Determining if page can be cached:");

    const bool canCacheMainFrame = CanCacheFrame(m_page->mainFrame());
    const bool canCachePage = CanCachePageImpl();

    if (canCacheMainFrame && canCachePage)
        PolicyLog(" Page CAN be cached\n--------");
    else
        PolicyLog(" Page CANNOT be cached\n--------");
    return canCacheMainFrame && canCachePage;
}

void PageCachePolicy::PolicyLog(const String& message)
{
    LOG(PageCache, "%*s%s", m_logIndentLevel*4, "", message.utf8().data());
}

bool PageCachePolicy::CanCacheFrame(Frame* frame)
{
    ++m_logIndentLevel;

    for (Frame* child = frame->tree()->firstChild(); child; child = child->tree()->nextSibling()) {
        KURL currentURL = frame->loader()->documentLoader() ? frame->loader()->documentLoader()->url() : KURL();
        KURL newURL = frame->loader()->provisionalDocumentLoader() ? frame->loader()->provisionalDocumentLoader()->url() : KURL();
        if (newURL.isEmpty())
            PolicyLog(makeString(" Determining if subframe with URL(", currentURL.string(), ") can be cached:"));
        else
            PolicyLog(makeString(" Determining if frame can be cached navigating from (", currentURL.string(), ") to (", newURL.string(), "):"));
                      
        if (!CanCacheFrame(child)) {
            PolicyLog(" Frame CANNOT be cached");
            --m_logIndentLevel;
            return false;
        }
        PolicyLog(" Frame CAN be cached");
    }

    --m_logIndentLevel;
    if (!frame->loader()->documentLoader()) {
        PolicyLog(" -There is no DocumentLoader object");
        return false;
    }
    FrameData framedata;

    framedata.isErrorPage = frame->loader()->documentLoader()->substituteData().isValid()
                            && !frame->loader()->documentLoader()->substituteData().failingURL().isEmpty();
    framedata.pageCacheSupportsPlugins = frame->page()->settings()->pageCacheSupportsPlugins();
    framedata.hasDocumentLoader = frame->loader()->documentLoader();
    if (framedata.hasDocumentLoader) {
        framedata.mainDocumentError = frame->loader()->documentLoader()->mainDocumentError().isNull();
        framedata.isLoadingInAPISense = frame->loader()->documentLoader()->isLoadingInAPISense();
        framedata.loaderIsStopping = frame->loader()->documentLoader()->isStopping();
        framedata.appCacheDenied = !frame->loader()->documentLoader()->applicationCacheHost()->canCacheInPageCache();
    }
    framedata.containsPlugins = frame->loader()->subframeLoader()->containsPlugins();
    framedata.protocol = frame->document()->url().protocol();
    framedata.responseDenies = frame->loader()->documentLoader()->response().cacheControlContainsNoCache()
                               || frame->loader()->documentLoader()->response().cacheControlContainsNoStore();
    framedata.hasUnloadListener = frame->domWindow() && frame->domWindow()->hasEventListeners(eventNames().unloadEvent);
#if ENABLE(SQL_DATABASE)
    framedata.hasOpenDatabase = frame->document()->hasOpenDatabases();
#endif
#if ENABLE(SHARED_WORKERS)
    framedata.hasSharedWorkers = SharedWorkerRepository::hasSharedWorkers(frame->document());
#endif
    framedata.usesGeolocation = frame->document()->usingGeolocation();
    framedata.hasHistoryCurrentItem = frame->loader()->history()->currentItem();
    framedata.quickRedirectComing = frame->loader()->quickRedirectComing();
    framedata.canSuspendActiveDOMObjects = frame->document()->canSuspendActiveDOMObjects();
    framedata.canCachePage = frame->loader()->client()->canCachePage();

    return CanCacheFrameImpl(framedata);
}

// These *Impl functions implement the default page cache policy.
bool PageCachePolicy::CanCachePageImpl()
{
    bool canCachePage = true;

    if (!m_backForwardIsActive) {
        PolicyLog("   -The back/forward list is disabled or has 0 capacity.");
        canCachePage = false;
    }
    if (!m_settingsAllowPageCache) {
        PolicyLog("   -Page settings says b/f cache disabled");
        canCachePage = false;
    }
#if ENABLE(DEVICE_ORIENTATION)
    if (m_deviceMotionIsActive) {
        PolicyLog("   -Page is using DeviceMotion");
        canCachePage = false;
    }
    if (m_deviceOrientationIsActive) {
        PolicyLog("   -Page is using DeviceOrientation");
        canCachePage = false;
    }
#endif
    if (m_loadType == FrameLoadTypeReload) {
        PolicyLog("   -Load type is: Reload");
        canCachePage = false;
    } else if (m_loadType == FrameLoadTypeReloadFromOrigin) {
        PolicyLog("   -Load type is: Reload from origin");
        canCachePage = false;
    } else if (m_loadType == FrameLoadTypeSame) {
        PolicyLog("   -Load type is: Same");
        canCachePage = false;
    }


    return canCachePage;
}

bool PageCachePolicy::CanCacheFrameImpl(const FrameData& framedata)
{
    if (!framedata.hasDocumentLoader) {
        PolicyLog("   -There is no DocumentLoader object");
        return false;
    }
    bool canCacheFrame = true;
    if (framedata.mainDocumentError) {
        PolicyLog("   -Main document has an error");
        canCacheFrame = false;
    }
    if (framedata.isErrorPage) {
        PolicyLog("   -Frame is an error page");
        canCacheFrame = false;
    }
    if (framedata.containsPlugins && !framedata.pageCacheSupportsPlugins) {
        PolicyLog("   -Frame contains plugins, and the page cache settings do not support plugins.");
        canCacheFrame = false;
    }
    if (framedata.protocol == "https") {
        PolicyLog("   -Frame is HTTPS");
        canCacheFrame = false;
    }
    if (framedata.responseDenies) {
        PolicyLog("   -Frame response contains NoCache or NoStore");
        canCacheFrame = false;
    }
    if (framedata.hasUnloadListener) {
        PolicyLog("   -Frame has an unload event listener");
        canCacheFrame = false;
    }
#if ENABLE(SQL_DATABASE)
    if (framedata.hasOpenDatabase) {
        PolicyLog("   -Frame has one or more open database handles");
        canCacheFrame = false;
    }
#endif
#if ENABLE(SHARED_WORKERS)
    if (framedata.hasSharedWorkers) {
        PolicyLog("   -Frame has associated SharedWorkers");
        canCacheFrame = false;
    }
#endif
    if (framedata.usesGeolocation) {
        PolicyLog("   -Frame uses Geolocation");
        canCacheFrame = false;
    }
    if (framedata.hasHistoryCurrentItem) {
        PolicyLog("   -No current history item");
        canCacheFrame = false;
    }
    if (framedata.quickRedirectComing) {
        PolicyLog("   -Quick redirect is coming");
        canCacheFrame = false;
    }
    if (framedata.isLoadingInAPISense) {
        PolicyLog("   -DocumentLoader is still loading in API sense");
        canCacheFrame = false;
    }
    if (framedata.loaderIsStopping) {
        PolicyLog("   -DocumentLoader is in the middle of stopping");
        canCacheFrame = false;
    }
    if (!framedata.canSuspendActiveDOMObjects) {
        PolicyLog("   -The document cannot suspend its active DOM Objects");
        canCacheFrame = false;
    }
    if (framedata.appCacheDenied) {
        PolicyLog("   -The application cache denied the page cache.");
        canCacheFrame = false;
    }
    if (!framedata.canCachePage) {
        PolicyLog("   -The client says this frame cannot be cached");
        canCacheFrame = false;
    }
    
    return canCacheFrame;
}

} // namespace WebCore
