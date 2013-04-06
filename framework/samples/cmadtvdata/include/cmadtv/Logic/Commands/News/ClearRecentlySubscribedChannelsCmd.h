#ifndef CMADTV_CLEAR_RECENTLY_SUBSCRIBED_CHANNELS_CMD_H
#define CMADTV_CLEAR_RECENTLY_SUBSCRIBED_CHANNELS_CMD_H

#include "cmadtv/Logic/Commands/PlayerCommand.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( NewsChannel );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ClearRecentlySubscribedChannelsCmd
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ClearRecentlySubscribedChannelsCmd : public PlayerCommand
    {
    public:
        /// Error codes
        enum Error
        {
            ERROR_NONE,
        };

        /// Constructor
        ClearRecentlySubscribedChannelsCmd( PlayerContextPtr _spContext = PlayerContextPtr() );

        /// Serialization
        /// Should serialize the non-context dependent data
        virtual void            Serialize( CFoundation::Serializer& _Serializer );

        /// Verifies if the command can be executed without changing any data
        /// Returns 0 if everything is ok, otherwise a error code is returned
        virtual Unsigned32      Verify() const;

        /// Executes the command
        virtual void            Execute() const;

    private:
        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( ClearRecentlySubscribedChannelsCmd );
}

#endif
