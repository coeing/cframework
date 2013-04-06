#include "cmadtv/Data/Game/Mission.h"

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Text/String.h"

#include "cmadtv/Data/Game/MissionCondition.h"
#include "cmadtv/Data/Game/MissionConditionFactory.h"

using namespace CMadTV;

Mission::Mission()
{
    // Create delegates
    m_spDelegateConditionForfilled = MissionConditionEventDelegate1Ptr( new MissionConditionEventDelegate1( this, &Mission::OnConditionForfilled ) );
}

void Mission::Serialize( CFoundation::Serializer& _Serializer )
{
    _Serializer.SerializeVector< MissionConditionFactory >( m_aspConditionsSucceed );
}
        
void Mission::Update( Float32 _f32TimeDifference )
{
}
        
void Mission::SetPlayer( PlayerPtr _spPlayer )
{
    Unsigned32 u32NumConditions = m_aspConditionsSucceed.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumConditions; ++u32Idx )
    {
        m_aspConditionsSucceed[ u32Idx ]->SetPlayer( _spPlayer );
    }
    
    u32NumConditions = m_aspConditionsFailed.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumConditions; ++u32Idx )
    {
        m_aspConditionsFailed[ u32Idx ]->SetPlayer( _spPlayer );
    }
}
        
void Mission::AddCondition( MissionConditionPtr _spCondition, bool _bSucceedOrFailed )
{
    if ( _bSucceedOrFailed )
    {
        m_aspConditionsSucceed.push_back( _spCondition );
    }
    else
    {
        m_aspConditionsFailed.push_back( _spCondition );
    }

    // Register delegate
    _spCondition->RegisterDelegate( MISSION_CONDITION_EVENT_FORFILLED, m_spDelegateConditionForfilled );
}

bool Mission::IsSuccessful() const
{
    // Check each condition
    Unsigned32 u32NumConditions = m_aspConditionsSucceed.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumConditions; ++u32Idx )
    {
        if ( !m_aspConditionsSucceed[ u32Idx ]->IsForfilled() )
        {
            return false;
        }
    }
    return true;
}

bool Mission::IsFailed() const
{
    // Check each condition
    Unsigned32 u32NumConditions = m_aspConditionsFailed.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumConditions; ++u32Idx )
    {
        if ( m_aspConditionsFailed[ u32Idx ]->IsForfilled() )
        {
            return true;
        }
    }
    return false;
}
       
CFoundation::String Mission::GetDescription() const
{
    CFoundation::String strDescription;

    Unsigned32 u32NumConditions = m_aspConditionsSucceed.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumConditions; ++u32Idx )
    {
        MissionConditionPtr spCondition = m_aspConditionsSucceed[ u32Idx ];
        strDescription += spCondition->GetDescription() + "\n";
    }

    return strDescription;
}

void Mission::OnConditionForfilled( MissionConditionPtr _spCondition )
{
    // Check if condition to succeed
    ConditionVec::iterator it = std::find( m_aspConditionsSucceed.begin(), m_aspConditionsSucceed.end(), _spCondition );
    if ( it != m_aspConditionsSucceed.end() )
    {
        // Check if mission successful
        if ( IsSuccessful() )
        {
            // Dispatch event
            DispatchEvent( MISSION_EVENT_SUCCESSFUL, GetSharedPtr() );
        }
    }

    // Check if condition to succeed
    it = std::find( m_aspConditionsFailed.begin(), m_aspConditionsFailed.end(), _spCondition );
    if ( it != m_aspConditionsFailed.end() )
    {
        // Dispatch event
        DispatchEvent( MISSION_EVENT_FAILED, GetSharedPtr() );
    }
}

