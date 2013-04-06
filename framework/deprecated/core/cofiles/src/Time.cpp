#include "cofiles/Time.h"
#include <stdio.h>
#include <time.h>

#ifdef WIN32
#include <sys/timeb.h>
#endif

#include "cofiles/Globals.h"

using namespace CoFiles;

Time::Time()
:	m_u64Secs( 0 )
{
	// initialize with actual time
	tm timeinfo;
	time_t rawtime = time( 0 );
#ifdef WIN32
	localtime_s( &timeinfo, &rawtime );
#else
	timeinfo = *localtime( &rawtime );
#endif

	SetTime( timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec );
}

Time::Time( Integer64 i64Sec )
:	m_u64Secs( 0 )
{
	SetTime( 0, 0, i64Sec );
}

Time::Time( Integer64 i64Hour, Integer64 i64Min, Integer64 i64Sec )
:	m_u64Secs( 0 )
{
	SetTime( i64Hour, i64Min, i64Sec );
}

void Time::AddSeconds( Integer64 i64Secs )
{
	m_u64Secs = ( m_u64Secs + i64Secs ) % 86400;
}

void Time::AddMinutes( Integer64 i64Mins )
{
	m_u64Secs = ( m_u64Secs + i64Mins * 60 ) % 86400;
}

void Time::AddHours( Integer64 i64Hours )
{
	m_u64Secs = ( m_u64Secs + i64Hours * 3600 ) % 86400;
}

void Time::GetTime( Unsigned8 &u8Hour, Unsigned8 &u8Min, Unsigned8 &u8Sec ) const
{
	u8Hour = static_cast<Unsigned8>( m_u64Secs / 3600 );
	u8Min  = static_cast<Unsigned8>( m_u64Secs / 60 - u8Hour * 60 );
	u8Sec  = static_cast<Unsigned8>( m_u64Secs % 60 );
}

void Time::ToString( String &rString ) const
{
	Unsigned8 u8Hour;
	Unsigned8 u8Min;
	Unsigned8 u8Sec;
	GetTime( u8Hour, u8Min, u8Sec );
	rString.Format( "%02hu:%02hu:%02hu", u8Hour, u8Min, u8Sec );
}

const String Time::ToString() const
{
	String sTime;
	ToString( sTime );

	return sTime;
}

void Time::SetTime( Integer64 i64Hour, Integer64 i64Min, Integer64 i64Sec )
{
	i64Min += static_cast<Integer64>( i64Sec / 60 );
	i64Hour += static_cast<Integer64>( i64Min / 60 );
	Integer64 i64TmpSecs = (i64Hour%24)*3600 + (i64Min%60)*60 + (i64Sec%60);

	if (i64TmpSecs < 0)
	{
		m_u64Secs = static_cast<Unsigned64>( i64TmpSecs + 86400 );
	}
	else
	{
		m_u64Secs = i64TmpSecs;
	}
}

Unsigned64 Time::GetTimeMs()
{
	LONGLONG Frequency = 0, LastTime = 0;
	double TimeScale = 0.0;
	if( QueryPerformanceFrequency( (LARGE_INTEGER*) &Frequency ) )
	{
		TimeScale = 1000.0/Frequency;
		QueryPerformanceCounter( (LARGE_INTEGER*) &LastTime);
	}
	/*else
	{
		LastTime = timeGetTime();
		TimeScale = 1;
	}*/

	return static_cast<Unsigned64>( LastTime * TimeScale );
}

void Time::Save( CoFiles::Serializer &clSerializer ) const
{
	clSerializer.AddU64( m_u64Secs );
}

void Time::Load( const CoFiles::Serializer &clSerializer )
{
	m_u64Secs = clSerializer.GetU64();
}
