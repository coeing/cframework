#include "cmadtv/Logic/Commands/SellBroadcastCmd.h"

#include "cmadtv/Data/Broadcast.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/World/BroadcastDealer.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// SellBroadcastCmd
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF1_INST( SellBroadcastCmd, "CMadTV::SellBroadcastCmd", PlayerCommand );
        
SellBroadcastCmd::SellBroadcastCmd( PlayerContextPtr _spContext, BroadcastPtr _spBroadcast )
:   PlayerCommand( _spContext )
{
    SetBroadcast( _spBroadcast );
}

void SellBroadcastCmd::SetBroadcast( BroadcastPtr _spBroadcast )
{
    m_spBroadcast = _spBroadcast;
    m_u32BroadcastID = m_spBroadcast != NULL ? m_spBroadcast->GetID() : MAX_U32;
}

BroadcastPtr SellBroadcastCmd::GetBroadcast() const
{
    return m_spBroadcast;
}

void SellBroadcastCmd::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    // Serialize broadcast ID
    _Serializer.Serialize( m_u32BroadcastID );
}
        
Unsigned32 SellBroadcastCmd::Verify() const
{
    // Check if bag broadcast
    /*if ( !clBag.HasBroadcast( _spBroadcast ) )
    {
        return false;
    }*/

    return ERROR_NONE;
}

void SellBroadcastCmd::Execute() const
{
    // Get player
    PlayerPtr spPlayer = m_spContext->GetPlayer();

    // Get broadcast dealer
    GamePtr spGame = m_spContext->GetGame();
    BroadcastDealerPtr spDealer = spGame->GetBroadcastDealer();
    
    // Move broadcast
    spPlayer->GetBroadcastBag().RemoveBroadcast( m_spBroadcast );

    // TODO CO Remove after archiv window is implemented
    BroadcastRack& clRack = spPlayer->GetBroadcastRack();
    clRack.RemoveBroadcast( m_spBroadcast );

    spDealer->AddBroadcast( m_spBroadcast );

    // Sell broadcast
    spPlayer->GetBudget().Book( ACCOUNT_BROADCAST_SALES, ACCOUNT_CASH, m_spBroadcast->GetCurrentPrice(), spGame->GetDateTime(), CFoundation::String::Build( "Sold broadcast %s", m_spBroadcast->GetTitle().c_str() ) );
}
        
void SellBroadcastCmd::OnContextChanged()
{
    if ( m_spContext != NULL )
    {
        DatabasePtr spDatabase = m_spContext->GetDatabase();

        // Get broadcast by ID
        m_spBroadcast = spDatabase->GetBroadcast( m_u32BroadcastID );
    }
    else
    {
        m_spBroadcast = BroadcastPtr();
    }
}

