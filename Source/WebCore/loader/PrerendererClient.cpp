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
#include "PrerendererClient.h"

#include "Page.h"
#include "Supplementable.h"
#include <wtf/text/AtomicString.h>

#if ENABLE(LINK_PRERENDER)

namespace WebCore {

// Our supplement is declared file local for two reasons: firstly, we can't cons up a full
// Prerenderer yet, as it depends on the document.  But we need something deletable to put
// in a Supplement, and a naked PrerenderClient* is not deletable. Another benefit to doing
// it with a private class is that it keeps our header simple.
class PrerendererClientSupplement : public Supplement<Page> {
public:
    explicit PrerendererClientSupplement(PrerendererClient*);
    virtual ~PrerendererClientSupplement();

    PrerendererClient* prerendererClient() { return m_prerendererClient; }

    static const AtomicString& supplementName();
    static PrerendererClient* prerendererClientFrom(Page*);

private:
    PrerendererClient* m_prerendererClient;
};

PrerendererClientSupplement::PrerendererClientSupplement(PrerendererClient* prerendererClient)
        : m_prerendererClient(prerendererClient)
{
}

// static
const AtomicString& PrerendererClientSupplement::supplementName()
{
    DEFINE_STATIC_LOCAL(AtomicString, name, ("PrerendererClientSupplement"));
    return name;
}

// static
PrerendererClient* PrerendererClientSupplement::prerendererClientFrom(Page* page)
{
    PrerendererClientSupplement* supplement = static_cast<PrerendererClientSupplement*>(Supplement<Page>::from(page, supplementName()));
    ASSERT(supplement);
    return supplement->prerendererClient();
}

// static
PrerendererClient* PrerendererClient::from(Page* page)
{
    return PrerendererClientSupplement::prerendererClientFrom(page);
}

void providePrerendererClientTo(Page* page, PrerendererClient* client)
{
    PrerendererClientSupplement::provideTo(page, PrerendererClientSupplement::supplementName(), adoptPtr(new PrerendererClientSupplement(client)));
}

}

#endif
