#include "ced/Input/InputTranslator.h"

using namespace Ced;

template<>
void CFoundation::EnumConverter< InputCommand >::Initialize()
{
    Assign( INPUT_COMMAND_DELETE_SELECTED_OBJECT,   "delete selected object" );
}
        
InputTranslator::InputTranslator()
{
    // Load from xml
    LoadFromXML( "data/configuration/Input.xml" );
}
