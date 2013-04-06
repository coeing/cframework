#ifndef CSYSTEM_INPUT_COMMAND_KEY_H
#define CSYSTEM_INPUT_COMMAND_KEY_H

#include "csystem/Input/InputCommand.h"

namespace CSystem
{
    class InputCommandKey : public InputCommand
    {
    public:
        /// Loads the command from XML
        virtual void        LoadFromXML( CFoundation::XMLElementCPtr _spElement );

        /// Indicates if the command is active
        virtual bool        IsActive( const InputState& _state ) const;

    private:
        /// Key code
        KeyCode             m_eKey;

        /// Activation
        KeyState            m_eActivation;
    };
}

#endif
