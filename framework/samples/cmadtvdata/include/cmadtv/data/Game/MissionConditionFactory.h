#ifndef CMADTV_MISSION_CONDITION_FACTORY_H
#define CMADTV_MISSION_CONDITION_FACTORY_H

#include "cfoundation/Utils/SharedPtr.h"
#include "cfoundation/Utils/Singleton.h"

namespace CFoundation
{
    class Serializer;
    class String;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( MissionCondition );

    class MissionConditionFactory : public CFoundation::Singleton< MissionConditionFactory >
    {
    public:
        /// Creates a condition of the passed type
        MissionConditionPtr         Create( const CFoundation::String& _strType );

        /// Serialization of a condition
        static void                 Serialize( CFoundation::Serializer& _Serializer, MissionConditionPtr& _spCondition );

    private:
    };
}

#endif
