#include "cmadtv/Logic/Commands/MoveAdvertCmd.h"

#include "cmadtv/Data/Advert.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/World/AdvertDealer.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// MoveAdvertCmd
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF1_INST( MoveAdvertCmd, "CMadTV::MoveAdvertCmd", PlayerCommand );
        
MoveAdvertCmd::MoveAdvertCmd( PlayerContextPtr _spContext, AdvertPtr _spAdvert, Location _eFrom, Location _eTo )
:   PlayerCommand( _spContext ),
    m_eLocationFrom( _eFrom ),
    m_eLocationTo( _eTo )
{
    SetAdvert( _spAdvert );
}

void MoveAdvertCmd::SetAdvert( AdvertPtr _spAdvert )
{
    m_spAdvert = _spAdvert;
    m_u32AdvertID = m_spAdvert != NULL ? m_spAdvert->GetID() : MAX_U32;
}

AdvertPtr MoveAdvertCmd::GetAdvert() const
{
    return m_spAdvert;
}
       
void MoveAdvertCmd::SetLocations( Location _eFrom, Location _eTo )
{
    m_eLocationFrom = _eFrom;
    m_eLocationTo = _eTo;
}

void MoveAdvertCmd::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    // Serialize broadcast ID
    _Serializer.Serialize( m_u32AdvertID );
    _Serializer.SerializeEnum( m_eLocationFrom );
    _Serializer.SerializeEnum( m_eLocationTo );
}
        
Unsigned32 MoveAdvertCmd::Verify() const
{
    if ( m_spContext == NULL ||
         m_spAdvert == NULL )
    {
        return ERROR_NONE;
    }

    PlayerPtr spPlayer = m_spContext->GetPlayer();
    GamePtr spGame = m_spContext->GetGame();
    if ( spPlayer == NULL ||
         spGame == NULL )
    {
        return ERROR_NONE;
    }

    AdvertDealerPtr spDealer = spGame->GetAdvertDealer();
    AdvertBag& clBag = spPlayer->GetAdvertBag();

    // Check if advert is in location it's expected
    switch ( m_eLocationFrom )
    {
    case LOCATION_DEALER:
        {
            if ( !spDealer->HasAdvert( m_spAdvert ) )
            {
                return ERROR_ADVERT_NOT_AT_DEALER;
            }
        }
        break;
    case LOCATION_BAG:
        {
            if ( !clBag.HasAdvert( m_spAdvert ) )
            {
                return ERROR_ADVERT_NOT_IN_BAG;
            }
        }
        break;
    default:
        {
            return ERROR_INVALID_LOCATION;
        }
        break;
    }

    // Check if advert the location advert should move to
    switch ( m_eLocationTo )
    {
    case LOCATION_DEALER:
        break;
    case LOCATION_BAG:
        {
            // Check if bag not full
            if ( clBag.GetNumAdverts() >= AdvertBag::MAX_NUM_ADVERTS )
            {
                return ERROR_BAG_IS_FULL;
            }

            // TODO Check if advert owner would make a contract with player
        }
        break;   
    case LOCATION_CONTRACTS:
        break;
    default:
        {
            return ERROR_INVALID_LOCATION;
        }
        break;
    }

    return ERROR_NONE;
}

void MoveAdvertCmd::Execute() const
{
    if ( m_spContext == NULL ||
         m_spAdvert == NULL )
    {
        return;
    }

    PlayerPtr spPlayer = m_spContext->GetPlayer();
    GamePtr spGame = m_spContext->GetGame();
    if ( spPlayer == NULL ||
         spGame == NULL )
    {
        return;
    }

    AdvertDealerPtr spDealer = spGame->GetAdvertDealer();
    AdvertBag& clBag = spPlayer->GetAdvertBag();
    AdvertContracts& clContracts = spPlayer->GetAdvertContracts();

    // Remove from location
    switch ( m_eLocationFrom )
    {
    case LOCATION_DEALER:
        {
            spDealer->RemoveAdvert( m_spAdvert );
        }
        break;
    case LOCATION_BAG:
        {
            clBag.RemoveAdvert( m_spAdvert );
        }
        break;    
    default:
        return;
    }

    // Add to location
    switch ( m_eLocationTo )
    {
    case LOCATION_DEALER:
        {
            spDealer->AddAdvert( m_spAdvert );
        }
        break;
    case LOCATION_BAG:
        {
            clBag.AddAdvert( m_spAdvert );
        }
        break;   
    case LOCATION_CONTRACTS:
        {
            // Start new contract
            clContracts.StartContract( m_spAdvert, spGame->GetDateTime().GetDate() );
        }
        break;
    default:
        return;
    }
}
        
void MoveAdvertCmd::OnContextChanged()
{
    if ( m_spContext != NULL )
    {
        DatabasePtr spDatabase = m_spContext->GetDatabase();

        // Get advert by ID
        m_spAdvert = spDatabase->GetAdvert( m_u32AdvertID );
    }
    else
    {
        m_spAdvert = AdvertPtr();
    }
}
