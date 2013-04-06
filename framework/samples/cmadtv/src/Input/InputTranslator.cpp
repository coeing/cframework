#include "cmadtv/Input/InputTranslator.h"

using namespace CMadTV;

template<>
void CFoundation::EnumConverter< InputCommand >::Initialize()
{
    Assign( INPUT_COMMAND_PLANNING_ADVERT_ARCHIV_WINDOW_CLOSE, "close planning advert archiv" );
    Assign( INPUT_COMMAND_PLANNING_BROADCAST_ARCHIV_WINDOW_CLOSE, "close planning broadcast archiv" );
    Assign( INPUT_COMMAND_PLANNING_RACK_SHELF_WINDOW_CLOSE, "close planning rack shelf" );
    Assign( INPUT_COMMAND_DEBUG_SPEED_PAUSE, "debug speed pause" );
    Assign( INPUT_COMMAND_DEBUG_SPEED_1, "debug speed 1" );
    Assign( INPUT_COMMAND_DEBUG_SPEED_2, "debug speed 2" );
    Assign( INPUT_COMMAND_DEBUG_SPEED_3, "debug speed 3" );
    Assign( INPUT_COMMAND_DEBUG_GET_MONEY, "debug get money" );
}
        
InputTranslator::InputTranslator()
{
    // Load from xml
    LoadFromXML( "data/configuration/Input.xml" );
}
