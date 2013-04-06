#include "cmadtv/Data/News/News.h"

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/News/NewsChannel.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// News
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF_INST_EASY( News )

News::News( Unsigned32 _u32ID )
:   m_u32ID( _u32ID )
{
}
        
void News::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.Serialize( m_u32ID );
    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_Context >( m_spChannel );
    _Serializer.Serialize( m_strText );
    m_Price.Serialize( _Serializer );
}
        
Unsigned32 News::GetID() const
{
    return m_u32ID;
}
        
void News::SetChannel( NewsChannelPtr _spChannel )
{
    m_spChannel = _spChannel;
}
        
NewsChannelPtr News::GetChannel() const
{
    return m_spChannel;
}

void News::SetText( const CFoundation::String& _strText )
{
    m_strText = _strText;
}

const CFoundation::String& News::GetText() const
{
    return m_strText;
}
       
void News::SetPrice( const CFoundation::Money& _Price )
{
    m_Price = _Price;
}

const CFoundation::Money& News::GetPrice() const
{
    return m_Price;
}
        