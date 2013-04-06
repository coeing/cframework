#ifndef CMADTV_WORLD_EVENTS_H
#define CMADTV_WORLD_EVENTS_H

#include "cfoundation/Events/DelegateInvoker.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Game );

    enum WorldEvent
    {
        WORLD_EVENT_HOUR_CHANGED,
        WORLD_EVENT_HOUR_STAGE_CHANGED,
        WORLD_EVENT_DAY_STARTED,
        WORLD_EVENT_DAY_ENDED,
    };
    TYPEDEF_DELEGATE1( WorldEvent, GamePtr );

    enum WorldEventGame
    {
        WORLD_EVENT_GAME_STARTED,
        WORLD_EVENT_GAME_LOST,
        WORLD_EVENT_GAME_WON,
    };
    TYPEDEF_DELEGATE1( WorldEventGame, GamePtr );
}

#endif
