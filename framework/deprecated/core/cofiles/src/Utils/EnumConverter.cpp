#include "cofiles/Enum/EnumConverter.h"

#include "cofiles/Makros.h"

using namespace CoFiles;

void EnumConverterBase::Assign( Unsigned32 _u32Value, const String& _sValue )
{
    m_mapEnumString[ _u32Value ] = _sValue;
    m_mapStringEnum[ _sValue ] = _u32Value;
}
        
void EnumConverterBase::Validate( Unsigned32 _u32LastValue ) const
{
    // Check if all values are assigned
    CO_WARN_IF( m_mapEnumString.size() < _u32LastValue - 1, "EnumConverter: Not all values assigned." );
}

const String& EnumConverterBase::Convert( Unsigned32 _u32Value ) const
{
    EnumStringMap::const_iterator it = m_mapEnumString.find( _u32Value );
    if ( it != m_mapEnumString.end() )
    {
        return it->second;
    }
    else
    {
        static String sEmpty = "";
        return sEmpty;
    }
}

Unsigned32 EnumConverterBase::Convert( const CoFiles::String& _sValue ) const
{
    StringEnumMap::const_iterator it = m_mapStringEnum.find( _sValue );
    if ( it != m_mapStringEnum.end() )
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}