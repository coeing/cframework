#ifndef COFILES_SYSTEM_H
#define COFILES_SYSTEM_H

#include "Singleton.h"

namespace CoFiles
{
	class String;

	class System : public Singleton<System>
	{
	public:
		static String ExtensionFromFilename( const String &sFilename );

		String GetPath( const String& sFilename );
	};
}

#endif
