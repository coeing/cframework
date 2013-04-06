#include "cfoundation/DateTime/Date.h"

#include <time.h>
#ifndef WIN32
#include <sys/time.h>
#endif

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Text/String.h"

using namespace CFoundation;

Date::Date()
{
	// initialize with actual date
	time_t rawtime = time( 0 );
#ifdef WIN32
	tm timeinfo;
	localtime_s( &timeinfo, &rawtime );
#else
	tm timeinfo = *localtime( &rawtime );
#endif
	SetDate( timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900 );
}

Date::Date( Integer64 i64Day, Integer64 i64Month, Integer64 i64Year )
{
	SetDate( i64Day, i64Month, i64Year );
}

const Date& Date::operator=( const Date &rhs )
{
	m_u64JulianDay = rhs.m_u64JulianDay;

	return *this;
}
        
void Date::Serialize( CFoundation::Serializer& _Serializer )
{
	_Serializer.Serialize( m_u64JulianDay );
}

void Date::Save( Serializer& _Serializer ) const
{
	_Serializer.Add( m_u64JulianDay );
}

void Date::Load( const Serializer& _Serializer )
{
	m_u64JulianDay = _Serializer.GetU64();
}

void Date::AddDays( Integer64 i64Days )
{
	m_u64JulianDay += i64Days;
}

void Date::AddMonths( Integer64 i64Months )
{
	Unsigned8 u8Day;
	Unsigned8 u8Month;
	Integer64 i64Year;

	GetDate( u8Day, u8Month, i64Year );
	SetDate( u8Day, ( u8Month + i64Months ), i64Year );
}

void Date::AddYears( Integer64 i64Years )
{
	Unsigned8 u8Day;
	Unsigned8 u8Month;
	Integer64 i64Year;

	GetDate( u8Day, u8Month, i64Year );
	SetDate( u8Day, u8Month, (i64Year + i64Years) );
}

void Date::GetDate( Unsigned8 &u8Day, Unsigned8 &u8Month, Integer64 &i64Year ) const
{
	Unsigned64 u64Z = m_u64JulianDay + 1;

	Unsigned64 u64A = 0;
	if (u64Z < 2299161)
	{
		u64A = u64Z;
	}
	else
	{
		Unsigned64 u64G = static_cast<Unsigned64>( (u64Z - 1867216.25) / 36524.25 );
		u64A = u64Z + 1 + u64G - static_cast<Unsigned64>( u64G >> 2 );
	}

	Unsigned64 u64B = u64A + 1524;
	Unsigned64 u64C = static_cast<Unsigned64>( (u64B - 122.1) / 365.25 );
	Unsigned64 u64D = static_cast<Unsigned64>( 365.25 * u64C );
	Unsigned8  u8E  = static_cast<Unsigned8>( (u64B - u64D) / 30.6001 );

	// calculate day
	u8Day = static_cast<Unsigned8>( u64B - u64D - static_cast<Unsigned64>( 30.6001 * u8E ) + 0.5 );

	// calculate month
	if (u8E < 14)
	{
		u8Month = u8E - 1;
	}
	else
	{
		u8Month = u8E - 13;
	}

	// calculate year
	if (u8Month > 2)
	{
		i64Year = static_cast<Integer64>( u64C - 4716 );
	}
	else
	{
		i64Year = static_cast<Integer64>( u64C - 4715 );
	}
}
        
Unsigned8 Date::GetDay() const
{
    Unsigned8 u8Day = 0;
    Unsigned8 u8Month = 0;
    Integer64 i64Year = 0;
    GetDate( u8Day, u8Month, i64Year );
    return u8Day;
}

Unsigned8 Date::GetMonth() const
{
    Unsigned8 u8Day = 0;
    Unsigned8 u8Month = 0;
    Integer64 i64Year = 0;
    GetDate( u8Day, u8Month, i64Year );
    return u8Month;
}

Integer64 Date::GetYear() const
{
    Unsigned8 u8Day = 0;
    Unsigned8 u8Month = 0;
    Integer64 i64Year = 0;
    GetDate( u8Day, u8Month, i64Year );
    return i64Year;
}

void Date::ToString( String& rString ) const
{
	Unsigned8 u8Day, u8Month;
	Integer64 i64Year;

	GetDate( u8Day, u8Month, i64Year );

	rString.Format( "%02hu.%02hu.%ld", u8Day, u8Month, i64Year );
}

const String& Date::ToString() const
{
	static String sTmp;
	ToString( sTmp );
	return sTmp;
}

void Date::SetDate( Integer64 i64Day, Integer64 i64Month, Integer64 i64Year )
{
	// handle january and february as the 13. and 14. month to have the february
	// with its 28 or 29 days at the end of the year
	if (i64Month <= 2)
	{
		--i64Year;
		i64Month += 12;
	}

	Integer64 i64A = static_cast<Integer64>( i64Year / 100 );				// caluclate number of years with 366 days
	Integer64 i64B = 2 - i64A + static_cast<Integer64>( i64A >> 2 );		// calculate diff between julian and gregorian calendar

	m_u64JulianDay =	static_cast<Unsigned64>( static_cast<Unsigned64>( 365.25 * (i64Year + 4716) ) +			// days for passed year
												 static_cast<Unsigned64>( 30.6001 * (i64Month + 1) ) - 122  +	// days for passed months (months go from 3 to 14 (s.a.))
												 i64Day + i64B - 1402.5 );
}

void Date::operator+=( Integer64 _i64Days )
{
	AddDays( _i64Days );
}

void Date::operator-=( Integer64 _i64Days )
{
	AddDays( -_i64Days );
}
		
bool Date::operator==( const Date& _rhs ) const
{
    return m_u64JulianDay == _rhs.m_u64JulianDay;
}

bool Date::operator!=( const Date& _rhs ) const
{
    return m_u64JulianDay != _rhs.m_u64JulianDay;
}

bool Date::operator>( const Date& _rhs ) const
{
    return m_u64JulianDay > _rhs.m_u64JulianDay;
}

bool Date::operator>=( const Date& _rhs ) const
{
    return m_u64JulianDay >= _rhs.m_u64JulianDay;
}

bool Date::operator<( const Date& _rhs ) const
{
    return m_u64JulianDay < _rhs.m_u64JulianDay;
}

bool Date::operator<=( const Date& _rhs ) const
{
    return m_u64JulianDay <= _rhs.m_u64JulianDay;
}
    
Integer64 Date::operator-( const Date& _rhs ) const
{
    return m_u64JulianDay - _rhs.m_u64JulianDay;
}

const Date CFoundation::operator+( const Date& _lhs, Integer64 _i64Days )
{
    Date clSum( _lhs );
    clSum += _i64Days;
    return clSum;
}

const Date CFoundation::operator-( const Date& _lhs, Integer64 _i64Days )
{
    Date clSum( _lhs );
    clSum -= _i64Days;
    return clSum;
}

