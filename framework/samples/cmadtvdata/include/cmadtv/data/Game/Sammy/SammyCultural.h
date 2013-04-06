#ifndef CMADTV_SAMMY_CULTURAL_H
#define CMADTV_SAMMY_CULTURAL_H

#include "cmadtv/Data/Game/Sammy/Sammy.h"

namespace CMadTV
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// SammyCultural
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class SammyCultural : public Sammy
    {
    public:
        /// Constructor
        SammyCultural();

        /// Called after the broadcasts have been started
        virtual void            OnBroadcastsStarted( const CFoundation::Date& _Date, Hour _eHour );
    };
}

#endif
