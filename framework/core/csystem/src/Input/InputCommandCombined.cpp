#include "csystem/Input/InputCommandCombined.h"

#include "cfoundation/XML/XMLElement.h"

using namespace CSystem;
        
void InputCommandCombined::LoadFromXML( CFoundation::XMLElementCPtr _spElement )
{
    CFoundation::XMLNodeCPtr spChildNode = _spElement->GetFirstChild();
    while ( spChildNode != NULL )
    {
        if ( spChildNode->IsElement() )
        {
            CFoundation::XMLElementCPtr spElement = static_pointer_cast< const CFoundation::XMLElement >( spChildNode );
            InputCommandPtr spSubCommand = InputCommand::Create( spElement->GetName() );
            if ( spSubCommand != NULL )
            {
                spSubCommand->LoadFromXML( spElement );
                m_aspSubCommands.push_back( spSubCommand );
            }
        }

        spChildNode = spChildNode->GetNextSibling();
    }
}

bool InputCommandCombined::IsActive( const InputState& _state ) const
{
    Unsigned32 u32NumSubCommands = m_aspSubCommands.size();
    for ( Unsigned32 u32Idx = 0; u32Idx != u32NumSubCommands; ++u32Idx )
    {
        if ( !m_aspSubCommands[ u32Idx ]->IsActive( _state ) )
        {
            return false;
        }
    }

    return true;
}
