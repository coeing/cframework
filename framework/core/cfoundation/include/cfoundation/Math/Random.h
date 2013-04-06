#ifndef CFOUNDATION_RANDOM_H
#define CFOUNDATION_RANDOM_H

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Base/Types.h"
#include "cfoundation/Utils/Singleton.h"

namespace CFoundation
{
	namespace Random
	{
        /// Returns a random unsigned char
		Unsigned8    GetU8( Unsigned8 _u8MinBorder, Unsigned8 _u8MaxBorder = MAX_U8 );
		
        /// Returns a random unsigned int
        Unsigned32   GetU32( Unsigned32 _u32MinBorder, Unsigned32 _u32MaxBorder = MAX_U32 );
	}
}

#endif
