#ifndef CMADTV_NEWS_H
#define CMADTV_NEWS_H

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Finance/Money.h"
#include "cfoundation/RTTI/rtti.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( NewsChannel );

    /// Represents a News
    /**
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 24.07.2010
     */
    class News
    {
    public:
        /// Constructor
        News( Unsigned32 _u32ID = MAX_U32 );

        /// Serialization
        void                        Serialize( CFoundation::Serializer& _Serializer );

        /// Returns the ID
        Unsigned32                  GetID() const;

        /// Sets the channel
        void                        SetChannel( NewsChannelPtr _spChannel );

        /// Returns the channel
        NewsChannelPtr              GetChannel() const;

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
        NewsChannelPtr          m_spChannel;

        /// Text
        CFoundation::String     m_strText;

        /// Price
        CFoundation::Money      m_Price;

        TYPE_DATA
    };
}

#endif
