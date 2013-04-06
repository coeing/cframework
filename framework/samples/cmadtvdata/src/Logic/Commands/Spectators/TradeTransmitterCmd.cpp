#include "cmadtv/Logic/Commands/Spectators/TradeTransmitterCmd.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/PlayerCompSpectators.h"
#include "cmadtv/Data/Spectators/Transmitter.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// TradeTransmitterCmd
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF1_INST( TradeTransmitterCmd, "CMadTV::TradeTransmitterCmd", PlayerCommand );
        
TradeTransmitterCmd::TradeTransmitterCmd( PlayerContextPtr _spContext, Mode _eMode, TransmitterPtr _spTransmitter )
:   PlayerCommand( _spContext ),
    m_eMode( _eMode )
{
    SetTransmitter( _spTransmitter );
}

void TradeTransmitterCmd::SetTransmitter( TransmitterPtr _spTransmitter )
{
    m_spTransmitter = _spTransmitter;
    if ( m_eMode == MODE_SELL )
    {
        PlayerPtr spPlayer = m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr();
        if ( spPlayer != NULL )
        {
            // Get index of transmitter
            PlayerCompSpectatorsPtr spCompSpectators = spPlayer->GetCompSpectators();
            m_u32TransmitterID = spCompSpectators->GetTransmitterIdx( m_spTransmitter );
        }
    }
    else
    {
        m_u32TransmitterID = m_spTransmitter != NULL ? m_spTransmitter->GetID() : MAX_U32;
    }
}

TransmitterPtr TradeTransmitterCmd::GetTransmitter() const
{
    return m_spTransmitter;
}

void TradeTransmitterCmd::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    // Serialize mode
    _Serializer.SerializeEnum( m_eMode );

    // Serialize transmitter
    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_SharedPtrSimple >( m_spTransmitter );

    // Serialize ID
    _Serializer.Serialize( m_u32TransmitterID );
}
        
Unsigned32 TradeTransmitterCmd::Verify() const
{
    // Get database
    DatabasePtr spDatabase = m_spContext->GetDatabase();

    // Get map
    MapPtr spMap = spDatabase->GetMap();

    // Get player
    PlayerPtr spPlayer = m_spContext->GetPlayer();

    switch ( m_eMode )
    {
    case MODE_BUY:
        {
            // Check if valid transmitter
            if ( !spDatabase->IsValidTransmitter( m_spTransmitter ) )
            {
                return ERROR_INVALID_TRANSMITTER;
            }

            // Compute price
            CFoundation::Money priceTransmitter = spDatabase->ComputeTransmitterPrice( m_spTransmitter );

            // Check if player has enough money
            Budget& playerBudget = spPlayer->GetBudget();
            if ( playerBudget.GetCash() < priceTransmitter )
            {
                return ERROR_INSUFFICIENT_MONEY;
            }
        }
        break;
    case MODE_SELL:
        {
            // Get spectators component
            PlayerCompSpectatorsPtr spCompSpectators = spPlayer->GetCompSpectators();

            // Get transmitters
            const std::vector< TransmitterPtr >& aspTransmitters = spCompSpectators->GetTransmitters();

            // Check if player has transmitter
            TransmitterVec::const_iterator it = std::find( aspTransmitters.begin(), aspTransmitters.end(), m_spTransmitter );

            Unsigned32 u32NumTransmitters = aspTransmitters.size();

            // Don't sell last transmitter
            if ( u32NumTransmitters <= 1 )
            {
                return ERROR_CANT_SELL_LAST_TRANSMITTER;
            }    
        }
        break;
    }

    return ERROR_NONE;
}

void TradeTransmitterCmd::Execute() const
{
    // Get database
    DatabasePtr spDatabase = m_spContext->GetDatabase();

    // Get player
    PlayerPtr spPlayer = m_spContext->GetPlayer();

    // Get game
    GamePtr spGame = m_spContext->GetGame();

    // Compute price
    CFoundation::Money priceTransmitter = spDatabase->ComputeTransmitterPrice( m_spTransmitter );

    switch ( m_eMode )
    {
    case MODE_BUY:
        {
            // Buy transmitter
            Budget& playerBudget = spPlayer->GetBudget();
            playerBudget.Book( ACCOUNT_CASH, ACCOUNT_TRANSMITTERS_BOUGHT, priceTransmitter, spGame->GetDateTime(), "Bought antenna" );

            // Add transmitter
            PlayerCompSpectatorsPtr spCompSpectators = spPlayer->GetCompSpectators();
            spCompSpectators->AddTransmitter( m_spTransmitter );
        }
        break;
    case MODE_SELL:
        {
            // Sell transmitter
            Budget& playerBudget = spPlayer->GetBudget();
            playerBudget.Book( ACCOUNT_SUNDRY_INCOME, ACCOUNT_CASH, priceTransmitter / 2, spGame->GetDateTime(), "Sold antenna" );

            // Remove transmitter
            PlayerCompSpectatorsPtr spCompSpectators = spPlayer->GetCompSpectators();
            spCompSpectators->RemoveTransmitter( m_spTransmitter );
        }
        break;
    }
}
        
void TradeTransmitterCmd::OnContextChanged()
{
    if ( m_spContext != NULL )
    {
        DatabasePtr spDatabase = m_spContext->GetDatabase();

        if ( m_spTransmitter != NULL &&
             m_spTransmitter->GetType() == Transmitter::TYPE_SATELLITE )
        {
            // Get transmitter by ID
            m_spTransmitter = spDatabase->GetSatellite( m_u32TransmitterID );
        }
        else if ( m_eMode == MODE_SELL )
        {
            // Get player
            PlayerPtr spPlayer = m_spContext->GetPlayer();

            // Get transmitter by index
            PlayerCompSpectatorsPtr spCompSpectators = spPlayer->GetCompSpectators();
            m_spTransmitter = spCompSpectators->GetTransmitter( m_u32TransmitterID );
        }
    }
    else
    {
        m_spTransmitter = TransmitterPtr();
    }
}
