#ifndef COFILES_DATETIME_H
#define COFILES_DATETIME_H

#include "cofiles/Types.h"
#include "cofiles/Date.h"
#include "cofiles/Time.h"

#include "cofiles/Serializable.h"

namespace CoFiles
{
	//! Holds the date and the time
	/*! DateTime is able to make a day change over when the
	 *  time passed midnight.
	 *
	 *	\author	Christian Oeing
	 *
	 *	\date	22.10.06
	 *
	 */
	class DateTime : public Serializable
	{
	public:
		//! Default constructor
		/*! Fills the object with the actual date and time
		 */
		DateTime();

		//! Constructor
		DateTime(	Integer64 i64Day, Integer64 i64Month, Integer64 i64Year, 
					Integer64 i64Hour = 0, Integer64 i64Min = 0, Integer64 i64Sec = 0 );

		//! Adds the number of days to the date
		/*! \param i64Days	Days to add (can be positive or negative)
		 */
		void AddDays( Integer64 i64Days );

		//! Adds the number of months to the date
		/*! \param i64Months	Months to add (can be positive or negative)
		 */
		void AddMonths( Integer64 i64Months );

		//! Adds the number of years to the date
		/*! \param i64Years	Years to add (can be positive or negative)
		 */
		void AddYears( Integer64 i64Years );

		//! Adds the number of seconds
		/*! \param i64Secs	Seconds to add (can be positive or negative)
		 */
		void AddSeconds( Integer64 i64Secs );

		//! Adds the number of minutes
		/*! \param i64Mins	Minutes to add (can be positive or negative)
		 */
		void AddMinutes( Integer64 i64Mins );

		//! Adds the number of hours
		/*! \param i64Hours	Hours to add (can be positive or negative)
		 */
		void AddHours( Integer64 i64Hours );

		//! Returns the Date in days, months and years
		void GetDate( Unsigned8 &u8Day, Unsigned8 &u8Month, Integer64 &i64Year ) const;

		//! Returns the Time in hours, minutes and seconds
		void GetTime( Unsigned8 &u8Hour, Unsigned8 &u8Min, Unsigned8 &u8Sec ) const;

		//! Assigns to rString the date and time in the form "dd.mm.yyyy, hh:mm:ss"
		void ToString( String& rString ) const;

		//! Returns the date and time in the form "dd.mm.yyyy, hh:mm:ss"
		const String& ToString() const;
		
		virtual void Save( CoFiles::Serializer &clSerializer ) const;
		virtual void Load( const CoFiles::Serializer &clSerializer );

	private:
		//! The date
		Date	m_clDate;
		//! The time
		Time	m_clTime;
	};
};

#endif
