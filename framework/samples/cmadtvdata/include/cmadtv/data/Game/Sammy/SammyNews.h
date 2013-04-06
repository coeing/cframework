#ifndef CMADTV_SAMMY_NEWS_H
#define CMADTV_SAMMY_NEWS_H

#include "cmadtv/Data/Game/Sammy/Sammy.h"

namespace CMadTV
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// SammyNews
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class SammyNews : public Sammy
    {
    public:
        /// Constructor
        SammyNews();

        /// Called after the news have been started
        virtual void            OnNewsStarted( const CFoundation::Date& _Date, Hour _eHour );
    };
}

#endif
