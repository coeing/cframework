#ifndef CSYSTEM_INPUT_COMMAND_H
#define CSYSTEM_INPUT_COMMAND_H

#include "cfoundation/Utils/SharedPtr.h"

#include "csystem/Input/InputState.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( XMLElement );
}

namespace CSystem
{
    class InputCommand;
    typedef SharedPtr< InputCommand > InputCommandPtr;

    class InputCommand
    {
    public:
        /// Factory method to create an input command depending on the passed string
        static InputCommandPtr  Create( const CFoundation::String& _strName );

        /// Loads the command from XML
        virtual void    LoadFromXML( CFoundation::XMLElementCPtr _spElement ) = 0;

        /// Indicates if the command is active
        virtual bool    IsActive( const InputState& _state ) const = 0;

    private:
    };
}

#endif
