#ifndef CMADTV_BUY_TRANSMITTER_CMD_H
#define CMADTV_BUY_TRANSMITTER_CMD_H

#include "cfoundation/Math/Vector.h"

#include "cmadtv/Logic/Commands/PlayerCommand.h"

namespace CMadTV
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BuyAntennaCmd
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BuyAntennaCmd : public PlayerCommand
    {
    public:
        /// Error codes
        enum Error
        {
            ERROR_NONE,
            ERROR_INVALID_TRANSMITTER,
            ERROR_INSUFFICIENT_MONEY,
        };

        /// Constructor
        BuyAntennaCmd( PlayerContextPtr _spContext = PlayerContextPtr(), const CFoundation::Vector2Di& _vPosition = CFoundation::Vector2Di() );

        /// Serialization
        /// Should serialize the non-context dependent data
        virtual void            Serialize( CFoundation::Serializer& _Serializer );

        /// Verifies if the command can be executed without changing any data
        /// Returns 0 if everything is ok, otherwise a error code is returned
        virtual Unsigned32      Verify() const;

        /// Executes the command
        virtual void            Execute() const;

    private:
        /// Position
        CFoundation::Vector2Di  m_vPosition;

        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( BuyAntennaCmd );
}

#endif
