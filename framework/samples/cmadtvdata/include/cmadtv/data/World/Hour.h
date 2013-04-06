#ifndef CMADTV_HOUR_H
#define CMADTV_HOUR_H

namespace CMadTV
{
    /// Available hours
    enum Hour
    {
        HOUR_NONE,

        HOUR_BEGIN,

        HOUR_18 = HOUR_BEGIN,
        HOUR_19,
        HOUR_20,
        HOUR_21,
        HOUR_22,
        HOUR_23,
        HOUR_24,

        HOUR_END
    };

    Hour& operator++( Hour& _eSlot ); 
    Hour& operator--( Hour& _eSlot );  

    /// Hour stages
    enum HourStage
    {
        HOUR_STAGE_NONE,

        HOUR_STAGE_NEWS,
        HOUR_STAGE_BROADCAST,
        HOUR_STAGE_ADVERT,

        HOUR_STAGE_END
    };
}

#endif
