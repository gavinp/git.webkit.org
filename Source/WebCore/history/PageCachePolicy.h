/* google copyright here
 */

#ifndef PageCachePolicy_h
#define PageCachePolicy_h

#include <wtf/FastAllocBase.h>
#include <wtf/Forward.h>
#include <wtf/Noncopyable.h>
#include <wtf/PassOwnPtr.h>
#include <loader/FrameLoaderTypes.h>

/* encapsulate policy for page cache, plus give implementations
 * a chance to instrument for perf measurement
 */

namespace WebCore {
    class Frame;
    class Page;

    class PageCachePolicy  {
        WTF_MAKE_NONCOPYABLE(PageCachePolicy); WTF_MAKE_FAST_ALLOCATED;
    public:
        typedef PassOwnPtr<PageCachePolicy> (FactoryFunction)(Page*);

        PageCachePolicy(Page*);
        bool CanCachePage();

        static PassOwnPtr<PageCachePolicy> DefaultFactoryFunction(Page*);
        static FactoryFunction* GetFactory();

    protected:
        void PolicyLog(const String&);

        virtual bool CanCacheFrameImpl(bool pageCacheSupportsPlugins,
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
                                       bool usesAppCache,
                                       bool canCachePage);

        virtual bool CanCachePageImpl();

        // Page level parameters
        bool m_backForwardIsActive;
        bool m_settingsAllowPageCache;
        bool m_settingsAllowsPlugins;
#if ENABLE(DEVICE_ORIENTATION)
        bool m_deviceMotionIsActive;
        bool m_deviceOrientationIsActive;
#endif
        FrameLoadType m_loadType;

    private:
        bool CanCacheFrame(Frame*);
        
        static FactoryFunction* s_factory;
        Page* m_page;
        int m_logIndentLevel;
    };

} // namespace WebCore

#endif
