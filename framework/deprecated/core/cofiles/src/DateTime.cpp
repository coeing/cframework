#include "cofiles/DateTime.h"

using namespace CoFiles;

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
	if ( abs(i64Secs) > 86400 )
	{
		i64Days = static_cast<Integer64>( i64Secs / 86400 );
		m_clDate.AddDays( i64Days );
	}

	m_clTime.AddSeconds( i64Secs - i64Days * 86400 );
}

void DateTime::AddMinutes( Integer64 i64Mins )
{
	AddSeconds( i64Mins * 60 );
}

void DateTime::AddHours( Integer64 i64Hours )
{
	AddSeconds( i64Hours * 3600 );
}

void DateTime::Save( CoFiles::Serializer &clSerializer ) const
{
	m_clDate.Save( clSerializer );
	m_clTime.Save( clSerializer );
}

void DateTime::Load( const CoFiles::Serializer &clSerializer )
{
	m_clDate.Load( clSerializer );
	m_clTime.Load( clSerializer );
}
