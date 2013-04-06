#include "cmadtv/Data/Broadcast.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Debug/Makros.h"
#include "cfoundation/Base/Defines.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Genre.h"
#include "cmadtv/Data/World/Country.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Broadcast
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF_INST_EASY( Broadcast )

Broadcast::Broadcast( Unsigned32 _u32ID )
:   m_u32ID( _u32ID ),
    m_eCountry( COUNTRY_NONE ),
    m_u8Year( 83 ),
    m_u32NumBlocks( 2 ),
    m_u8Up2date( 100 )
{
}
        
void Broadcast::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.Serialize( m_u32ID );
    _Serializer.Serialize( m_strTitle );
    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_Context >( m_spGenre );
    _Serializer.Serialize( m_strDescription );
    _Serializer.SerializeEnum( m_eCountry );
    _Serializer.Serialize( m_u8Year );
    _Serializer.Serialize( m_u32NumBlocks );
    m_clPrice.Serialize( _Serializer );
    _Serializer.Serialize( m_u8Up2date );
}
        
void Broadcast::SetID( Unsigned32 _u32ID )
{
    m_u32ID = _u32ID;
}

Unsigned32 Broadcast::GetID() const
{
    return m_u32ID;
}

void Broadcast::SetTitle( const CFoundation::String& _strTitle )
{
    m_strTitle = _strTitle;
}

const CFoundation::String& Broadcast::GetTitle() const
{
    return m_strTitle;
}
        
void Broadcast::SetGenre( GenrePtr _spGenre )
{
    m_spGenre = _spGenre;
}

GenrePtr Broadcast::GetGenre() const
{
    return m_spGenre;
}

void Broadcast::SetDescription( const CFoundation::String& _strDescription )
{
    m_strDescription = _strDescription;
}

const CFoundation::String& Broadcast::GetDescription() const
{
    return m_strDescription;
}

void Broadcast::SetCountry( Country _eCountry )
{
    m_eCountry = _eCountry;
}

Country Broadcast::GetCountry() const
{
    return m_eCountry;
}

void Broadcast::SetYear( Unsigned16 _u16Year )
{
    CF_WARN_IF( _u16Year < 1900 || _u16Year >= 1900 + MAX_U8, "Invalid year." );
    m_u8Year = static_cast< Unsigned8 >( _u16Year - 1900 );
}

Unsigned16 Broadcast::GetYear() const
{
    return 1900 + m_u8Year;
}

void Broadcast::SetNumBlocks( Unsigned32 _u32NumBlocks )
{
    m_u32NumBlocks = _u32NumBlocks;
}

Unsigned32 Broadcast::GetNumBlocks() const
{
    return m_u32NumBlocks;
}

void Broadcast::SetPrice( const CFoundation::Money& _clPrice )
{
    m_clPrice = _clPrice;
}

const CFoundation::Money& Broadcast::GetPrice() const
{
    return m_clPrice;
}
        
CFoundation::Money Broadcast::GetCurrentPrice() const
{
    CFoundation::Money clPrice = m_clPrice * 0.5f;
    clPrice += m_clPrice * 0.5f * m_u8Up2date / 100.0f;
    return clPrice;
}
        
void Broadcast::SetUp2date( Unsigned8 _u8Up2date )
{
    m_u8Up2date = _u8Up2date;
}

Unsigned8 Broadcast::GetUp2date() const
{
    return m_u8Up2date;
}
