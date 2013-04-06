#include "cgui/GUI/State.h"

#include "cfoundation/Enum/EnumIncrement.h"

using namespace CGui;

State& CGui::operator++( State& _eState )
{
    return enum_increment< State >( _eState, STATE_BEGIN, STATE_END );
}
    
State CGui::GetFallbackState( const State& _eState )
{
    switch ( _eState )
    {
    case STATE_NORMAL:
        return STATE_NONE;
    case STATE_ACTIVE_HIGHLIGHT:
        return STATE_ACTIVE;
    default:
        return STATE_NORMAL;
    }
}

bool CGui::IsStateFlagSet( const State& _eState, const StateFlag& _eStateFlag )
{
    switch ( _eState )
    {
    case STATE_NORMAL:
        return false;
    case STATE_HIGHLIGHT:
        return _eStateFlag == STATE_FLAG_HIGHLIGHT;
    case STATE_ACTIVE:
        return _eStateFlag == STATE_FLAG_ACTIVE;
    case STATE_ACTIVE_HIGHLIGHT:
        return _eStateFlag == STATE_FLAG_HIGHLIGHT || _eStateFlag == STATE_FLAG_ACTIVE;
    case STATE_DISABLED:
        return _eStateFlag == STATE_FLAG_DISABLED;
    case STATE_MODIFIED:
        return _eStateFlag == STATE_FLAG_MODIFIED;
    case STATE_INFORMATION:
        return _eStateFlag == STATE_FLAG_INFORMATION;
    default:
        return false;
    }
}

template <>
void CFoundation::EnumConverter< State >::Initialize()
{
    Assign( STATE_NORMAL,           "state_normal" );
    Assign( STATE_HIGHLIGHT,        "state_highlight" );
    Assign( STATE_ACTIVE,           "state_active" );
    Assign( STATE_ACTIVE_HIGHLIGHT, "state_active_highlight" );
    Assign( STATE_DISABLED,         "state_disabled" );
    Assign( STATE_MODIFIED,         "state_modified" );
    Assign( STATE_INFORMATION,      "state_information" );
}
