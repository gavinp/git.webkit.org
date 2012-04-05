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
#include "Prerenderer.h"

#if ENABLE(LINK_PRERENDER)

#include "Document.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "PrerenderHandle.h"
#include "PrerendererClient.h"
#include "ReferrerPolicy.h"
#include "SecurityPolicy.h"

#include <wtf/RefPtr.h>
#include <wtf/text/CString.h>
#include <wtf/text/WTFString.h>

namespace WebCore {

Prerenderer::Prerenderer(Document* document)
    : ActiveDOMObject(document, this)
    , m_client(adoptPtr(PrerendererClient::create(document)))
{
}

Prerenderer::~Prerenderer()
{
}

Document* Prerenderer::document()
{
    return static_cast<Document*>(scriptExecutionContext());
}

PassRefPtr<PrerenderHandle> Prerenderer::render(const KURL& url)
{
    // Prerenders are unlike requests in most ways (for instance, they pass down fragments, and they don't return data),
    // but they do have referrers.
    const ReferrerPolicy referrerPolicy = document()->referrerPolicy();
    const String referrer = SecurityPolicy::generateReferrerHeader(referrerPolicy, url, document()->frame()->loader()->outgoingReferrer());

    RefPtr<PrerenderHandle> prerenderHandle = adoptRef(new PrerenderHandle(url, referrer, referrerPolicy));

    m_client->willAddPrerender(prerenderHandle->prerender());
    prerenderHandle->add();

    m_handles.append(prerenderHandle);
    return prerenderHandle;
}

void Prerenderer::stop()
{
    while (!m_handles.isEmpty()) {
        RefPtr<PrerenderHandle> handle = m_handles[0].release();
        m_handles.remove(0);
        handle->abandon();
    }
}

}

#endif // ENABLE(LINK_PRERENDER)
