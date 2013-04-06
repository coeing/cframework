#include "cmadtv/Logic/Commands/MoveBroadcastCmd.h"

#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Broadcast.h"

#include "cmadtv/Data/Genre.h"
#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/World/BroadcastDealer.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/Scheduling.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// MoveBroadcastCmd
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF1_INST( MoveBroadcastCmd, "CMadTV::MoveBroadcastCmd", PlayerCommand );
        
MoveBroadcastCmd::MoveBroadcastCmd( PlayerContextPtr _spContext, BroadcastPtr _spBroadcast, Location _eFrom, Location _eTo )
:   PlayerCommand( _spContext ),
    m_eLocationFrom( _eFrom ),
    m_eLocationTo( _eTo )
{
    SetBroadcast( _spBroadcast );
}

void MoveBroadcastCmd::SetBroadcast( BroadcastPtr _spBroadcast )
{
    m_spBroadcast = _spBroadcast;
    //m_u32BroadcastID = m_spBroadcast != NULL ? m_spBroadcast->GetID() : MAX_U32;
}

BroadcastPtr MoveBroadcastCmd::GetBroadcast() const
{
    return m_spBroadcast;
}
       
void MoveBroadcastCmd::SetLocations( Location _eFrom, Location _eTo )
{
    m_eLocationFrom = _eFrom;
    m_eLocationTo = _eTo;
}

void MoveBroadcastCmd::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_Context >( m_spBroadcast );
    _Serializer.SerializeEnum( m_eLocationFrom );
    _Serializer.SerializeEnum( m_eLocationTo );
}
        
Unsigned32 MoveBroadcastCmd::Verify() const
{
    if ( m_spContext == NULL ||
         m_spBroadcast == NULL )
    {
        return ERROR_NONE;
    }

    PlayerPtr spPlayer = m_spContext->GetPlayer();
    GamePtr spGame = m_spContext->GetGame();
    if ( spPlayer == NULL )
    {
        return ERROR_NONE;
    }

    BroadcastBag& clBag = spPlayer->GetBroadcastBag();
    BroadcastRack& clRack = spPlayer->GetBroadcastRack();
    
    switch ( m_eLocationFrom )
    {
    case LOCATION_BAG:
        {
            // Check if bag has broadcast
            if ( !clBag.HasBroadcast( m_spBroadcast ) )
            {
                return ERROR_NOT_IN_LOCATION;
            }
        }
        break;
    case LOCATION_RACK:
        {
            // Check if broadcast is in rack
            if ( !clRack.HasBroadcast( m_spBroadcast ) )
            {
                return ERROR_NOT_IN_LOCATION;
            }

            if ( spGame != NULL )
            {
                // Check if currently planned
                // Compute start hour
                Hour eStartHour = spGame->GetHour();
                if ( spGame->GetHourStage() > HOUR_STAGE_BROADCAST )
                {
                    ++eStartHour;
                }

                bool bPlanned = spPlayer->GetScheduling()->IsBroadcastPlanned( m_spBroadcast, SCHEDULE_DAY_TODAY, eStartHour );
                if ( bPlanned )
                {
                    return ERROR_IS_PLANNED;
                }
            }
        }
        break;
    }

    switch ( m_eLocationTo )
    {
    case LOCATION_BAG:
        {
            // Check if bag not full
            if ( clBag.GetNumBroadcasts() >= BroadcastBag::MAX_NUM_BROADCASTS )
            {
                return ERROR_BAG_FULL;
            }
        }
        break;
    case LOCATION_RACK:
        {
            // Check if rack shelf of genre not full
            if ( clRack.GetNumBroadcasts( m_spBroadcast->GetGenre()->GetID() ) >= BroadcastRack::MAX_BROADCASTS_PER_GENRE )
            {
                return ERROR_RACK_SHELF_FULL;
            }
        }
        break;
    }

    return ERROR_NONE;
}

void MoveBroadcastCmd::Execute() const
{
    if ( m_spContext == NULL ||
         m_spBroadcast == NULL )
    {
        return;
    }

    PlayerPtr spPlayer = m_spContext->GetPlayer();
    if ( spPlayer == NULL )
    {
        return;
    }

    BroadcastBag& clBag = spPlayer->GetBroadcastBag();
    BroadcastRack& clRack = spPlayer->GetBroadcastRack();

    switch ( m_eLocationFrom )
    {
    case LOCATION_BAG:
        {
            clBag.RemoveBroadcast( m_spBroadcast );
        }
        break;
    case LOCATION_RACK:
        {
            clRack.RemoveBroadcast( m_spBroadcast );
        }
        break;
    }

    switch ( m_eLocationTo )
    {
    case LOCATION_BAG:
        {
            clBag.AddBroadcast( m_spBroadcast );
        }
        break;
    case LOCATION_RACK:
        {
            clRack.AddBroadcast( m_spBroadcast );
        }
        break;
    }
}
        
void MoveBroadcastCmd::OnContextChanged()
{
    /*
    if ( m_spContext != NULL )
    {
        DatabasePtr spDatabase = m_spContext->GetDatabase();

        // Get broadcast by ID
//        m_spBroadcast = spDatabase->GetBroadcast( m_u32BroadcastID );
    }
    else
    {
        m_spBroadcast = BroadcastPtr();
    }*/
}
