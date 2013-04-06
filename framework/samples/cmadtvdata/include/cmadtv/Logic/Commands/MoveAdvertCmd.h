#ifndef CMADTV_MOVE_ADVERT_CMD_H
#define CMADTV_MOVE_ADVERT_CMD_H

#include "cmadtv/Logic/Commands/PlayerCommand.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// MoveAdvertCmd
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class MoveAdvertCmd : public PlayerCommand
    {
    public:
        /// Error codes
        enum Error
        {
            ERROR_NONE,
            ERROR_INVALID_LOCATION,
            ERROR_ADVERT_NOT_AT_DEALER,
            ERROR_ADVERT_NOT_IN_BAG,
            ERROR_BAG_IS_FULL,
        };

        /// Locations
        enum Location
        {
            LOCATION_NONE,
            LOCATION_DEALER,
            LOCATION_BAG,
            LOCATION_CONTRACTS,
        };

        /// Constructor
        MoveAdvertCmd( PlayerContextPtr _spContext = PlayerContextPtr(), AdvertPtr _spAdvert = AdvertPtr(), Location _eFrom = LOCATION_NONE, Location _eTo = LOCATION_NONE );

        /// Sets the broadcast
        void                    SetAdvert( AdvertPtr _spAdvert );

        /// Returns the broadcast
        AdvertPtr            GetAdvert() const;

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
        /// Advert ID    
        Unsigned32              m_u32AdvertID;

        /// Advert
        AdvertPtr            m_spAdvert;

        /// From where
        Location                m_eLocationFrom;

        /// To where
        Location                m_eLocationTo;

        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( MoveAdvertCmd );
}

#endif
