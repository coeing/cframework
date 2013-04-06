#ifndef CSYSTEM_MOUSE_H
#define CSYSTEM_MOUSE_H

#include "cfoundation/Enum/EnumConverter.h"

namespace CSystem
{
	enum MouseButton
	{
        MOUSE_NONE,

		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
		MOUSE_OTHER,

		MOUSE_END
	};

    enum MouseButtonState
    {
        MBS_NONE,

        MBS_FIRST_DOWN,
        MBS_DOWN,
        MBS_FIRST_UP,
        MBS_UP,

        MBS_END
    };
}

template<>
void CFoundation::EnumConverter< CSystem::MouseButton >::Initialize();

template<>
void CFoundation::EnumConverter< CSystem::MouseButtonState >::Initialize();

#endif
