#ifndef CMADTV_TRADE_TRANSMITTER_CMD_H
#define CMADTV_TRADE_TRANSMITTER_CMD_H

#include "cmadtv/Logic/Commands/PlayerCommand.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Transmitter );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// TradeTransmitterCmd
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class TradeTransmitterCmd : public PlayerCommand
    {
    public:
        /// Modes
        enum Mode
        {
            MODE_NONE,
            MODE_BUY,
            MODE_SELL,
        };

        /// Error codes
        enum Error
        {
            ERROR_NONE,
            ERROR_INVALID_TRANSMITTER,
            ERROR_INSUFFICIENT_MONEY,

            ERROR_CANT_SELL_LAST_TRANSMITTER,
        };

        /// Constructor
        TradeTransmitterCmd( PlayerContextPtr _spContext = PlayerContextPtr(), Mode _eMode = MODE_NONE, TransmitterPtr _spTransmitter = TransmitterPtr() );

        /// Sets the transmitter
        void                    SetTransmitter( TransmitterPtr _spTransmitter );

        /// Returns the transmitter
        TransmitterPtr          GetTransmitter() const;

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
        /// Mode
        Mode                    m_eMode;

        /// Transmitter
        TransmitterPtr          m_spTransmitter;

        /// Transmitter ID    
        Unsigned32              m_u32TransmitterID;

        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( TradeTransmitterCmd );
}

#endif
