#ifndef CMADTV_TRANSMITTER_H
#define CMADTV_TRANSMITTER_H

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Finance/Money.h"
#include "cfoundation/RTTI/rtti.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CMadTV
{
    /// Represents a Transmitter
    /**
     *  A transmitter contains its x/y coordinates on the map and its
     *  type.
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 24.07.2010
     */
    class Transmitter
    {
    public:
        enum Type
        {
            TYPE_NONE,
            TYPE_ANTENNA,
            TYPE_SATELLITE,
            TYPE_END
        };

        /// Constructor
        Transmitter( Unsigned32 _u32ID = MAX_U32 );

        /// Initialization
        void                    Init( Type _eType, Unsigned32 _u32X, Unsigned32 _u32Y );

        /// Serialization
        void                    Serialize( CFoundation::Serializer& _Serializer );

        /// Returns the ID
        Unsigned32              GetID() const;

        /// Returns the type
        Type                    GetType() const;

        /// Returns the X coordinate
        Unsigned32              GetX() const;

        /// Returns the Y coordinate
        Unsigned32              GetY() const;

    private:
        /// ID
        Unsigned32              m_u32ID;

        /// Type
        Type                    m_eType;

        /// Coordinates
        Unsigned32              m_u32X;
        Unsigned32              m_u32Y;

        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( Transmitter );
}

#endif
