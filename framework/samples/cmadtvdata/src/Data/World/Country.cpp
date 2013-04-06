#include "cmadtv/Data/World/Country.h"

#include "cfoundation/Enum/EnumIncrement.h"

using namespace CMadTV;

template<>
void CFoundation::EnumConverter< Country >::Initialize()
{
    Assign( COUNTRY_US, "USA" );
    Assign( COUNTRY_DE, "D" );
    Assign( COUNTRY_FR, "F" );
    Assign( COUNTRY_GB, "GB" );
    Assign( COUNTRY_IT, "IT" );
    Assign( COUNTRY_CA, "CAN" );
    Assign( COUNTRY_ES, "ESP" );
}

Country& CMadTV::operator++( Country& _eCountry )
{
    return enum_increment< Country >( _eCountry, COUNTRY_NONE, COUNTRY_END );
}

Country& CMadTV::operator--( Country& _eCountry )
{
    return enum_decrement< Country >( _eCountry, COUNTRY_NONE, COUNTRY_END );
}  
