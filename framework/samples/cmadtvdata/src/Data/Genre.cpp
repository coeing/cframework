#include "cmadtv/Data/Genre.h"

#include "cfoundation/Serialization/Serializer.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Genre
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF_INST_EASY( Genre )

Genre::Genre( Unsigned32 _u32ID )
:   m_u32ID( _u32ID )
{
    for ( Unsigned32 u32Idx = 0; u32Idx < HOUR_END; ++u32Idx )
    {
        m_au8Ratings[ u32Idx ] = 100;
    }
}
        
void Genre::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.Serialize( m_u32ID );
    _Serializer.Serialize( m_strName );
    _Serializer.SerializeArray< CFoundation::SerializeWrapper_Value >( m_au8Ratings, HOUR_END );
}
        
Unsigned32 Genre::GetID() const
{
    return m_u32ID;
}

void Genre::SetName( const CFoundation::String& _strName )
{
    m_strName = _strName;
}

const CFoundation::String& Genre::GetName() const
{
    return m_strName;
}
        
void Genre::SetRating( Hour _eSlot, Unsigned8 _u8Rating )
{
    m_au8Ratings[ _eSlot ] = _u8Rating;
}
        
Unsigned8 Genre::GetRating( Hour _eSlot ) const
{
    return m_au8Ratings[ _eSlot ];
}

