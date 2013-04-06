#include "cmadtv/Data/Player/Scheduling.h"

#include "cfoundation/DateTime/Date.h"
#include "cfoundation/Enum/EnumIncrement.h"
#include "cfoundation/Serialization/Serializer.h"

#include "cmadtv/Data/Player/Schedule.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// ScheduleDay
///
////////////////////////////////////////////////////////////////////////////////////////////

ScheduleDay& CMadTV::operator++( ScheduleDay& _eScheduleDay )
{
    return enum_increment< ScheduleDay >( _eScheduleDay, SCHEDULE_DAY_TODAY, SCHEDULE_DAY_END );
}

ScheduleDay& CMadTV::operator--( ScheduleDay& _eScheduleDay )
{
    return enum_decrement< ScheduleDay >( _eScheduleDay, SCHEDULE_DAY_TODAY, SCHEDULE_DAY_END );
}  
   
ScheduleDay CMadTV::ComputeScheduleDayFromDate( const CFoundation::Date& _DateToday, const CFoundation::Date& _Date )
{
    if ( _DateToday == _Date )
    {
        return SCHEDULE_DAY_TODAY;
    }
    else if ( _DateToday + 1 == _Date )
    {
        return SCHEDULE_DAY_TOMORROW;
    }
    else
    {
        return SCHEDULE_DAY_END;
    }
}

CFoundation::Date CMadTV::ComputeDateFromScheduleDay( const CFoundation::Date& _DateToday, ScheduleDay _eScheduleDay )
{
    switch ( _eScheduleDay )
    {
    case SCHEDULE_DAY_TODAY:
        return _DateToday;
    case SCHEDULE_DAY_TOMORROW:
        return _DateToday + 1;
    default:
        return CFoundation::Date( 0, 0, 0 );
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Scheduling
///
////////////////////////////////////////////////////////////////////////////////////////////

Scheduling::Scheduling()
{
    for ( Unsigned32 u32Idx = 0; u32Idx < SCHEDULE_DAY_END; ++u32Idx )
    {
        m_aspSchedules[ u32Idx ] = SchedulePtr( new Schedule() );
    }
}

#define SHARED_PTR_COPY( spDest, spSrc )    if ( spDest == NULL || spSrc == NULL ) {    \
                                                spDest = spSrc; }                       \
                                            else {                                      \
                                                *spDest = *spSrc; }
       
void Scheduling::operator=( const Scheduling& _rhs )
{
    for ( ScheduleDay eDay = SCHEDULE_DAY_TODAY; eDay < SCHEDULE_DAY_END; ++eDay )
    {
        SHARED_PTR_COPY( m_aspSchedules[ eDay ], _rhs.m_aspSchedules[ eDay ] );
    }
}
        
void Scheduling::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.SerializeArray< CFoundation::SerializeWrapper_SharedPtrSimple >( m_aspSchedules, SCHEDULE_DAY_END );
}
        
void Scheduling::SetSchedule( ScheduleDay _eDay, SchedulePtr _spSchedule )
{
    m_aspSchedules[ _eDay ] = _spSchedule;
}

SchedulePtr Scheduling::GetSchedule( ScheduleDay _eDay ) const
{
    if ( _eDay < SCHEDULE_DAY_END )
    {
        return m_aspSchedules[ _eDay ];
    }
    else
    {
        return SchedulePtr();
    }
}
        
bool Scheduling::IsBroadcastPlanned( BroadcastPtr _spBroadcast, ScheduleDay _eDayBegin, Hour _eHourBegin ) const
{
    for ( ScheduleDay eDay = _eDayBegin; eDay < SCHEDULE_DAY_END; ++eDay )
    {
        if ( m_aspSchedules[ eDay ]->IsBroadcastPlanned( _spBroadcast, eDay == _eDayBegin ? _eHourBegin : HOUR_BEGIN, HOUR_END ) )
        {
            return true;
        }
    }
    return false;
}
        
void Scheduling::RemoveAdverts( AdvertPtr _spAdvert, ScheduleDay _eDayBegin, Hour _eHourBegin )
{
    for ( ScheduleDay eDay = _eDayBegin; eDay < SCHEDULE_DAY_END; ++eDay )
    {
        m_aspSchedules[ eDay ]->RemoveAdverts( _spAdvert, eDay == _eDayBegin ? _eHourBegin : HOUR_BEGIN, HOUR_END );
    }
}

Unsigned32 Scheduling::GetNumAdverts( AdvertPtr _spAdvert, ScheduleDay _eDayBegin, Hour _eHourBegin, ScheduleDay _eDayEnd, Hour _eHourEnd ) const
{
    Unsigned32 u32NumAdverts = 0;
    for ( ScheduleDay eDay = _eDayBegin; eDay < _eDayEnd; ++eDay )
    {
        u32NumAdverts += m_aspSchedules[ eDay ]->GetNumAdverts( _spAdvert, eDay == _eDayBegin ? _eHourBegin : HOUR_BEGIN, eDay == _eDayEnd - 1 ? _eHourEnd : HOUR_END );
    }
    return u32NumAdverts;
}
      