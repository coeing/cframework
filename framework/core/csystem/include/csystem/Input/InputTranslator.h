#ifndef CSYSTEM_INPUT_TRANSLATOR_H
#define CSYSTEM_INPUT_TRANSLATOR_H

#include "cfoundation/Text/String.h"
#include "cfoundation/Enum/EnumConverter.h"

#include "csystem/Input/InputCommand.h"

namespace CSystem
{
    class InputTranslatorBase
    {
    public:
        /// Loads commands from a xml file
        void        LoadFromXML( const CFoundation::String& _strFile, const CFoundation::EnumConverterBase& _clConverter );

        /// Indicates if the passed command was executed
        bool        WasExecuted( Unsigned32 _u32Command, const InputState& _state ) const;

    private:
        typedef std::map< Unsigned32, InputCommandPtr > IdentifierCommandMap;

        /// Input commands
        IdentifierCommandMap    m_mapCommands;
    };

    template< class InputCommandIdentifier >
    class InputTranslator : protected InputTranslatorBase
    {
    public:
        /// Loads commands from a xml file
        void        LoadFromXML( const CFoundation::String& _strFile );

        /// Indicates if the passed command was executed
        bool        WasExecuted( InputCommandIdentifier _eCommand, const InputState& _state ) const;
    };
}

#include "InputTranslator.inl"

#endif
