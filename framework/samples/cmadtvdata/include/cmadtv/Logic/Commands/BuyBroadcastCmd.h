#ifndef CMADTV_BUY_BROADCAST_CMD_H
#define CMADTV_BUY_BROADCAST_CMD_H

#include "cmadtv/Logic/Commands/PlayerCommand.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Broadcast );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BuyBroadcastCmd
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BuyBroadcastCmd : public PlayerCommand
    {
    public:
        /// Error codes
        enum Error
        {
            ERROR_NONE,
            ERROR_BROADCAST_NOT_AT_DEALER,
            ERROR_BAG_FULL,
            ERROR_INSUFFICIENT_MONEY,
        };

        /// Constructor
        BuyBroadcastCmd( PlayerContextPtr _spContext = PlayerContextPtr(), BroadcastPtr _spBroadcast = BroadcastPtr() );

        /// Sets the broadcast
        void                    SetBroadcast( BroadcastPtr _spBroadcast );

        /// Returns the broadcast
        BroadcastPtr            GetBroadcast() const;

        /// Serialization
        /// Should serialize the non-context dependent data
        virtual void            Serialize( CFoundation::Serializer& _Serializer );

        /// Verifies if the command can be executed without changing any data
        /// Returns 0 if everything is ok, otherwise a error code is returned
        virtual Unsigned32      Verify() const;

        /// Executes the command
        virtual void            Execute() const;

    protected:
        /// Called by base class when context changed
        virtual void            OnContextChanged();

    private:
        /// Broadcast ID    
        Unsigned32              m_u32BroadcastID;

        /// Broadcast
        BroadcastPtr            m_spBroadcast;

        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( BuyBroadcastCmd );
}

#endif
