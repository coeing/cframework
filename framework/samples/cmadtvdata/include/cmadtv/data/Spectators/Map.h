#ifndef CMADTV_MAP_H
#define CMADTV_MAP_H

#include <set>

#include "cfoundation/Finance/Money.h"
#include "cfoundation/Math/Vector.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Transmitter );

    /// Represents a Map
    /**
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 24.07.2010
     */
    class Map
    {
    public:
        /// Constructor
        Map();

        /// Destructor
        ~Map();

        /// Serialization
        void                    Serialize( CFoundation::Serializer& _Serializer );

        /// Init from raw data
        void                    Init( Unsigned32 _u32Width, Unsigned32 _u32Height, Unsigned8* _au8Density );

        /// Deinitialization
        void                    Deinit();
        
        /// Returns the map width
        Unsigned32              GetWidth() const;
        
        /// Returns the map height
        Unsigned32              GetHeight() const;

        /// Sets the antenna radius
        void                    SetAntennaRadius( Unsigned32 _u32AntennaRadius );

        /// Returns the antenna radius
        Unsigned32              GetAntennaRadius() const;

        /// Indicates if the passed position is on the map
        bool                    IsValidPosition( const CFoundation::Vector2Di32& _vPosition ) const;

        /// Indicates if the passed position is on the map
        bool                    IsValidPosition( Integer32 _i32X, Integer32 _i32Y ) const;

        /// Computes the spectators under the passed circle
        Unsigned32              ComputeNumSpectators( Unsigned32 _u32X, Unsigned32 _u32Y ) const;

        /// Computes the spectators under the passed circle
        Unsigned32              ComputeNumSpectators( TransmitterPtr _spTransmitter ) const;

        /// Computes the spectators of the passed transmitters
        Unsigned32              ComputeNumSpectators( const std::vector< TransmitterPtr >& _aspTransmitters ) const;

        /// Computes the new spectators of the passed transmitter if the passed array of transmitters are already there
        /// _u32NumSpectators is filled with the original number of spectators if no other transmitter would overlap
        Unsigned32              ComputeNumNewSpectators( Unsigned32 _u32X, Unsigned32 _u32Y, const std::vector< TransmitterPtr >& _aspTransmitters, Unsigned32& _u32NumSpectators ) const;

        /// Computes the new spectators of the passed transmitter if the passed array of transmitters are already there
        /// _u32NumSpectators is filled with the original number of spectators if no other transmitter would overlap
        Unsigned32              ComputeNumNewSpectators( TransmitterPtr _spTransmitter, const std::vector< TransmitterPtr >& _aspTransmitters, Unsigned32& _u32NumSpectators ) const;

    private:
        /// Collects all pixels that are reached by the passed transmitter
        void                    CollectCoveredPixels( std::vector< Unsigned32 >& _au32Pixels, TransmitterPtr _spTransmitter, Unsigned32 _u32Radius ) const;

        /// Collects all pixels that are reached by the passed circle
        void                    CollectCoveredPixels( std::vector< Unsigned32 >& _au32Pixels, Unsigned32 _u32X, Unsigned32 _u32Y, Unsigned32 _u32Radius ) const;

        /// Collects all pixels that are reached by the passed area
        void                    CollectCoveredPixels( std::vector< Unsigned32 >& _au32Pixels, Unsigned32 _u32X1, Unsigned32 _u32X2 ) const;

        /// Computes the number of spectators for the passed pixels
        Unsigned32              ComputeNumSpectators( const std::vector< Unsigned32 >& _au32Pixels ) const;

        /// Packs the data
        /// The returned array has to be deleted by the caller
        Unsigned8*              Pack( Unsigned32& _u32PackedSize ) const;

        /// Unpacks the data
        void                    Unpack( Unsigned8* _au8PackedData, Unsigned32 _u32PackedSize, Unsigned32 _u32Width, Unsigned32 _u32Height );

        /// Width
        Unsigned32              m_u32Width;

        /// Height
        Unsigned32              m_u32Height;

        /// Population density
        Unsigned8*              m_au8Density;

        /// Antenna radius
        Unsigned32              m_u32AntennaRadius;
    };

    INCLUDE_SHARED_CLASS( Map );
}

#endif
