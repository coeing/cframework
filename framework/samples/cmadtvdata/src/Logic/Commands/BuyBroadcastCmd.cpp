#include "cmadtv/Logic/Commands/BuyBroadcastCmd.h"

#include "cmadtv/Data/Broadcast.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/World/BroadcastDealer.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BuyBroadcastCmd
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF1_INST( BuyBroadcastCmd, "CMadTV::BuyBroadcastCmd", PlayerCommand );
        
BuyBroadcastCmd::BuyBroadcastCmd( PlayerContextPtr _spContext, BroadcastPtr _spBroadcast )
:   PlayerCommand( _spContext )
{
    SetBroadcast( _spBroadcast );
}

void BuyBroadcastCmd::SetBroadcast( BroadcastPtr _spBroadcast )
{
    m_spBroadcast = _spBroadcast;
    m_u32BroadcastID = m_spBroadcast != NULL ? m_spBroadcast->GetID() : MAX_U32;
}

BroadcastPtr BuyBroadcastCmd::GetBroadcast() const
{
    return m_spBroadcast;
}

void BuyBroadcastCmd::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    // Serialize broadcast ID
    _Serializer.Serialize( m_u32BroadcastID );
}
        
Unsigned32 BuyBroadcastCmd::Verify() const
{
    PlayerPtr spPlayer = m_spContext->GetPlayer();
    BroadcastBag& clBag = spPlayer->GetBroadcastBag();

    // Get broadcast dealer
    GamePtr spGame = m_spContext->GetGame();
    BroadcastDealerPtr spDealer = spGame->GetBroadcastDealer();

    // Check if dealer broadcast
    if ( !spDealer->HasBroadcast( m_spBroadcast ) )
    {
        return ERROR_BROADCAST_NOT_AT_DEALER;
    }

    // Check if bag not full
    if ( clBag.GetNumBroadcasts() >= BroadcastBag::MAX_NUM_BROADCASTS )
    {
        return ERROR_BAG_FULL;
    }

    // Check if enough money
    if ( m_spBroadcast->GetCurrentPrice() > spPlayer->GetBudget().GetCash() )
    {
        return ERROR_INSUFFICIENT_MONEY;
    }

    return ERROR_NONE;
}

void BuyBroadcastCmd::Execute() const
{
    // Get player
    PlayerPtr spPlayer = m_spContext->GetPlayer();

    // Get broadcast dealer
    GamePtr spGame = m_spContext->GetGame();
    BroadcastDealerPtr spDealer = spGame->GetBroadcastDealer();
    
    // Move broadcast
    spDealer->RemoveBroadcast( m_spBroadcast );
    spPlayer->GetBroadcastBag().AddBroadcast( m_spBroadcast );

    // Buy broadcast
    spPlayer->GetBudget().Book( ACCOUNT_CASH, ACCOUNT_BROADCAST_PURCHASES, m_spBroadcast->GetCurrentPrice(), spGame->GetDateTime(), CFoundation::String::Build( "Bought broadcast %s", m_spBroadcast->GetTitle().c_str() ) );
}
        
void BuyBroadcastCmd::OnContextChanged()
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
