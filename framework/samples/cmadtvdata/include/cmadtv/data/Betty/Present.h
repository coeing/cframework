#ifndef CMADTV_PRESENT_H
#define CMADTV_PRESENT_H

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
    /// Represents a present
    /**
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 04.10.2010
     */
    class Present
    {
    public:
        /// Constructor
        Present( Unsigned32 _u32ID = MAX_U32 );

        /// Serialization
        void                        Serialize( CFoundation::Serializer& _Serializer );

        /// Returns the ID
        Unsigned32                  GetID() const;

        /// Sets the name
        void                        SetName( const CFoundation::String& _strName );

        /// Returns the name
        const CFoundation::String&  GetName() const;

        /// Sets the image
        void                        SetImage( const CFoundation::String& _strTexture );

        /// Returns the image
        const CFoundation::String&  GetImage() const;

        /// Sets the price
        void                        SetPrice( const CFoundation::Money& _Price );

        /// Returns the price
        const CFoundation::Money&   GetPrice() const;

    private:
        /// ID
        Unsigned32              m_u32ID;

        /// Name
        CFoundation::String     m_strName;

        /// Image
        CFoundation::String     m_strImage;

        /// Price
        CFoundation::Money      m_Price;

        TYPE_DATA
    };
}

#endif
