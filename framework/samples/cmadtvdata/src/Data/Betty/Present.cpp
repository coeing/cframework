#include "cmadtv/Data/Betty/Present.h"

#include "cfoundation/Serialization/Serializer.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Present
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF_INST_EASY( Present )

Present::Present( Unsigned32 _u32ID )
:   m_u32ID( _u32ID )
{
}
        
void Present::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.Serialize( m_u32ID );
    _Serializer.Serialize( m_strName );
    _Serializer.Serialize( m_strImage );
    m_Price.Serialize( _Serializer );
}
        
Unsigned32 Present::GetID() const
{
    return m_u32ID;
}

void Present::SetName( const CFoundation::String& _strName )
{
    m_strName = _strName;
}

const CFoundation::String& Present::GetName() const
{
    return m_strName;
}

void Present::SetImage( const CFoundation::String& _strImage )
{
    m_strImage = _strImage;
}

const CFoundation::String& Present::GetImage() const
{
    return m_strImage;
}
       
void Present::SetPrice( const CFoundation::Money& _Price )
{
    m_Price = _Price;
}

const CFoundation::Money& Present::GetPrice() const
{
    return m_Price;
}
        