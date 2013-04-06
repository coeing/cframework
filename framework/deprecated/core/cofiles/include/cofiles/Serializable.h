#ifndef COFILES_SERIALIZABLE_H
#define COFILES_SERIALIZABLE_H

#include "cofiles/Serializer.h"

namespace CoFiles
{
	class Serializable
	{
	public:
		virtual void Save( CoFiles::Serializer &clSerializer ) const = 0;
		virtual void Load( const CoFiles::Serializer &clSerializer ) = 0;
	};
}

#endif
