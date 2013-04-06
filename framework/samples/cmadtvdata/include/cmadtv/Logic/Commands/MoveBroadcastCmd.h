#ifndef CMADTV_MOVE_BROADCAST_CMD_H
#define CMADTV_MOVE_BROADCAST_CMD_H

#include "cmadtv/Logic/Commands/PlayerCommand.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Broadcast );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// MoveBroadcastCmd
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class MoveBroadcastCmd : public PlayerCommand
    {
    public:
        /// Error codes
        enum Error
        {
            ERROR_NONE,
            ERROR_NOT_IN_LOCATION,
            ERROR_RACK_SHELF_FULL,
            ERROR_BAG_FULL,
            ERROR_IS_PLANNED,
        };

        /// Locations
        enum Location
        {
            LOCATION_NONE,
            LOCATION_BAG,
            LOCATION_RACK,
        };

        /// Constructor
        MoveBroadcastCmd( PlayerContextPtr _spContext = PlayerContextPtr(), BroadcastPtr _spBroadcast = BroadcastPtr(), Location _eFrom = LOCATION_NONE, Location _eTo = LOCATION_NONE );

        /// Sets the broadcast
        void                    SetBroadcast( BroadcastPtr _spBroadcast );

        /// Returns the broadcast
        BroadcastPtr            GetBroadcast() const;

        /// Sets the locations from where to where the broadcast should be moved
        void                    SetLocations( Location _eFrom, Location _eTo );

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
        //Unsigned32              m_u32BroadcastID;

        /// Broadcast
        BroadcastPtr            m_spBroadcast;

        /// From where
        Location                m_eLocationFrom;

        /// To where
        Location                m_eLocationTo;

        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( MoveBroadcastCmd );
}

#endif
