#ifndef CMADTV_MISSION_H
#define CMADTV_MISSION_H

#include <vector>

#include "cfoundation/Base/Types.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cmadtv/Data/Game/MissionEvents.h"

namespace CFoundation
{
    class Serializer;
    class String;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( MissionCondition );
    INCLUDE_SHARED_CLASS( Player );

    class Mission : public CFoundation::SharedPtrBase< Mission >, public CFoundation::Delegate1Invoker< MissionEvent, MissionPtr >
    {
    public:
        DELEGATE1_INVOKER( MissionEvent, MissionPtr );

        /// Constructor
        Mission();

        /// Serialization
        void                    Serialize( CFoundation::Serializer& _Serializer );

        /// Per frame update
        void                    Update( Float32 _f32TimeDifference );

        /// Sets the player
        void                    SetPlayer( PlayerPtr _spPlayer );

        /// Adds a condition
        void                    AddCondition( MissionConditionPtr _spCondition, bool _bSucceedOrFailed );

        /// Indicates if the mission was successful
        bool                    IsSuccessful() const;

        /// Indicates if the mission was failed
        bool                    IsFailed() const;

        /// Creates a description of this mission
        CFoundation::String     GetDescription() const;

    private:
        typedef std::vector< MissionConditionPtr > ConditionVec;

        /// Callback when condition was forfilled
        void                    OnConditionForfilled( MissionConditionPtr _spCondition );

        /// Delegates
        MissionConditionEventDelegate1Ptr   m_spDelegateConditionForfilled;

        /// Conditions to succeed the mission
        ConditionVec            m_aspConditionsSucceed;

        /// Conditions to fail the mission
        ConditionVec            m_aspConditionsFailed;
    };
}

#endif
