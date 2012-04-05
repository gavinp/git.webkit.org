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

#ifndef Prerender_h
#define Prerender_h

#include "KURL.h"
#include "ReferrerPolicy.h"
#include <public/WebSize.h>
#include <wtf/text/WTFString.h>

#if ENABLE(LINK_PRERENDER)

namespace WebCore {

class Prerender {
public:
    Prerender(const KURL&, const String& referrer, ReferrerPolicy);

    void add();
    void cancel();
    void abandon();

    // Allows the prerender to be matched up with its requestor.
    int requestorID() const { return m_requestorID; }
    void setRequestorID(int requestorID) { m_requestorID = requestorID; }

    // Setting the initial size avoids unneeded resizes. This is particluarly important on platforms that do not support resize.
    const WebKit::WebSize& size() const { return m_size; }
    void setSize(const WebKit::WebSize& size) { m_size = size; }

private:
    const int m_id;
    const KURL m_url;
    const String m_referrer;
    const ReferrerPolicy m_referrerPolicy;

    WebKit::WebSize m_size;
    int m_requestorID;
};

}

#endif // ENABLE(LINK_PRERENDER)

#endif // Prerender_h
