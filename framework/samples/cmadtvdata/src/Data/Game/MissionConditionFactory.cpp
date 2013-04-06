#include "cmadtv/Data/Game/MissionConditionFactory.h"

#include "cfoundation/Text/String.h"
#include "cfoundation/Serialization/Serializer.h"

#include "cmadtv/Data/Game/Conditions/MissionConditionAdvertsSent.h"

using namespace CMadTV;
        
MissionConditionPtr MissionConditionFactory::Create( const CFoundation::String& _strType )
{
    MissionConditionPtr spCondition;
    if ( _strType == "adverts_sent" )
    {
        spCondition = MissionConditionPtr( new MissionConditionAdvertsSent() );
    }
    return spCondition;
}
        
void MissionConditionFactory::Serialize( CFoundation::Serializer& _Serializer, MissionConditionPtr& _spCondition )
{
    // Serialize condition type
    CFoundation::String strType = _spCondition != NULL ? _spCondition->GetType() : "";
    _Serializer.Serialize( strType );

    // If loading create condition
    if ( _Serializer.IsLoading() )
    {
        _spCondition = GetInstance().Create( strType );
    }

    // Serialize condition
    if ( _spCondition != NULL )
    {
        _spCondition->Serialize( _Serializer );
    }
}
