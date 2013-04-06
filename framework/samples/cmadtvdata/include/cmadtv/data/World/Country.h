#ifndef CMADTV_COUNTRY_H
#define CMADTV_COUNTRY_H

#include "cfoundation/Enum/EnumConverter.h"

namespace CMadTV
{
    /// Available countries
    enum Country
    {
        COUNTRY_NONE,

        COUNTRY_BEGIN,

        COUNTRY_US = COUNTRY_BEGIN,
        COUNTRY_DE,
        COUNTRY_FR,
        COUNTRY_GB,
        COUNTRY_IT,
        COUNTRY_CA,
        COUNTRY_ES,

        COUNTRY_END
    };

    Country& operator++( Country& _eCountry ); 
    Country& operator--( Country& _eCountry );  
}

template<>
void CFoundation::EnumConverter< CMadTV::Country >::Initialize();

#endif
