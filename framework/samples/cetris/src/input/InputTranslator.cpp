#include "cetris/Input/InputTranslator.h"

using namespace Cetris;

template<>
void CFoundation::EnumConverter< InputCommand >::Initialize()
{
    Assign( INPUT_COMMAND_GAME_PAUSE,       "game pause" );

    Assign( INPUT_COMMAND_GAME_MOVE_LEFT,   "game move left" );
    Assign( INPUT_COMMAND_GAME_MOVE_RIGHT,  "game move right" );
    Assign( INPUT_COMMAND_GAME_MOVE_DOWN,   "game move down" );

    Assign( INPUT_COMMAND_GAME_ROT_CCW,     "game rotate ccw" );
    Assign( INPUT_COMMAND_GAME_ROT_CW,      "game rotate cw" );
}
        
InputTranslator::InputTranslator()
{
    // Load from xml
    LoadFromXML( "data/configuration/Input.xml" );
}
