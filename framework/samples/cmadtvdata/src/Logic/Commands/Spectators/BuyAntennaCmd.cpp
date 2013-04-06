#include "cmadtv/Logic/Commands/Spectators/BuyAntennaCmd.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/PlayerCompSpectators.h"
#include "cmadtv/Data/Spectators/Map.h"
#include "cmadtv/Data/Spectators/Transmitter.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BuyAntennaCmd
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF1_INST( BuyAntennaCmd, "CMadTV::BuyAntennaCmd", PlayerCommand );
        
BuyAntennaCmd::BuyAntennaCmd( PlayerContextPtr _spContext, const CFoundation::Vector2Di& _vPosition )
:   PlayerCommand( _spContext ),
    m_vPosition( _vPosition )
{
}

void BuyAntennaCmd::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    m_vPosition.Serialize( _Serializer );
}
        
Unsigned32 BuyAntennaCmd::Verify() const
{
    // Get database
    DatabasePtr spDatabase = m_spContext->GetDatabase();

    // Get map
    MapPtr spMap = spDatabase->GetMap();

    // Get player
    PlayerPtr spPlayer = m_spContext->GetPlayer();

    // Check if valid transmitter
    if ( !spMap->IsValidPosition( m_vPosition ) )
    {
        return ERROR_INVALID_TRANSMITTER;
    }

    // Compute price
    CFoundation::Money priceTransmitter = spDatabase->ComputeAntennaPrice( m_vPosition );

    // Check if player has enough money
    Budget& playerBudget = spPlayer->GetBudget();
    if ( playerBudget.GetCash() < priceTransmitter )
    {
        return ERROR_INSUFFICIENT_MONEY;
    }
    
    return ERROR_NONE;
}

void BuyAntennaCmd::Execute() const
{
    // Get database
    DatabasePtr spDatabase = m_spContext->GetDatabase();

    // Get player
    PlayerPtr spPlayer = m_spContext->GetPlayer();

    // Get game
    GamePtr spGame = m_spContext->GetGame();

    // Compute price
    CFoundation::Money priceTransmitter = spDatabase->ComputeAntennaPrice( m_vPosition );

    // Buy transmitter
    Budget& playerBudget = spPlayer->GetBudget();
    playerBudget.Book( ACCOUNT_CASH, ACCOUNT_TRANSMITTERS_BOUGHT, priceTransmitter, spGame->GetDateTime(), "Bought antenna" );

    // Create transmitter
    TransmitterPtr spTransmitter = spGame->CreateTransmitter();
    spTransmitter->Init( Transmitter::TYPE_ANTENNA, m_vPosition.GetX(), m_vPosition.GetY() );

    // Add transmitter
    PlayerCompSpectatorsPtr spCompSpectators = spPlayer->GetCompSpectators();
    spCompSpectators->AddTransmitter( spTransmitter );
}
        