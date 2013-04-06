#ifndef CMADTV_SUBSCRIBE_NEWS_CHANNEL_CMD_H
#define CMADTV_SUBSCRIBE_NEWS_CHANNEL_CMD_H

#include "cmadtv/Logic/Commands/PlayerCommand.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( NewsChannel );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// SubscribeNewsChannelCmd
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class SubscribeNewsChannelCmd : public PlayerCommand
    {
    public:
        /// Error codes
        enum Error
        {
            ERROR_NONE,
            ERROR_ALREADY_SUBSCRIBED,
            ERROR_NOT_SUBSCRIBED,
            ERROR_INSUFFICIENT_MONEY,
        };

        /// Constructor
        SubscribeNewsChannelCmd( PlayerContextPtr _spContext = PlayerContextPtr(), NewsChannelPtr _spNewsChannel = NewsChannelPtr(), bool _bSubscribe = true );

        /// Serialization
        /// Should serialize the non-context dependent data
        virtual void            Serialize( CFoundation::Serializer& _Serializer );

        /// Verifies if the command can be executed without changing any data
        /// Returns 0 if everything is ok, otherwise a error code is returned
        virtual Unsigned32      Verify() const;

        /// Executes the command
        virtual void            Execute() const;

    private:
        /// News channel
        NewsChannelPtr          m_spChannel;

        /// Indicates if the channel should be subscribed or unsubscribed
        bool                    m_bSubscribe;

        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( SubscribeNewsChannelCmd );
}

#endif
