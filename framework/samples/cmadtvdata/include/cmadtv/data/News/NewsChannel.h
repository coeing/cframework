#ifndef CMADTV_NEWS_CHANNEL_H
#define CMADTV_NEWS_CHANNEL_H

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Finance/Money.h"
#include "cfoundation/RTTI/rtti.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/Color.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    /// Represents a NewsChannel
    /**
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 22.07.2010
     */
    class NewsChannel
    {
    public:
        /// Constructor
        NewsChannel( Unsigned32 _u32ID = MAX_U32 );

        /// Serialization
        void                        Serialize( CFoundation::Serializer& _Serializer );

        /// Returns the ID
        Unsigned32                  GetID() const;

        /// Sets the name
        void                        SetName( const CFoundation::String& _strName );

        /// Returns the name
        const CFoundation::String&  GetName() const;

        /// Sets the price
        void                        SetPrice( const CFoundation::Money& _Price );

        /// Returns the price
        const CFoundation::Money&   GetPrice() const;

        /// Sets the color 
        void                        SetColor( const CFoundation::Color& _Color );

        /// Returns the color
        const CFoundation::Color&   GetColor() const;

    private:
        /// ID
        Unsigned32              m_u32ID;

        /// Name
        CFoundation::String     m_strName;

        /// Price
        CFoundation::Money      m_Price;

        /// Color
        CFoundation::Color      m_Color;

        TYPE_DATA
    };
}

#endif
