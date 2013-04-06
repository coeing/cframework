#include "cfoundation/Math/Random.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Text/String.h"

using namespace CFoundation;

Unsigned8 Random::GetU8( Unsigned8 _u8MinBorder, Unsigned8 _u8MaxBorder )
{
	return static_cast< Unsigned8 >( GetU32( _u8MinBorder, _u8MaxBorder ) );
}

Unsigned32 Random::GetU32( Unsigned32 _u32MinBorder, Unsigned32 _u32MaxBorder )
{
    if ( _u32MaxBorder == 0 )
    {
        CF_WARN( "Random::GetU32: Max border has to be > 0, was 0." );
        return 0;
    }
    else if ( _u32MinBorder >= _u32MaxBorder )
    {
        CF_WARN( "Random::GetU32: Min border is bigger than max border." );
        return 0;
    }

	Unsigned32 u32Value = rand();
	return _u32MinBorder + u32Value % ( _u32MaxBorder - _u32MinBorder );
}
