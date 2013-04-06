#ifndef CSYSTEM_INPUT_COMMAND_MOUSE_H
#define CSYSTEM_INPUT_COMMAND_MOUSE_H

#include "csystem/Input/InputCommand.h"

namespace CSystem
{
    class InputCommandMouse : public InputCommand
    {
    public:
        /// Loads the command from XML
        virtual void        LoadFromXML( CFoundation::XMLElementCPtr _spElement );

        /// Indicates if the command is active
        virtual bool        IsActive( const InputState& _state ) const;

    private:
        /// Button
        MouseButton         m_eButton;

        /// Activation
        MouseButtonState    m_eActivation;
    };
}

#endif
