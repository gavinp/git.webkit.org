
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
    , m_deviceMotionIsActive(page->deviceMotionController() && page->deviceMotionController()->isActive())
    , m_deviceOrientationIsActive(page->deviceOrientationController() && page->deviceOrientationController()->isActive())
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

bool PageCachePolicy::CanCachePage()
{
    PolicyLog("--------\n Determining if page can be cached:");

    bool canCacheMainFrame = CanCacheFrame(m_page->mainFrame());
    bool canCachePage = CanCachePageImpl();

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

bool PageCachePolicy::CanCacheFrameImpl(bool pageCacheSupportsPlugins,
                                        bool hasDocumentLoader,
                                        bool mainDocumentError,
                                        bool containsPlugins,
                                        const String& protocol,
                                        bool hasUnloadListener,
#if ENABLE(SQL_DATABASE)
                                        bool hasOpenDatabase,
#endif
#if ENABLE(SHARED_WORKERS)
                                        bool hasSharedWorkers,
#endif                                   
                                        bool usesGeolocation,
                                        bool hasHistoryCurrentItem,
                                        bool quickRedirectComing,
                                        bool isLoadingInAPISense,
                                        bool loaderIsStopping,
                                        bool canSuspendActiveDOMObjects,
                                        bool appCacheDenies,
                                        bool canCachePage) {
    if (!hasDocumentLoader) {
        PolicyLog("   -There is no DocumentLoader object");
        return false;
    }
    bool canCacheFrame = true;
    if (mainDocumentError) {
        PolicyLog("   -Main document has an error");
        canCacheFrame = false;
    }
    if (containsPlugins && !pageCacheSupportsPlugins) {
        PolicyLog("   -Frame contains plugins, and the page cache settings do not support plugins.");
        canCacheFrame = false;
    }
    if (protocol == "https") {
        PolicyLog("   -Frame is HTTPS");
        canCacheFrame = false;
    }
    if (hasUnloadListener) {
        PolicyLog("   -Frame has an unload event listener");
        canCacheFrame = false;
    }
#if ENABLE(SQL_DATABASE)
    if (hasOpenDatabase) {
        PolicyLog("   -Frame has one or more open database handles");
        canCacheFrame = false;
    }
#endif
#if ENABLE(SHARED_WORKERS)
    if (hasSharedWorkers) {
        PolicyLog("   -Frame has associated SharedWorkers");
        canCacheFrame = false;
    }
#endif
    if (usesGeolocation) {
        PolicyLog("   -Frame uses Geolocation");
        canCacheFrame = false;
    }
    if (hasHistoryCurrentItem) {
        PolicyLog("   -No current history item");
        canCacheFrame = false;
    }
    if (quickRedirectComing) {
        PolicyLog("   -Quick redirect is coming");
        canCacheFrame = false;
    }
    if (isLoadingInAPISense) {
        PolicyLog("   -DocumentLoader is still loading in API sense");
        canCacheFrame = false;
    }
    if (loaderIsStopping) {
        PolicyLog("   -DocumentLoader is in the middle of stopping");
        canCacheFrame = false;
    }
    if (!canSuspendActiveDOMObjects) {
        PolicyLog("   -The document cannot suspend its active DOM Objects");
        canCacheFrame = false;
    }
    if (appCacheDenies) {
        PolicyLog("   -The application cache denied the page cache.");
        canCacheFrame = false;
    }
    if (!canCachePage) {
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
    else
        return CanCacheFrameImpl(
            frame->page()->settings()->pageCacheSupportsPlugins(),
            frame->loader()->documentLoader(),
            frame->loader()->documentLoader() && frame->loader()->documentLoader()->mainDocumentError().isNull(),
            // error pages?
            frame->loader()->subframeLoader()->containsPlugins(),
            frame->document()->url().protocol(),
            frame->domWindow() && frame->domWindow()->hasEventListeners(eventNames().unloadEvent),
#if ENABLE(SQL_DATABASE)
            frame->document()->hasOpenDatabases(),
#endif
#if ENABLE(SHARED_WORKERS)
        SharedWorkerRepository::hasSharedWorkers(frame->document()),
#endif
            frame->document()->usingGeolocation(),
            frame->loader()->history()->currentItem(),
            frame->loader()->quickRedirectComing(),
            frame->loader()->documentLoader() && frame->loader()->documentLoader()->isLoadingInAPISense(),
            frame->loader()->documentLoader() && frame->loader()->documentLoader()->isStopping(),
            frame->document()->canSuspendActiveDOMObjects(),
            frame->loader()->documentLoader() && frame->loader()->documentLoader()->applicationCacheHost()->canCacheInPageCache(),
            frame->loader()->client()->canCachePage());
}


} // namespace WebCore
