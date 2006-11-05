/* This file is part of the KDE project

   Copyright (C) 1997 Martin Jones (mjones@kde.org)
             (C) 1998 Waldo Bastian (bastian@kde.org)
             (C) 1998, 1999 Torben Weis (weis@kde.org)
             (C) 1999 Lars Knoll (knoll@kde.org)
             (C) 1999 Antti Koivisto (koivisto@kde.org)
   Copyright (C) 2004, 2005, 2006 Apple Computer, Inc.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef FrameView_H
#define FrameView_H

#include "ScrollView.h"
#include "IntSize.h"
#include "PlatformString.h"

namespace WebCore {

class AtomicString;
class Color;
class Clipboard;
class Event;
class EventTargetNode;
class Frame;
class FrameViewPrivate;
class HTMLFrameSetElement;
class IntPoint;
class IntRect;
class PlatformMouseEvent;
class MouseEventWithHitTestResults;
class Node;
class RenderLayer;
class RenderObject;
class PlatformWheelEvent;

template <typename T> class Timer;

class FrameView : public ScrollView {
public:
    FrameView(Frame*);
    virtual ~FrameView();

    Frame* frame() const { return m_frame.get(); }

    void ref() { ++m_refCount; }
    void deref() { if (!--m_refCount) delete this; }
    bool hasOneRef() { return m_refCount == 1; }

    int marginWidth() const { return m_margins.width(); } // -1 means default
    int marginHeight() const { return m_margins.height(); } // -1 means default
    void setMarginWidth(int);
    void setMarginHeight(int);

    virtual void setVScrollbarMode(ScrollbarMode);
    virtual void setHScrollbarMode(ScrollbarMode);
    virtual void setScrollbarsMode(ScrollbarMode);
    
    void print();

    void layout(bool allowSubtree = true);
    bool didFirstLayout() const;
    void layoutTimerFired(Timer<FrameView>*);
    void scheduleRelayout();
    void scheduleRelayoutOfSubtree(Node*);
    void unscheduleRelayout();
    bool haveDelayedLayoutScheduled();
    bool layoutPending() const;

    Node* layoutRoot() const;
    int layoutCount() const;

    bool needsFullRepaint() const;
    void repaintRectangle(const IntRect&, bool immediate);
    void addRepaintInfo(RenderObject*, const IntRect&);

    void resetScrollbars();

    void clear();
    void clearPart();

    bool isTransparent() const;
    void setTransparent(bool isTransparent);

    Color baseBackgroundColor() const;
    void setBaseBackgroundColor(Color);

    void adjustViewSize();
    void initScrollbars();
    
    void setHasBorder(bool);
    bool hasBorder() const;
    
    virtual IntRect windowClipRect() const;
    IntRect windowClipRect(bool clipToContents) const;
    IntRect windowClipRectForLayer(const RenderLayer*, bool clipToLayerContents) const;

    virtual void scrollPointRecursively(int x, int y);
    virtual void setContentsPos(int x, int y);

    String mediaType() const;

    void setUseSlowRepaints();

    void addSlowRepaintObject();
    void removeSlowRepaintObject();

#if PLATFORM(MAC)
    void updateDashboardRegions();
#endif

private:
    void init();

    virtual bool isFrameView() const;

    void setMediaType(const String&);

    bool scrollTo(const IntRect&);

    bool useSlowRepaints() const;

    void restoreScrollbar();

    void applyOverflowToViewport(RenderObject*, ScrollbarMode& hMode, ScrollbarMode& vMode);

    void updateBorder();

    void updateOverflowStatus(bool horizontalOverflow, bool verticalOverflow);

    unsigned m_refCount;
    IntSize m_size;
    IntSize m_margins;
    RefPtr<Frame> m_frame;
    FrameViewPrivate* d;

    friend class Frame;
    friend class FrameMac;

// === to be moved into EventHandler

public:
    void handleMousePressEvent(const PlatformMouseEvent&);
    void handleMouseDoubleClickEvent(const PlatformMouseEvent&);
    virtual void handleMouseMoveEvent(const PlatformMouseEvent&);
    virtual void handleMouseReleaseEvent(const PlatformMouseEvent&);
    void handleWheelEvent(PlatformWheelEvent&);

    bool passMousePressEventToSubframe(MouseEventWithHitTestResults&, Frame*);
    bool passMouseMoveEventToSubframe(MouseEventWithHitTestResults&, Frame*);
    bool passMouseReleaseEventToSubframe(MouseEventWithHitTestResults&, Frame*);
    bool passWheelEventToSubframe(PlatformWheelEvent&, Frame*);
    bool passMousePressEventToScrollbar(MouseEventWithHitTestResults&, PlatformScrollbar*);

    bool mousePressed();
    void setMousePressed(bool);

    void doAutoScroll();

    bool advanceFocus(bool forward);

    bool updateDragAndDrop(const PlatformMouseEvent&, Clipboard*);
    void cancelDragAndDrop(const PlatformMouseEvent&, Clipboard*);
    bool performDragAndDrop(const PlatformMouseEvent&, Clipboard*);

    void scheduleHoverStateUpdate();
    void hoverTimerFired(Timer<FrameView>*);

    void setResizingFrameSet(HTMLFrameSetElement*);

    void scheduleEvent(PassRefPtr<Event>, PassRefPtr<EventTargetNode>, bool tempEvent);

    IntPoint currentMousePosition() const;

    void setIgnoreWheelEvents(bool);

private:
    void invalidateClick();

    Node *nodeUnderMouse() const;

    MouseEventWithHitTestResults prepareMouseEvent(bool readonly, bool active, bool mouseMove, const PlatformMouseEvent&);

    bool dispatchMouseEvent(const AtomicString& eventType, Node* target,
        bool cancelable, int clickCount, const PlatformMouseEvent&, bool setUnder);
    bool dispatchDragEvent(const AtomicString& eventType, Node* target,
        const PlatformMouseEvent&, Clipboard*);

    void dispatchScheduledEvents();

};

}

#endif

