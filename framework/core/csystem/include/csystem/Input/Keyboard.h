#ifndef CSYSTEM_KEYBOARD_H
#define CSYSTEM_KEYBOARD_H

#include "cfoundation/Enum/EnumConverter.h"

namespace CSystem
{
	enum KeyCode
	{
		KEY_BACKSPACE = 8,
		KEY_TAB = 9,
		KEY_RETURN = 13,
		KEY_SHIFT = 16,
		KEY_ESC = 27,

		KEY_SPACE = 32,

		KEY_PRIOR,
		KEY_NEXT,
		KEY_END,
		KEY_HOME,

		KEY_LEFT,
		KEY_UP,
		KEY_RIGHT,
		KEY_DOWN,

		KEY_ENTER = 43,

		KEY_DEL = 46,

		KEY_0 = 48,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9 = 57,

		KEY_A = 65,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z = 90,

		KEY_NUMPAD_0 = 96,
		KEY_NUMPAD_1,
		KEY_NUMPAD_2,
		KEY_NUMPAD_3,
		KEY_NUMPAD_4,
		KEY_NUMPAD_5,
		KEY_NUMPAD_6,
		KEY_NUMPAD_7,
		KEY_NUMPAD_8,
		KEY_NUMPAD_9,

		KEY_F1 = 112,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_F11,
		KEY_F12,

		KEY_COMMA = 188,
		KEY_POINT = 190,

        NUM_KEYS = 256
	};

    enum KeyState
    {
        KEY_STATE_NONE,

        KEY_STATE_FIRST_DOWN,
        KEY_STATE_DOWN,
        KEY_STATE_FIRST_UP,
        KEY_STATE_UP,

        KEY_STATE_END
    };

	enum KeyboardEvent
	{
		KE_UNDEFINED,
		KE_DOWN,
		KE_UP,
		NUM_KEYBOARD_EVENTS
	};

	wchar_t KeyCodeToChar( const KeyCode &eKeyCode );
}

template<>
void CFoundation::EnumConverter< CSystem::KeyCode >::Initialize();

template<>
void CFoundation::EnumConverter< CSystem::KeyState >::Initialize();

#endif
