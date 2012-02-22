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

#ifndef PageCachePolicy_h
#define PageCachePolicy_h

#include <wtf/FastAllocBase.h>
#include <wtf/Forward.h>
#include <wtf/Noncopyable.h>
#include <wtf/PassOwnPtr.h>
#include <wtf/text/WTFString.h>
#include "FrameLoaderTypes.h"

namespace WebCore {
    class Frame;
    class Page;

    class PageCachePolicy  {
        WTF_MAKE_NONCOPYABLE(PageCachePolicy); WTF_MAKE_FAST_ALLOCATED;
    public:
        typedef PassOwnPtr<PageCachePolicy> (FactoryFunction)(Page*);

        PageCachePolicy(Page*);
        bool CanCachePage();

        static void SetFactory(FactoryFunction*);
        static FactoryFunction* GetFactory();

    protected:
        struct FrameData {
            bool isErrorPage;
            bool pageCacheSupportsPlugins;
            bool hasDocumentLoader;
            bool mainDocumentError;
            bool containsPlugins;
            String protocol;
            bool responseDenies;
            bool hasUnloadListener;
#if ENABLE(SQL_DATABASE)
            bool hasOpenDatabase;
#endif
#if ENABLE(SHARED_WORKERS)
            bool hasSharedWorkers;
#endif                                   
            bool usesGeolocation;
            bool hasHistoryCurrentItem;
            bool quickRedirectComing;
            bool isLoadingInAPISense;
            bool loaderIsStopping;
            bool canSuspendActiveDOMObjects;
            bool appCacheDenied;
            bool canCachePage;
        };

        void PolicyLog(const String&);

        // Override these two functions for alternative policies.
        virtual bool CanCacheFrameImpl(const FrameData&);
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
        static PassOwnPtr<PageCachePolicy> DefaultFactoryFunction(Page*);
        bool CanCacheFrame(Frame*);
        
        static FactoryFunction* s_factory;
        Page* m_page;
        int m_logIndentLevel;
    };

} // namespace WebCore

#endif
