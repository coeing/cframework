#include "cmadtv/Data/Game/Conditions/MissionConditionAdvertsSent.h"

#include "cfoundation/Serialization/Serializer.h"

#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/Scheduling.h"

using namespace CMadTV;

MissionConditionAdvertsSent::MissionConditionAdvertsSent()
:   m_u32NumSentTarget( 0 ),
    m_u32NumSent( 0 )
{
    // Create delegates
    m_spDelegateAdvertSent = SchedulingAdvertEventDelegate2Ptr( new SchedulingAdvertEventDelegate2( this, &MissionConditionAdvertsSent::OnAdvertSent ) );
}
        
CFoundation::String MissionConditionAdvertsSent::GetType() const
{
    return "adverts_sent";
}
        
void MissionConditionAdvertsSent::Serialize( CFoundation::Serializer& _Serializer )
{
    _Serializer.Serialize( m_u32NumSentTarget );
    _Serializer.Serialize( m_u32NumSent );
}

void MissionConditionAdvertsSent::SetPlayer( PlayerPtr _spPlayer )
{
    PlayerPtr spOldPlayer = GetPlayer();
    if ( spOldPlayer != NULL )
    {
        // Remove delegate
        spOldPlayer->GetScheduling()->RemoveDelegate( SCHEDULING_EVENT_ADVERT_SENT, m_spDelegateAdvertSent );
    }

    if ( _spPlayer != NULL )
    {
        // Register delegate
        _spPlayer->GetScheduling()->RegisterDelegate( SCHEDULING_EVENT_ADVERT_SENT, m_spDelegateAdvertSent );
    }

    MissionCondition::SetPlayer( _spPlayer );
}
        
void MissionConditionAdvertsSent::SetNumAdverts( Unsigned32 _u32NumAdverts )
{
    m_u32NumSentTarget = _u32NumAdverts;
}

void MissionConditionAdvertsSent::SetAdvertSuccess( bool _bSucceed )
{
}

bool MissionConditionAdvertsSent::IsForfilled() const
{
    return m_u32NumSent >= m_u32NumSentTarget;
}
        
CFoundation::String MissionConditionAdvertsSent::GetDescription() const
{
    return CFoundation::String::Build( "Sende %d erfolgreiche Werbeblöcke.", m_u32NumSentTarget );
}
        
void MissionConditionAdvertsSent::OnAdvertSent( AdvertPtr _spAdvert, bool _bSuccess )
{
    if ( _bSuccess )
    {
        ++m_u32NumSent;

        // Dispatch event
        if ( IsForfilled() )
        {
            DispatchEvent( MISSION_CONDITION_EVENT_FORFILLED, GetSharedPtr< MissionCondition >() );
        }
    }
}
