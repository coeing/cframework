#include "cmadtv/Data/World/Hour.h"

#include "cfoundation/Enum/EnumIncrement.h"

using namespace CMadTV;

Hour& CMadTV::operator++( Hour& _eSlot )
{
    return enum_increment< Hour >( _eSlot, HOUR_NONE, HOUR_END );
}

Hour& CMadTV::operator--( Hour& _eSlot )
{
    return enum_decrement< Hour >( _eSlot, HOUR_NONE, HOUR_END );
}  
