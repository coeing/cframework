#ifndef CGUI_STATE_H
#define CGUI_STATE_H

#include "cfoundation/Enum/EnumConverter.h"

namespace CGui
{
    enum StateFlag
    {
        STATE_FLAG_NONE = 0,

        STATE_FLAG_HIGHLIGHT = 1,
        STATE_FLAG_ACTIVE = 2,
        STATE_FLAG_DISABLED = 4,
        STATE_FLAG_MODIFIED = 8,
        STATE_FLAG_FOCUSED = 16,
        STATE_FLAG_INFORMATION = 32
    };

    enum State
    {
        STATE_BEGIN,

        STATE_NONE = STATE_BEGIN,

        STATE_NORMAL,
        STATE_HIGHLIGHT,
        STATE_ACTIVE,
        STATE_ACTIVE_HIGHLIGHT,
        STATE_DISABLED,
        STATE_MODIFIED,
        STATE_INFORMATION,

        STATE_END,

        NUM_STATES = STATE_END
    };

    State& operator++( State& _eState );

    /// Returns the fallback state for the passed state
    State   GetFallbackState( const State& _eState );

    /// Indicates if in the passed state the passed flag is set
    bool    IsStateFlagSet( const State& _eState, const StateFlag& _eStateFlag );
}

template <>
void CFoundation::EnumConverter< CGui::State >::Initialize();

#endif
