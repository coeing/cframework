#ifndef COFILES_DATE_H
#define COFILES_DATE_H

#include "cofiles/Serializable.h"
#include "cofiles/String.h"
#include "cofiles/Types.h"

namespace CoFiles
{
	//! A date class
	/*! This class implements a date as an Julian Day,
	 *  that means that its counted as days since the 1st January 4713 BC
	 *  See more under http://en.wikipedia.org/wiki/Julian_day.
	 *  
	 *  \author	Christian Oeing
	 *
	 *  \date	20.10.06
	 *
	 */
	class Date : public Serializable
	{
	public:
		//! Creates the actual date
		Date();
		//! Creates a Date by the specification of the day, the month and the year
		Date( Integer64 i64Day, Integer64 i64Month, Integer64 i64Year );

		//! Adds the number of days to the date
		/*! \param i64Days	Days to add (can be positive or negative)
		 */
		void            AddDays( Integer64 i64Days );

		//! Adds the number of months to the date
		/*! \param i64Months	Months to add (can be positive or negative)
		 */
		void            AddMonths( Integer64 i64Months );

		//! Adds the number of years to the date
		/*! \param i64Years	Years to add (can be positive or negative)
		 */
		void            AddYears( Integer64 i64Years );

		//! Returns the Date in days, months and years
		void            GetDate( Unsigned8 &u8Day, Unsigned8 &u8Month, Integer64 &i64Year ) const;

		//! Assigns to rString the date in the form "dd.mm.yyyy"
		void            ToString( String& rString ) const;

		//! Returns the date in the form "dd.mm.yyyy"
		const String&   ToString() const;
		
		virtual void    Save( CoFiles::Serializer &clSerializer ) const;
		virtual void    Load( const CoFiles::Serializer &clSerializer );

		//! Assigns the Date rhs to this date
		const Date&     operator=( const Date &rhs );

		/// Adds the number of days to the date
		void            operator+=( Integer64 _i64Days );

		/// Subtracts the number of days from the date
		void            operator-=( Integer64 _i64Days );

		/// Indicates if the date is later than the passed date
		bool            operator>( const Date& _rhs ) const;

		/// Indicates if the date is earlier than the passed date
		bool            operator<( const Date& _rhs ) const;

        /// Subtracts the passed date from this date
        /// Returns the number of days between the two dates
        Integer64       operator-( const Date& _rhs ) const;

	private:
		void SetDate( Integer64 i64Day, Integer64 i64Month, Integer64 i64Year );

		Unsigned64	m_u64JulianDay;
	};

    /// Adds the the passed amount of days to the passed date
    const Date operator+( const Date& _lhs, Integer64 _i64Days );

    /// Subtracts the passed amount of days from the passed date
    const Date operator-( const Date& _lhs, Integer64 _i64Days );
};

#endif
