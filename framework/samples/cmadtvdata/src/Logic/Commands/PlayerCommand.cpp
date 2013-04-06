#include "cmadtv/Logic/Commands/PlayerCommand.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// PlayerCommand
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF( PlayerCommand, "CMadTV::PlayerCommand" );

PlayerCommand::PlayerCommand( PlayerContextPtr _spContext )
:   m_spContext( _spContext )
{
}

void PlayerCommand::Init( PlayerContextPtr _spContext )
{
    if ( _spContext == m_spContext )
    {
        return;
    }

    m_spContext = _spContext;

    OnContextChanged();
}

void PlayerCommand::Serialize( CFoundation::Serializer& _Serializer )
{
}
        
void PlayerCommand::OnContextChanged()
{
}
