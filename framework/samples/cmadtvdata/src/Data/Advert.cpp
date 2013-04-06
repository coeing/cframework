#include "cmadtv/Data/Advert.h"

#include "cfoundation/Serialization/Serializer.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Advert
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF_INST_EASY( Advert )

Advert::Advert( Unsigned32 _u32ID )
:   m_u32ID( _u32ID ),
    m_u32NumSpots( 2 ),
    m_u32NumDays( 2 ),
    m_u32MinSpectators( 0 )
{
}
        
void Advert::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.Serialize( m_u32ID );
    _Serializer.Serialize( m_strName );
    _Serializer.Serialize( m_strSlogan );
    m_clBonus.Serialize( _Serializer );
    m_clPenalty.Serialize( _Serializer );
    _Serializer.Serialize( m_u32MinSpectators );
    _Serializer.Serialize( m_u32NumSpots );
    _Serializer.Serialize( m_u32NumDays );
}
        
void Advert::SetID( Unsigned32 _u32ID )
{
    m_u32ID = _u32ID;
}

Unsigned32 Advert::GetID() const
{
    return m_u32ID;
}

void Advert::SetName( const CFoundation::String& _strName )
{
    m_strName = _strName;
}

const CFoundation::String& Advert::GetName() const
{
    return m_strName;
}

void Advert::SetSlogan( const CFoundation::String& _strSlogan )
{
    m_strSlogan = _strSlogan;
}

const CFoundation::String& Advert::GetSlogan() const
{
    return m_strSlogan;
}

void Advert::SetBonus( const CFoundation::Money& _clBonus )
{
    m_clBonus = _clBonus;
}

const CFoundation::Money& Advert::GetBonus() const
{
    return m_clBonus;
}

void Advert::SetPenalty( const CFoundation::Money& _clPenalty )
{
    m_clPenalty = _clPenalty;
}

const CFoundation::Money& Advert::GetPenalty() const
{
    return m_clPenalty;
}

void Advert::SetMinSpectators( Unsigned32 _u32MinSpectators )
{
    m_u32MinSpectators = _u32MinSpectators;
}

Unsigned32 Advert::GetMinSpectators() const
{
    return m_u32MinSpectators;
}

void Advert::SetNumSpots( Unsigned32 _u32NumSpots )
{
    m_u32NumSpots = _u32NumSpots;
}

Unsigned32 Advert::GetNumSpots() const
{
    return m_u32NumSpots;
}

void Advert::SetNumDays( Unsigned32 _u32NumDays )
{
    m_u32NumDays = _u32NumDays;
}

Unsigned32 Advert::GetNumDays() const
{
    return m_u32NumDays;
}
