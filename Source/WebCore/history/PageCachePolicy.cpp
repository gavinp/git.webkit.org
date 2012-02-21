
#include "config.h"
#include "PageCachePolicy.h"

#include "ApplicationCacheHost.h"
#include "Page.h"
#include "BackForwardController.h"
#include "DeviceMotionController.h"
#include "DeviceOrientationController.h"
#include "Document.h"
#include "DocumentLoader.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "FrameLoaderClient.h"
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
    , m_logIndentLevel( 0)
{
}

PassOwnPtr<PageCachePolicy> PageCachePolicy::DefaultFactoryFunction(Page* page)
{
    return adoptPtr(new PageCachePolicy(page));
}

PageCachePolicy::FactoryFunction* PageCachePolicy::GetFactory() {
    return s_factory;
}

void PageCachePolicy::SetFactory(FactoryFunction* newfactory) {
    s_factory = newfactory;
}

bool PageCachePolicy::CanCachePage()
{
    PolicyLog("--------\n Determining if page can be cached:");

    const bool canCachePage = CanCachePageImpl();
    const bool canCacheMainFrame = CanCacheFrame(m_page->mainFrame());

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

bool PageCachePolicy::CanCachePageImpl() {
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
    }
    else if (m_loadType == FrameLoadTypeReloadFromOrigin) {
        PolicyLog("   -Load type is: Reload from origin");
        canCachePage = false;
    }
    else if (m_loadType == FrameLoadTypeSame) {
        PolicyLog("   -Load type is: Same");
        canCachePage = false;
    }


    return canCachePage;
}

bool PageCachePolicy::CanCacheFrameImpl(const FrameData& framedata) {
    if (!framedata.hasDocumentLoader) {
        PolicyLog("   -There is no DocumentLoader object");
        return false;
    }
    bool canCacheFrame = true;
    if (framedata.mainDocumentError) {
        PolicyLog("   -Main document has an error");
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
    else {
        FrameData framedata;

        // error pages?

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
}


} // namespace WebCore
