/*
 * Copyright (C) 2011 Ericsson AB. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Ericsson nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
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
 */

#include "config.h"

#if ENABLE(MEDIA_STREAM)

#include "MediaStreamCenter.h"

#include "IceCandidateDescriptor.h"
#include "MediaStreamDescriptor.h"
#include "SessionDescriptionDescriptor.h"

#include <wtf/MainThread.h>

namespace WebCore {

MediaStreamCenter& MediaStreamCenter::instance()
{
    ASSERT(isMainThread());
    DEFINE_STATIC_LOCAL(MediaStreamCenter, center, ());
    return center;
}

void MediaStreamCenter::endLocalMediaStream(MediaStreamDescriptor* streamDescriptor)
{
    MediaStreamDescriptorOwner* owner = streamDescriptor->owner();
    if (owner)
        owner->streamEnded();
    else
        streamDescriptor->setEnded();
}

#if !PLATFORM(CHROMIUM)

// Empty implementations for ports that build with MEDIA_STREAM enabled by default, but haven't yet implemented MediaStreamCenter.

MediaStreamCenter::MediaStreamCenter()
{
}

MediaStreamCenter::~MediaStreamCenter()
{
}

void MediaStreamCenter::queryMediaStreamSources(PassRefPtr<MediaStreamSourcesQueryClient> client)
{
    MediaStreamSourceVector audioSources, videoSources;
    client->didCompleteQuery(audioSources, videoSources);
}

void MediaStreamCenter::didSetMediaStreamTrackEnabled(MediaStreamDescriptor*, MediaStreamComponent*)
{
}

void MediaStreamCenter::didStopLocalMediaStream(MediaStreamDescriptor*)
{
}

void MediaStreamCenter::didConstructMediaStream(MediaStreamDescriptor*)
{
}

String MediaStreamCenter::constructSDP(IceCandidateDescriptor*)
{
    return "";
}

String MediaStreamCenter::constructSDP(SessionDescriptionDescriptor*)
{
    return "";
}

#endif // !PLATFORM(CHROMIUM)

} // namespace WebCore

#endif // ENABLE(MEDIA_STREAM)
