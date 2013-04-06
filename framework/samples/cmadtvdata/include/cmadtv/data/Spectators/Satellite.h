#ifndef CMADTV_SATELLITE_H
#define CMADTV_SATELLITE_H

#include "cfoundation/Finance/Money.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( SatelliteChannel );

    /// Represents a Satellite
    /**
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 24.07.2010
     */
    class Satellite
    {
    public:
        /// Constructor
        Satellite( Unsigned32 _u32ID );

        /// Returns the ID
        Unsigned32                  GetID() const;

        /// Sets the channel
        void                        SetChannel( SatelliteChannelPtr _spChannel );

        /// Returns the channel
        SatelliteChannelPtr              GetChannel() const;

        /// Sets the text
        void                        SetText( const CFoundation::String& _strText );

        /// Returns the text
        const CFoundation::String&  GetText() const;

        /// Sets the price
        void                        SetPrice( const CFoundation::Money& _Price );

        /// Returns the price
        const CFoundation::Money&   GetPrice() const;

    private:
        /// ID
        Unsigned32              m_u32ID;

        /// Channel
        SatelliteChannelPtr          m_spChannel;

        /// Text
        CFoundation::String     m_strText;

        /// Price
        CFoundation::Money      m_Price;
    };
}

#endif
