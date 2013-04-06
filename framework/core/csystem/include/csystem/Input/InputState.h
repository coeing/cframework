#ifndef CSYSTEM_INPUT_STATE_H
#define CSYSTEM_INPUT_STATE_H

#include <vector>

#include "cfoundation/Math/Vector.h"

#include "csystem/Input/Keyboard.h"
#include "csystem/Input/Mouse.h"

namespace CSystem
{
    class InputState
    {
    public:
        /// Special events
        enum Event
        {
            EVENT_WINDOW_ACTIVATED = 1 << 1,
            EVENT_WINDOW_DEACTIVATED = 1 << 2,
        };

        /// Constructor
        InputState();

        /// Indicates if the passed mouse button is down
        bool                    IsDown( MouseButton _eButton ) const;

        /// Indicates if the passed key is down
        bool                    IsDown( KeyCode _eKey ) const;

        /// Indicates if the passed mouse button was pressed this frame
        bool                    WasPressed( MouseButton _eButton ) const;

        /// Indicates if the passed key was pressed this frame
        bool                    WasPressed( KeyCode _eKey ) const;

        /// Returns the current mouse position
        CFoundation::Vector2Di      GetMousePosition() const;

        /// Returns the mouse movement in the current frame
        CFoundation::Vector2Di      GetMouseMovement() const;

        /// Current mouse position
		CFoundation::Vector2Di      m_vMousePosition;

        /// Last mouse position
		CFoundation::Vector2Di      m_vLastMousePosition;

        /// Mouse button states
		MouseButtonState        m_eMouseButtonStates[ MOUSE_END ];

        /// Key states
		KeyState                m_eKeyStates[ NUM_KEYS ];

        /// Typed characters
        std::vector< wchar_t >  m_awcTypedCharacters;

        /// Events (see Event enum)
        Unsigned32              m_u32Events;

    private:
    };
}

#endif
