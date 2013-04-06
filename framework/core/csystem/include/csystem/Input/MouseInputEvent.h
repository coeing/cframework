#ifndef CSYSTEM_MOUSE_INPUT_EVENT_H
#define CSYSTEM_MOUSE_INPUT_EVENT_H

#include "cfoundation/Events/Delegate.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CSystem
{
    /// Input events
    enum MouseInputEvent
    {
        INPUT_EVENT_LMB_PRESSED,
        INPUT_EVENT_LMB_RELEASED,
        INPUT_EVENT_LMB_DOUBLE_CLICKED,
        INPUT_EVENT_RMB_PRESSED,
        INPUT_EVENT_RMB_RELEASED,
        INPUT_EVENT_RMB_DOUBLE_CLICKED,
    };
    typedef Delegate0<> InputEventDelegate;
    typedef Delegate0<> InputEventDelegate0;
    typedef SharedPtr< InputEventDelegate > InputEventDelegatePtr;
    typedef SharedPtr< InputEventDelegate0 > InputEventDelegate0Ptr;
}

#endif
