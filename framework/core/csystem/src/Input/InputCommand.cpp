#include "csystem/Input/InputCommand.h"

#include "csystem/Input/InputCommandCombined.h"
#include "csystem/Input/InputCommandKey.h"
#include "csystem/Input/InputCommandMouse.h"

using namespace CSystem;

InputCommandPtr InputCommand::Create( const CFoundation::String& _strName )
{
    InputCommandPtr spCommand;
    if ( _strName == "Command" )
    {
        spCommand = InputCommandPtr( new InputCommandCombined() );
    }
    else if ( _strName == "Mouse" )
    {
        spCommand = InputCommandPtr( new InputCommandMouse() );
    }
    else if ( _strName == "Key" )
    {
        spCommand = InputCommandPtr( new InputCommandKey() );
    }
    return spCommand;
}
