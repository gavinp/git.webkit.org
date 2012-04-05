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
#include "Prerender.h"

#include <public/Platform.h>
#include <public/WebReferrerPolicy.h>
#include <public/WebString.h>
#include <public/WebURL.h>

#if ENABLE(LINK_PRERENDER)

namespace WebCore {

static int getNextPrerenderId()
{
    static int nextPrerenderId = 1;
    return nextPrerenderId++;
}

static const WebKit::WebSize getDefaultSize()
{
    return WebKit::WebSize(640, 480);
}

Prerender::Prerender(const KURL& url, const String& referrer, ReferrerPolicy policy)
    : m_id(getNextPrerenderId())
    , m_url(url)
    , m_referrer(referrer)
    , m_referrerPolicy(policy)
    , m_size(getDefaultSize())
    , m_requestorID(0)
{
}

void Prerender::add()
{
    WebKit::Platform::current()->addPrerender(m_id, WebKit::WebURL(m_url), WebKit::WebString(m_referrer), static_cast<WebKit::WebReferrerPolicy>(m_referrerPolicy), m_size, m_requestorID);
}

void Prerender::cancel()
{
    WebKit::Platform::current()->cancelPrerender(m_id);
}

void Prerender::abandon()
{
    WebKit::Platform::current()->abandonPrerender(m_id);
}

}

#endif // ENABLE(LINK_PRERENDER)
