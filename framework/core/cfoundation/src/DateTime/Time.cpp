#include "cfoundation/DateTime/Time.h"

#include <stdio.h>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#include <sys/timeb.h>
#endif

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Text/String.h"

using namespace CFoundation;

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
        
void Time::Serialize( CFoundation::Serializer& _Serializer )
{
	_Serializer.Serialize( m_u64Secs );
}

void Time::Save( Serializer& _Serializer ) const
{
	_Serializer.Add( m_u64Secs );
}

void Time::Load( const Serializer& _Serializer )
{
	m_u64Secs = _Serializer.GetU64();
}

void Time::AddSeconds( Integer64 _i64Secs )
{
    CF_ASSERT( _i64Secs >= 0 || _i64Secs < 0 && m_u64Secs >= static_cast< Unsigned64 >( -_i64Secs ), "Time::AddSeconds: Time underflow" );
	m_u64Secs = ( m_u64Secs + _i64Secs )/* % 86400*/;
}

void Time::AddMinutes( Integer64 _i64Mins )
{
    AddSeconds( _i64Mins * 60 );
}

void Time::AddHours( Integer64 _i64Hours )
{
    AddSeconds( _i64Hours * 3600 );
}

Unsigned8 Time::GetHour() const
{
    return static_cast< Unsigned8 >( m_u64Secs / 3600 );
}

Unsigned8 Time::GetMinute() const
{
    Unsigned8 u8Hours = GetHour();
    return static_cast< Unsigned8 >( m_u64Secs / 60 - u8Hours * 60 );
}
       
Unsigned8 Time::GetSecond() const
{
    return static_cast< Unsigned8 >( m_u64Secs % 60 );
}

Unsigned64 Time::GetTime() const
{
    return m_u64Secs;
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
    
Integer64 Time::operator-( const Time& _rhs ) const
{
    return m_u64Secs - _rhs.m_u64Secs;
}
		
bool Time::operator==( const Time& _rhs ) const
{
    return m_u64Secs == _rhs.m_u64Secs;
}

bool Time::operator!=( const Time& _rhs ) const
{
    return m_u64Secs != _rhs.m_u64Secs;
}

bool Time::operator>( const Time& _rhs ) const
{
    return m_u64Secs > _rhs.m_u64Secs;
}

bool Time::operator<( const Time& _rhs ) const
{
    return m_u64Secs < _rhs.m_u64Secs;
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
