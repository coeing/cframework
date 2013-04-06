#include "csystem/Input/InputTranslator.h"

#include "cfoundation/XML/XMLDocument.h"
#include "cfoundation/XML/XMLParser.h"

#include "csystem/Input/InputCommand.h"

using namespace CSystem;

void InputTranslatorBase::LoadFromXML( const CFoundation::String& _strFile, const CFoundation::EnumConverterBase& _clEnumConverter )
{
    // Load xml file
    CFoundation::XMLDocumentPtr spDoc = CFoundation::XMLParser::GetInstance().ParseFile( _strFile );
    if ( spDoc == NULL )
    {
        return;
    }

    // Get root element
    CFoundation::XMLElementPtr spRoot = spDoc->GetRootElement();
    CFoundation::XMLNodePtr spChildNode = spRoot->GetFirstChild();
    while ( spChildNode != NULL )
    {
        if ( spChildNode->IsElement() )
        {
            CFoundation::XMLElementPtr spElement = static_pointer_cast< CFoundation::XMLElement >( spChildNode );

            // Get identifier
            const CFoundation::String& strIdentifier = spElement->GetAttribute( "identifier" );
            Unsigned32 u32Identifier = _clEnumConverter.Convert( strIdentifier );
            if ( u32Identifier != 0 )
            {
                const CFoundation::String& strName = spElement->GetName();
                InputCommandPtr spCommand = InputCommand::Create( strName );
                if ( spCommand != NULL )
                {
                    spCommand->LoadFromXML( spElement );
                    m_mapCommands[ u32Identifier ] = spCommand;
                }
            }
        }

        spChildNode = spChildNode->GetNextSibling();
    }
}
        
bool InputTranslatorBase::WasExecuted( Unsigned32 _u32Command, const InputState& _state ) const
{
    IdentifierCommandMap::const_iterator it = m_mapCommands.find( _u32Command );
    if ( it != m_mapCommands.end() )
    {
        return it->second->IsActive( _state );
    }
    else
    {
        return false;
    }
}
