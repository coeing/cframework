#ifndef CSYSTEM_INPUT_COMMAND_COMBINED_H
#define CSYSTEM_INPUT_COMMAND_COMBINED_H

#include "csystem/Input/InputCommand.h"

namespace CSystem
{
    class InputCommandCombined : public InputCommand
    {
    public:
        /// Loads the command from XML
        virtual void        LoadFromXML( CFoundation::XMLElementCPtr _spElement );

        /// Indicates if the command is active
        virtual bool        IsActive( const InputState& _state ) const;

    private:
        typedef std::vector< InputCommandPtr > InputCommandVec;

        /// Sub commands
        InputCommandVec     m_aspSubCommands;
    };
}

#endif
