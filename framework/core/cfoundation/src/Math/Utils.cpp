#include "cfoundation/Math/Utils.h"

#include <math.h>

#include "cfoundation/Debug/Assert.h"

using namespace CFoundation;

Integer32 Math::Round( Float32 _f32Value )
{
    Unsigned8 u8Sign = _f32Value < 0.0f ? -1 : 1;
    return static_cast< Integer32 >( fabs( _f32Value ) + 0.5f ) * u8Sign;
}
        
Unsigned32 Math::GetNumDigits( Integer64 _i64Value )
{
    Unsigned32 u32NumDigits = 0;
    while ( _i64Value > 0 )
    {
        _i64Value /= 10;
        ++u32NumDigits;
    }
    return u32NumDigits;
}
