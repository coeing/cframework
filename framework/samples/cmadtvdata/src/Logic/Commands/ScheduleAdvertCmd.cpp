#include "cmadtv/Logic/Commands/ScheduleAdvertCmd.h"

#include "cmadtv/Data/Advert.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/World/AdvertDealer.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/Schedule.h"
#include "cmadtv/Data/Player/Scheduling.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// ScheduleAdvertCmd
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF1_INST( ScheduleAdvertCmd, "CMadTV::ScheduleAdvertCmd", PlayerCommand );
        
ScheduleAdvertCmd::ScheduleAdvertCmd( PlayerContextPtr _spContext, AdvertPtr _spAdvert, const CFoundation::Date& _Date, Hour _eSlot )
:   PlayerCommand( _spContext ),
    m_Date( _Date ),
    m_eSlot( _eSlot )
{
    SetAdvert( _spAdvert );
}

void ScheduleAdvertCmd::SetAdvert( AdvertPtr _spAdvert )
{
    m_spAdvert = _spAdvert;
    m_u32AdvertID = m_spAdvert != NULL ? m_spAdvert->GetID() : MAX_U32;
}

AdvertPtr ScheduleAdvertCmd::GetAdvert() const
{
    return m_spAdvert;
}
       
void ScheduleAdvertCmd::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    // Serialize advert ID
    _Serializer.Serialize( m_u32AdvertID );
    m_Date.Serialize( _Serializer );
    _Serializer.SerializeEnum( m_eSlot );
}
        
Unsigned32 ScheduleAdvertCmd::Verify() const
{
    if ( m_spContext == NULL ||
         m_spAdvert == NULL )
    {
        return ERROR_NONE;
    }

    // Get schedule
    SchedulePtr spSchedule = GetSchedule();
    if ( spSchedule == NULL )
    {
        return ERROR_INVALID_DATE;
    }

    return ERROR_NONE;
}

void ScheduleAdvertCmd::Execute() const
{
    // Get schedule
    SchedulePtr spSchedule = GetSchedule();
    if ( spSchedule == NULL )
    {
        return;
    }

    // Set dropped advert to slot
    spSchedule->SetAdvert( m_eSlot, m_spAdvert );
}
        
void ScheduleAdvertCmd::OnContextChanged()
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

SchedulePtr ScheduleAdvertCmd::GetSchedule() const
{
    if ( m_spContext == NULL )
    {
        return SchedulePtr();
    }

    // Get schedule day
    const CFoundation::Date& today = m_spContext->GetGame()->GetDate();
    ScheduleDay eScheduleDay = SCHEDULE_DAY_END;
    if ( m_Date == today )
    {
        eScheduleDay = SCHEDULE_DAY_TODAY;
    }
    else if ( m_Date == today + 1 )
    {
        eScheduleDay = SCHEDULE_DAY_TOMORROW;
    }

    PlayerPtr spPlayer = m_spContext->GetPlayer();
    if ( spPlayer == NULL )
    {
        return SchedulePtr();
    }
    
    // Get schedule
    SchedulingPtr spScheduling = spPlayer->GetScheduling();
    return spScheduling->GetSchedule( eScheduleDay );
}

