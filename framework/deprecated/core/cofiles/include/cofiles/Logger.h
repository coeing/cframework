#ifndef COFILES_LOGGER_H
#define COFILES_LOGGER_H

#include "cofiles/String.h"

namespace CoFiles
{
	class Logger
	{
	public:
        //! Constructor
		Logger( const String& _sFileName );

		void SetLogFile( const String& _sFileName );
		void EnableConsoleOutput( bool bEnable );

		void Log( const String &rFileName );
		void Log( const char* szLog, ... );
		void Log( const wchar_t* szLog, ... );

	private:
		static const Unsigned32 MAX_SIZE = 1000;

		String		m_sFileName;
		bool		m_bConsoleOutput;
	};
}

#endif
