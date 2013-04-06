#include "cofiles/Logger.h"
#include <stdarg.h>

#include <iostream>
#include <ostream>
#include <fstream>
#include <stdio.h>

using namespace CoFiles;

Logger::Logger( const String& _sFileName )
:	m_sFileName( _sFileName ),
	m_bConsoleOutput( true )
{
}

void Logger::SetLogFile( const String& _sFileName )
{
	m_sFileName = _sFileName;
}

void Logger::EnableConsoleOutput( bool bEnable )
{
	m_bConsoleOutput = bEnable;
}

void Logger::Log( const String &rLog )
{
	if( m_bConsoleOutput )
	{
		std::wcout << rLog << std::endl;
	}

	if( m_sFileName != "" )
	{
		std::wofstream fOut( m_sFileName.wc_str(), m_sFileName.GetLength() );
		fOut << rLog << std::endl;
		fOut.close();
	}
}

void Logger::Log( const char* szLog, ... )
{
	va_list argList;
	va_start( argList, szLog );

	char tmp[ MAX_SIZE ];
	memset( tmp, 0, sizeof( tmp ) );
#ifdef WIN32
	vsprintf_s( tmp, szLog, argList );
#else
	vsprintf( tmp, szLog, argList );
#endif

	va_end( argList );

	Log( String( tmp ) );
}

void Logger::Log( const wchar_t* szLog, ... )
{
	va_list argList;
	va_start( argList, szLog );

	wchar_t tmp[ MAX_SIZE ];
	memset( tmp, 0, sizeof( tmp ) );
	vswprintf( tmp, MAX_SIZE, szLog, argList );

	va_end( argList );

	Log( String( tmp ) );
}
