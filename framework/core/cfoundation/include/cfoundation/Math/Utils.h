#ifndef CFOUNDATION_UTILS_H
#define CFOUNDATION_UTILS_H

#include "cfoundation/Base/Types.h"

/// Clamps the passed number (excluding maximum border)
#define CLAMP( Number, Min, Max )   if ( Number < ( Min ) )         \
                                    {                           \
                                        Number = ( Min );           \
                                    }                           \
                                    else if ( Number > ( Max ) )    \
                                    {                           \
                                        Number = ( Max );           \
                                    }         

namespace CFoundation
{
    namespace Math
    {
        /// Rounds the passed value to the next integer value
        Integer32   Round( Float32 _f32Value );

        /// Returns the number of digits of the passed value
        Unsigned32  GetNumDigits( Integer64 _i64Value );
    }
}

#endif
