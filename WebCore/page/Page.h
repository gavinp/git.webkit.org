// -*- mode: c++; c-basic-offset: 4 -*-
/*
 * Copyright (C) 2006 Apple Computer, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef PAGE_H
#define PAGE_H

#include "PlatformString.h"
#include <kxmlcore/HashSet.h>
#include <kxmlcore/Noncopyable.h>
#include <kxmlcore/PassRefPtr.h>
#include <kxmlcore/RefPtr.h>

namespace WebCore {

    class Frame;
    class FrameNamespace;
    
    class Page : Noncopyable {
    public:
        Page();
        virtual ~Page();

        void setMainFrame(PassRefPtr<Frame> mainFrame);
        Frame* mainFrame() { return m_mainFrame.get(); }

        void setGroupName(const String&);
        String groupName() const { return m_groupName; }
        const HashSet<Page*>* frameNamespace() const;
        static const HashSet<Page*>* frameNamespace(const String&);

        void incrementFrameCount() { ++m_frameCount; }
        void decrementFrameCount() { --m_frameCount; }
        int frameCount() const { return m_frameCount; }

    private:
        RefPtr<Frame> m_mainFrame;
        int m_frameCount;
        String m_groupName;
    };

} // namespace WebCore
    
#endif // PAGE_H
