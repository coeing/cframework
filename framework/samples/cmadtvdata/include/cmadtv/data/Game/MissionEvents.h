#ifndef CMADTV_MISSION_CONDITION_EVENT_H
#define CMADTV_MISSION_CONDITION_EVENT_H

#include "cfoundation/Events/Delegate.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Mission );
    INCLUDE_SHARED_CLASS( MissionCondition );

    enum MissionConditionEvent
    {
        MISSION_CONDITION_EVENT_NONE,
        MISSION_CONDITION_EVENT_FORFILLED,
        MISSION_CONDITION_EVENT_END
    };
    TYPEDEF_DELEGATE1( MissionConditionEvent, MissionConditionPtr );

    enum MissionEvent
    {
        MISSION_EVENT_NONE,
        MISSION_EVENT_SUCCESSFUL,
        MISSION_EVENT_FAILED,
        MISSION_EVENT_END
    };
    TYPEDEF_DELEGATE1( MissionEvent, MissionPtr );
}

#endif
