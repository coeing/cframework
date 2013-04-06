#include "cmadtv/Data/Spectators/Transmitter.h"

#include "cfoundation/Serialization/Serializer.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Transmitter
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF_INST_EASY( Transmitter )

Transmitter::Transmitter( Unsigned32 _u32ID )
:   m_u32ID( _u32ID ),
    m_eType( TYPE_NONE ),
    m_u32X( 0 ),
    m_u32Y( 0 )
{
}
        
void Transmitter::Init( Type _eType, Unsigned32 _u32X, Unsigned32 _u32Y )
{
    m_eType = _eType;
    m_u32X = _u32X;
    m_u32Y = _u32Y;
}
       
Unsigned32 Transmitter::GetID() const
{
    return m_u32ID;
}
        
void Transmitter::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.Serialize( m_u32ID );
    _Serializer.SerializeEnum( m_eType );
    _Serializer.Serialize( m_u32X );
    _Serializer.Serialize( m_u32Y );
}

Transmitter::Type Transmitter::GetType() const
{
    return m_eType;
}

Unsigned32 Transmitter::GetX() const
{
    return m_u32X;
}

Unsigned32 Transmitter::GetY() const
{
    return m_u32Y;
}
 