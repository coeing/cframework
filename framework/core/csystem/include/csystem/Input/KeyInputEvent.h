#ifndef CSYSTEM_KEY_INPUT_EVENT_H
#define CSYSTEM_KEY_INPUT_EVENT_H

#include "cfoundation/Events/Delegate.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CSystem
{
    enum KeyInputEvent
    {
        INPUT_EVENT_KEY_FIRST_DOWN,
        INPUT_EVENT_KEY_DOWN,
        INPUT_EVENT_KEY_FIRST_UP,
        INPUT_EVENT_KEY_UP,
    };
    typedef Delegate1< KeyCode > KeyInputEventDelegate;
    typedef SharedPtr< KeyInputEventDelegate > KeyInputEventDelegatePtr;
}

#endif
