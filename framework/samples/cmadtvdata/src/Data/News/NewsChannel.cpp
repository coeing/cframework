#include "cmadtv/Data/News/NewsChannel.h"

#include "cfoundation/Serialization/Serializer.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// NewsChannel
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF_INST_EASY( NewsChannel )

NewsChannel::NewsChannel( Unsigned32 _u32ID )
:   m_u32ID( _u32ID )
{
}
        
void NewsChannel::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.Serialize( m_u32ID );
    _Serializer.Serialize( m_strName );
    m_Price.Serialize( _Serializer );
    m_Color.Serialize( _Serializer );
}
        
Unsigned32 NewsChannel::GetID() const
{
    return m_u32ID;
}

void NewsChannel::SetName( const CFoundation::String& _strName )
{
    m_strName = _strName;
}

const CFoundation::String& NewsChannel::GetName() const
{
    return m_strName;
}
       
void NewsChannel::SetPrice( const CFoundation::Money& _Price )
{
    m_Price = _Price;
}

const CFoundation::Money& NewsChannel::GetPrice() const
{
    return m_Price;
}
       
void NewsChannel::SetColor( const CFoundation::Color& _Color )
{
    m_Color = _Color;
}

const CFoundation::Color& NewsChannel::GetColor() const
{
    return m_Color;
}
      