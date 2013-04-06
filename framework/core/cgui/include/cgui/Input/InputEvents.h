#ifndef CGUI_INPUT_EVENTS_H
#define CGUI_INPUT_EVENTS_H

#include "cfoundation/Events/Delegate.h"

namespace CGui
{
    class GuiObject;
    typedef SharedPtr< GuiObject > GuiObjectPtr;

    /// Input events
    enum InputEvent
    {
        INPUT_EVENT_POINTED_OBJECT_CHANGED,
        INPUT_EVENT_FOCUSED_OBJECT_CHANGED
    };
    typedef Delegate2< GuiObjectPtr, GuiObjectPtr > InputEventDelegate;
    typedef SharedPtr< InputEventDelegate > InputEventDelegatePtr;
}

#endif
