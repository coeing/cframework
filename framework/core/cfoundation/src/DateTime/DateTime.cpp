#include "cfoundation/DateTime/DateTime.h"

#include <math.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace CFoundation;

DateTime::DateTime()
:	m_clDate(),
	m_clTime()
{
}

DateTime::DateTime( Integer64 i64Day, Integer64 i64Month, Integer64 i64Year, Integer64 i64Hour, Integer64 i64Min, Integer64 i64Sec )
:	m_clDate( i64Day, i64Month, i64Year ),
	m_clTime( i64Hour, i64Min, i64Sec )
{
}
		
DateTime::DateTime( const Date& _Date, const Time& _Time )
:	m_clDate( _Date ),
	m_clTime( _Time )
{
}
        
void DateTime::Serialize( CFoundation::Serializer& _Serializer )
{
	m_clDate.Serialize( _Serializer );
	m_clTime.Serialize( _Serializer );
}

void DateTime::Save( Serializer& _Serializer ) const
{
	m_clDate.Save( _Serializer );
	m_clTime.Save( _Serializer );
}

void DateTime::Load( const Serializer& _Serializer )
{
	m_clDate.Load( _Serializer );
	m_clTime.Load( _Serializer );
}
        
void DateTime::SetDateTime( Integer64 _i64Day, Integer64 _i64Month, Integer64 _i64Year, Integer64 _i64Hour, Integer64 _i64Min, Integer64 _i64Sec )
{
    m_clDate.SetDate( _i64Day, _i64Month, _i64Year );
    m_clTime.SetTime( _i64Hour, _i64Min, _i64Sec );
}

void DateTime::AddDays( Integer64 i64Days )
{
	m_clDate.AddDays( i64Days );
}

void DateTime::AddMonths( Integer64 i64Months )
{
	m_clDate.AddMonths( i64Months );
}

void DateTime::AddYears( Integer64 i64Years )
{
	m_clDate.AddYears( i64Years );
}

void DateTime::AddSeconds( Integer64 i64Secs )
{
	Integer64 i64Days = 0;
    if ( _abs64( i64Secs ) > 86400 )
	{
		i64Days = static_cast< Integer64 >( i64Secs / 86400 );
        i64Secs = i64Secs % 86400;
	}
    while ( i64Secs < 0 &&
            _abs64( i64Secs ) > m_clTime.GetTime() )
    {
        --i64Days;
        i64Secs += 86400;
    }

	m_clTime.AddSeconds( i64Secs );
    m_clDate.AddDays( i64Days );
}

void DateTime::AddMinutes( Integer64 i64Mins )
{
	AddSeconds( i64Mins * 60 );
}

void DateTime::AddHours( Integer64 i64Hours )
{
	AddSeconds( i64Hours * 3600 );
}
        
Date& DateTime::GetDate()
{
    return m_clDate;
}
        
const Date& DateTime::GetDate() const
{
    return m_clDate;
}

void DateTime::GetDate( Unsigned8& _u8Day, Unsigned8& _u8Month, Integer64& _i64Year ) const
{
    m_clDate.GetDate( _u8Day, _u8Month, _i64Year );
}

Time& DateTime::GetTime()
{
    return m_clTime;
}

const Time& DateTime::GetTime() const
{
    return m_clTime;
}
		
void DateTime::SetTime( Integer64 _i64Hour, Integer64 _i64Min, Integer64 _i64Sec )
{
    m_clTime.SetTime( _i64Hour, _i64Min, _i64Sec );
}

void DateTime::GetTime( Unsigned8& _u8Hour, Unsigned8& _u8Min, Unsigned8& _u8Sec ) const
{
    m_clTime.GetTime( _u8Hour, _u8Min, _u8Sec );
}
        
Unsigned8 DateTime::GetDay() const
{
    return m_clDate.GetDay();
}
        
Unsigned8 DateTime::GetMonth() const
{
    return m_clDate.GetMonth();
}

Integer64 DateTime::GetYear() const
{
    return m_clDate.GetYear();
}

Unsigned8 DateTime::GetHour() const
{
    return m_clTime.GetHour();
}

Unsigned8 DateTime::GetMinute() const
{
    return m_clTime.GetMinute();
}
		
bool DateTime::operator==( const DateTime& _rhs ) const
{
    return m_clDate == _rhs.m_clDate &&
        m_clTime == _rhs.m_clTime;
}

bool DateTime::operator!=( const DateTime& _rhs ) const
{
    return !( (*this) == _rhs );
}

bool DateTime::operator>( const DateTime& _rhs ) const
{
    return m_clDate > _rhs.m_clDate ||
        m_clDate == _rhs.m_clDate &&
        m_clTime > _rhs.GetTime();
}
		
bool DateTime::operator>=( const DateTime& _rhs ) const
{
    return (*this) > _rhs ||
           (*this) == _rhs;
}

bool DateTime::operator<( const DateTime& _rhs ) const
{
    return m_clDate < _rhs.m_clDate ||
        m_clDate == _rhs.m_clDate &&
        m_clTime < _rhs.GetTime();
}
		
bool DateTime::operator<=( const DateTime& _rhs ) const
{
    return (*this) < _rhs ||
           (*this) == _rhs;
}
