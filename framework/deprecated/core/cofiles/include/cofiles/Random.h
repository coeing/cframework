#ifndef COFILES_RANDOM_H
#define COFILES_RANDOM_H

#include "cofiles/Types.h"
#include "cofiles/Defines.h"
#include "cofiles/Singleton.h"

namespace CoFiles
{
	class Random : public Singleton<Random>
	{
	public:
		Random();

		Unsigned8 GetU8( Unsigned8 u8MaxBorder = MAX_U8 );
		Unsigned32 GetU32( Unsigned32 u32MaxBorder = MAX_U32 );
	};
}

#endif
