#include "csystem/Input/Mouse.h"

using namespace CSystem;

template<>
void CFoundation::EnumConverter< MouseButton >::Initialize()
{
    Assign( MOUSE_LEFT,     "left" );
    Assign( MOUSE_MIDDLE,   "middle" );
    Assign( MOUSE_RIGHT,    "right" );
    Assign( MOUSE_OTHER,    "other" );
}

template<>
void CFoundation::EnumConverter< MouseButtonState >::Initialize()
{
    Assign( MBS_FIRST_DOWN, "first down" );
    Assign( MBS_DOWN,       "down" );
    Assign( MBS_FIRST_UP,   "first up" );
    Assign( MBS_UP,         "up" );
}
