#include "cmadtv/Data/Spectators/Satellite.h"

using namespace CMadTV;

Satellite::Satellite( Unsigned32 _u32ID )
:   m_u32ID( _u32ID )
{
}
        
Unsigned32 Satellite::GetID() const
{
    return m_u32ID;
}
        
void Satellite::SetChannel( SatelliteChannelPtr _spChannel )
{
    m_spChannel = _spChannel;
}
        
SatelliteChannelPtr Satellite::GetChannel() const
{
    return m_spChannel;
}

void Satellite::SetText( const CFoundation::String& _strText )
{
    m_strText = _strText;
}

const CFoundation::String& Satellite::GetText() const
{
    return m_strText;
}
       
void Satellite::SetPrice( const CFoundation::Money& _Price )
{
    m_Price = _Price;
}

const CFoundation::Money& Satellite::GetPrice() const
{
    return m_Price;
}
        