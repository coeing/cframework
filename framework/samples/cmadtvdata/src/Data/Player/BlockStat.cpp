#include "cmadtv/Data/Player/BlockStat.h"

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Advert.h"
#include "cmadtv/Data/Broadcast.h"

using namespace CMadTV;

BlockStat::BlockStat()
:   m_f32NewsRating( 0.0f ),
    m_u32NewsSpectators( 0 ),
    m_f32BroadcastRating( 0.0f ),
    m_u32BroadcastSpectators( 0 ),
    m_u32BroadcastOrder( 0 ),
    m_bAdvertSuccessful( false ),
    m_u32AdvertOrder( 0 ),
    m_eImageEffect( IMAGE_EFFECT_NONE )
{
}

BlockStat::~BlockStat()
{
}
        
void BlockStat::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 3 );

    _Serializer.Serialize( m_f32NewsRating );
    _Serializer.Serialize( m_u32NewsSpectators );
    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_Context >( m_spBroadcast );
    _Serializer.Serialize( m_f32BroadcastRating );
    _Serializer.Serialize( m_u32BroadcastSpectators );
    if ( u32Version >= 3 )
    {
        _Serializer.Serialize( m_u32BroadcastOrder );
    }
    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_Context >( m_spAdvert );
    _Serializer.Serialize( m_bAdvertSuccessful );
    _Serializer.Serialize( m_u32AdvertOrder );
    if ( u32Version >= 2 )
    {
        _Serializer.SerializeEnum( m_eImageEffect );
    }
}
       
void BlockStat::SetNewsRating( Float32 _f32Rating )
{
    m_f32NewsRating = _f32Rating;
}

Float32 BlockStat::GetNewsRating() const
{
    return m_f32NewsRating;
}

void BlockStat::SetNewsSpectators( Unsigned32 _u32Spectators )
{
    m_u32NewsSpectators = _u32Spectators;
}

Unsigned32 BlockStat::GetNewsSpectators() const
{
    return m_u32NewsSpectators;
}
       
void BlockStat::SetBroadcast( BroadcastPtr _spBroadcast )
{
    m_spBroadcast = _spBroadcast;
}

BroadcastPtr BlockStat::GetBroadcast() const
{
    return m_spBroadcast;
}

void BlockStat::SetBroadcastRating( Float32 _f32Rating )
{
    m_f32BroadcastRating = _f32Rating;
}

Float32 BlockStat::GetBroadcastRating() const
{
    return m_f32BroadcastRating;
}

void BlockStat::SetBroadcastSpectators( Unsigned32 _u32Spectators )
{
    m_u32BroadcastSpectators = _u32Spectators;
}

Unsigned32 BlockStat::GetBroadcastSpectators() const
{
    return m_u32BroadcastSpectators;
}
        
void BlockStat::SetBroadcastOrder( Unsigned32 _u32Order )
{
    m_u32BroadcastOrder = _u32Order;
}

Unsigned32 BlockStat::GetBroadcastOrder() const
{
    return m_u32BroadcastOrder;
}
        
void BlockStat::SetAdvert( AdvertPtr _spAdvert )
{
    m_spAdvert = _spAdvert;
}

AdvertPtr BlockStat::GetAdvert() const
{
    return m_spAdvert;
}

void BlockStat::SetAdvertSuccessful( bool _bSuccess )
{
    m_bAdvertSuccessful = _bSuccess;
}

bool BlockStat::WasAdvertSuccessful() const
{
    return m_bAdvertSuccessful;
}
        
void BlockStat::SetAdvertOrder( Unsigned32 _u32Order )
{
    m_u32AdvertOrder = _u32Order;
}

Unsigned32 BlockStat::GetAdvertOrder() const
{
    return m_u32AdvertOrder;
}
        
void BlockStat::SetImageEffect( ImageEffect _eEffect )
{
    m_eImageEffect = _eEffect;
}

BlockStat::ImageEffect BlockStat::GetImageEffect() const
{
    return m_eImageEffect;
}
