#include "cmadtv/Data/Spectators/Map.h"

#include <algorithm>

#include "cfoundation/Math/Utils.h"

#include "cmadtv/Data/Spectators/Transmitter.h"

using namespace CMadTV;

Map::Map()
:   m_u32Width( 0 ),
    m_u32Height( 0 ),
    m_au8Density( NULL ),
    m_u32AntennaRadius( 16 )
{
}

Map::~Map()
{
    Deinit();
}
        
void Map::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.Serialize( m_u32Width );
    _Serializer.Serialize( m_u32Height );

    // Store packed data
    Unsigned32 u32PackedSize = 0;
    Unsigned8* au8PackedData = NULL;
    if ( _Serializer.IsLoading() )
    {
        _Serializer.Serialize( u32PackedSize );
        au8PackedData = new Unsigned8[ u32PackedSize ];
    }
    else
    {
        au8PackedData = Pack( u32PackedSize );
        _Serializer.Serialize( u32PackedSize );
    }

    _Serializer.SerializeDynArray< CFoundation::SerializeWrapper_Value >( au8PackedData, u32PackedSize );

    if ( _Serializer.IsLoading() )
    {
        // Unpack
        Unpack( au8PackedData, u32PackedSize, m_u32Width, m_u32Height );
    }
        
    SAFE_DELETE_ARRAY( au8PackedData );
}
        
void Map::Init( Unsigned32 _u32Width, Unsigned32 _u32Height, Unsigned8* _au8Density )
{
    // Deinitialize old map
    Deinit();

    m_u32Width = _u32Width;
    m_u32Height = _u32Height;

    // Copy data
    m_au8Density = new Unsigned8[ m_u32Width * m_u32Height ];
    memcpy( m_au8Density, _au8Density, m_u32Width * m_u32Height );
}
         
void Map::Deinit()
{
    // Delete density map
    m_u32Width = 0;
    m_u32Height = 0;
    SAFE_DELETE_ARRAY( m_au8Density );
}
               
Unsigned32 Map::GetWidth() const
{
    return m_u32Width;
}
        
Unsigned32 Map::GetHeight() const
{
    return m_u32Height;
}
     
void Map::SetAntennaRadius( Unsigned32 _u32AntennaRadius )
{
    m_u32AntennaRadius = _u32AntennaRadius;
}

Unsigned32 Map::GetAntennaRadius() const
{
    return m_u32AntennaRadius;
}

bool Map::IsValidPosition( const CFoundation::Vector2Di32& _vPosition ) const
{
    return IsValidPosition( _vPosition.GetX(), _vPosition.GetY() );
}
        
bool Map::IsValidPosition( Integer32 _i32X, Integer32 _i32Y ) const
{
    return _i32X >= 0 &&
        static_cast< Unsigned32 >( _i32X ) < m_u32Width &&
        _i32Y >= 0 &&
        static_cast< Unsigned32 >( _i32Y ) < m_u32Height;
}
        
Unsigned32 Map::ComputeNumSpectators( Unsigned32 _u32X, Unsigned32 _u32Y ) const
{
    // Collect covered pixels
    std::vector< Unsigned32 > au32Pixels;
    CollectCoveredPixels( au32Pixels, _u32X, _u32Y, m_u32AntennaRadius );
    
    // Compute num spectators
    return ComputeNumSpectators( au32Pixels );
}
       
Unsigned32 Map::ComputeNumSpectators( TransmitterPtr _spTransmitter ) const
{
    // Collect covered pixels
    std::vector< Unsigned32 > au32Pixels;
    CollectCoveredPixels( au32Pixels, _spTransmitter, m_u32AntennaRadius );

    // Compute num spectators
    return ComputeNumSpectators( au32Pixels );
}
        
Unsigned32 Map::ComputeNumSpectators( const std::vector< TransmitterPtr >& _aspTransmitters ) const
{
    // Collect covered pixels
    std::vector< Unsigned32 > au32Pixels;
    Unsigned32 u32NumTransmitters = _aspTransmitters.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumTransmitters; ++u32Idx )
    {
        TransmitterPtr spTransmitter = _aspTransmitters[ u32Idx ];
        CollectCoveredPixels( au32Pixels, spTransmitter, m_u32AntennaRadius );
    }

    // Remove duplicates
    std::vector< Unsigned32 >::iterator it = std::unique( au32Pixels.begin(), au32Pixels.end() );
    au32Pixels.erase( it, au32Pixels.end() );

    // Compute num spectators
    return ComputeNumSpectators( au32Pixels );
}
        
Unsigned32 Map::ComputeNumNewSpectators( Unsigned32 _u32X, Unsigned32 _u32Y, const std::vector< TransmitterPtr >& _aspTransmitters, Unsigned32& _u32NumSpectators ) const
{
    // Collect already covered pixels
    std::vector< Unsigned32 > au32Pixels;
    Unsigned32 u32NumTransmitters = _aspTransmitters.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumTransmitters; ++u32Idx )
    {
        TransmitterPtr spTransmitter = _aspTransmitters[ u32Idx ];
        CollectCoveredPixels( au32Pixels, spTransmitter, m_u32AntennaRadius );
    }

    // Collect new pixels
    std::vector< Unsigned32 > au32NewPixels;
    CollectCoveredPixels( au32NewPixels, _u32X, _u32Y, m_u32AntennaRadius );

    // Remove already covered pixels
    std::vector< Unsigned32 > au32DiffPixels;
    std::sort( au32Pixels.begin(), au32Pixels.end() );
    std::sort( au32NewPixels.begin(), au32NewPixels.end() );
    std::set_difference( au32NewPixels.begin(), au32NewPixels.end(), au32Pixels.begin(), au32Pixels.end(), std::inserter( au32DiffPixels, au32DiffPixels.begin() ) );
    
    // Compute num new spectators
    _u32NumSpectators = ComputeNumSpectators( au32NewPixels );
    return ComputeNumSpectators( au32DiffPixels );
}
        
Unsigned32 Map::ComputeNumNewSpectators( TransmitterPtr _spTransmitter, const std::vector< TransmitterPtr >& _aspTransmitters, Unsigned32& _u32NumSpectators ) const
{
    // Collect already covered pixels
    std::vector< Unsigned32 > au32Pixels;
    Unsigned32 u32NumTransmitters = _aspTransmitters.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumTransmitters; ++u32Idx )
    {
        TransmitterPtr spTransmitter = _aspTransmitters[ u32Idx ];
        CollectCoveredPixels( au32Pixels, spTransmitter, m_u32AntennaRadius );
    }

    // Collect new pixels
    std::vector< Unsigned32 > au32NewPixels;
    CollectCoveredPixels( au32NewPixels, _spTransmitter, m_u32AntennaRadius );

    // Remove already covered pixels
    std::vector< Unsigned32 > au32DiffPixels;
    std::sort( au32Pixels.begin(), au32Pixels.end() );
    std::sort( au32NewPixels.begin(), au32NewPixels.end() );
    std::set_difference( au32NewPixels.begin(), au32NewPixels.end(), au32Pixels.begin(), au32Pixels.end(), std::inserter( au32DiffPixels, au32DiffPixels.begin() ) );
    
    // Compute num new spectators
    _u32NumSpectators = ComputeNumSpectators( au32NewPixels );
    return ComputeNumSpectators( au32DiffPixels );
}
              
void Map::CollectCoveredPixels( std::vector< Unsigned32 >& _au32Pixels, TransmitterPtr _spTransmitter, Unsigned32 _u32Radius ) const
{
    if ( _spTransmitter == NULL )
    {
        return;
    }

    // Collect covered pixels
    switch ( _spTransmitter->GetType() )
    {
    case Transmitter::TYPE_ANTENNA:
        {
            CollectCoveredPixels( _au32Pixels, _spTransmitter->GetX(), _spTransmitter->GetY(), _u32Radius );
        }
        break;
    case Transmitter::TYPE_SATELLITE:
        {
            CollectCoveredPixels( _au32Pixels, _spTransmitter->GetX(), _spTransmitter->GetY() );
        }
        break;
    }
}
        
void Map::CollectCoveredPixels( std::vector< Unsigned32 >& _au32Pixels, Unsigned32 _u32X, Unsigned32 _u32Y, Unsigned32 _u32Radius ) const
{    
    Integer32 i32Radius = _u32Radius;
    Integer32 i32SqRadius = i32Radius * i32Radius;

    for ( Integer32 i32Y = -i32Radius; i32Y < i32Radius; ++i32Y )
    {
        for ( Integer32 i32X = -i32Radius; i32X < i32Radius; ++i32X )
        {
            // Check if inside radius
            if ( i32X * i32X + i32Y * i32Y > i32SqRadius )
            {
                continue;
            }

            // Compute pixel coordinates
            Integer32 i32PixelX = _u32X + i32X;
            Integer32 i32PixelY = _u32Y + i32Y;
            
            // Check if inside map
            if ( !IsValidPosition( i32PixelX, i32PixelY ) )
            {
                continue;
            }

            // Add pixel
            Unsigned32 u32Pixel = i32PixelX + i32PixelY * m_u32Width;
            _au32Pixels.push_back( u32Pixel );
        }
    }
}
       
void Map::CollectCoveredPixels( std::vector< Unsigned32 >& _au32Pixels, Unsigned32 _u32X1, Unsigned32 _u32X2 ) const
{
    if ( _u32X1 >= _u32X2 )
    {
        return;
    }

    // Clamp coordinates
    CLAMP( _u32X1, 0, m_u32Width );
    CLAMP( _u32X2, 0, m_u32Width );

    // Reserve space
    _au32Pixels.reserve( ( _u32X2 - _u32X1 ) * m_u32Height );

    for ( Unsigned32 u32Y = 0; u32Y < m_u32Height; ++u32Y )
    {
        for ( Unsigned32 u32X = _u32X1; u32X < _u32X2; ++u32X )
        {          
            // Add pixel
            Unsigned32 u32Pixel = u32X + u32Y * m_u32Width;
            _au32Pixels.push_back( u32Pixel );
        }
    }
}
        
Unsigned32 Map::ComputeNumSpectators( const std::vector< Unsigned32 >& _au32Pixels ) const
{
    Unsigned32 u32NumSpectators = 0;

    for ( std::vector< Unsigned32 >::const_iterator it = _au32Pixels.begin(); it != _au32Pixels.end(); ++it )
    {
        Unsigned32 u32Pixel = *it;

        // Get density
        Unsigned8 u8Density = m_au8Density[ u32Pixel ];
        switch ( u8Density )
        {
        case 3:
            {
                u32NumSpectators += 64000;
            }
            break;
        case 2:
            {
                u32NumSpectators += 6400;
            }
            break;
        case 1:
            {
                u32NumSpectators += 640;
            }
            break;
        }
    }

    return u32NumSpectators;
}
       
Unsigned8* Map::Pack( Unsigned32& _u32PackedSize ) const
{
    Unsigned32 u32DataSize = m_u32Width * m_u32Height;
    if ( u32DataSize == 0 )
    {
        return NULL;
    }

    // Packed size can be maximum double the size of the real data 
    // (i.e. when each density pixel is different to the one next to it)
    Unsigned8* au8Packed = new Unsigned8[ u32DataSize * 2 ];
    Unsigned32 u32Offset = 0;
    Unsigned8 u8Density = m_au8Density[ 0 ];
    Unsigned8 u8Count = 0;
    Unsigned32 u32SumCount = 0;
    for ( Unsigned32 u32Idx = 0; u32Idx < u32DataSize; ++u32Idx )
    {
        if ( m_au8Density[ u32Idx ] != u8Density ||
             u8Count == 255 )
        {
            // Store 
            au8Packed[ u32Offset++ ] = u8Count;
            au8Packed[ u32Offset++ ] = u8Density;
            
            u32SumCount += u8Count;

            // Next run
            u8Count = 0;
            u8Density = m_au8Density[ u32Idx ];
        }

        // Increase counter
        ++u8Count;
    }

    // Store last run
    au8Packed[ u32Offset++ ] = u8Count;
    au8Packed[ u32Offset++ ] = u8Density;
    u32SumCount += u8Count;

    _u32PackedSize = u32Offset;
    return au8Packed;
}
        
void Map::Unpack( Unsigned8* _au8PackedData, Unsigned32 _u32PackedSize, Unsigned32 _u32Width, Unsigned32 _u32Height )
{
    // Deinit old data
    Deinit();

    if ( _au8PackedData == NULL ||
         _u32Width == 0 ||
         _u32Height == 0 )
    {
        return;
    }

    // Create density data
    m_u32Width = _u32Width;
    m_u32Height = _u32Height;
    m_au8Density = new Unsigned8[ m_u32Width * m_u32Height ];

    // Initialize data
    Unsigned32 u32Offset = 0;
    for ( Unsigned32 u32Idx = 0; u32Idx < _u32PackedSize; u32Idx += 2 )
    {
        const Unsigned8& u8Count = _au8PackedData[ u32Idx ];
        const Unsigned8& u8Density = _au8PackedData[ u32Idx + 1 ];
        memset( m_au8Density + u32Offset * sizeof( Unsigned8 ), u8Density, u8Count * sizeof( Unsigned8 ) );
        u32Offset += u8Count;
    }
}
