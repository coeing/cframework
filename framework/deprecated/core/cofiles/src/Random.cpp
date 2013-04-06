#include "cofiles/Random.h"

#include <stdlib.h>

#include "cofiles/Makros.h"

using namespace CoFiles;

Random::Random()
{
}

Unsigned8 Random::GetU8( Unsigned8 u8MaxBorder )
{
	return static_cast<Unsigned8>( GetU32() % u8MaxBorder );
}

Unsigned32 Random::GetU32( Unsigned32 u32MaxBorder )
{
	Unsigned32 u32Value = rand();
	//CoAssert( err == 0, L"The rand_s function failed!\n" );

	return u32Value % u32MaxBorder;
}
