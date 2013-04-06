#ifndef CMADTV_MISSION_CONDITION_RATING_H
#define CMADTV_MISSION_CONDITION_RATING_H

#include "cmadtv/Data/Game/MissionCondition.h"

namespace CMadTV
{
    class MissionConditionRating : public MissionCondition
    {
    public:
        /// Constructor
        MissionConditionRating();

        /// Indicates if the condition is forfilled
        virtual bool            IsForfilled() const;

    private:
    };
}

#endif
