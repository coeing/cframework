#ifndef CFOUNDATION_DATETIME_H
#define CFOUNDATION_DATETIME_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/DateTime/Date.h"
#include "cfoundation/DateTime/Time.h"
#include "cfoundation/Serialization/Serializable.h"

namespace CFoundation
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

		/// Constructor
		DateTime(	Integer64 _i64Day, Integer64 _i64Month, Integer64 _i64Year, 
					Integer64 _i64Hour = 0, Integer64 _i64Min = 0, Integer64 _i64Sec = 0 );

		/// Constructor
		DateTime( const Date& _Date, const Time& _Time );

        /// Serialization
        virtual void    Serialize( CFoundation::Serializer& _Serializer );

        /// Saves the object data with the passed serializer
		virtual void    Save( Serializer& _clSerializer ) const;

        /// Loads the object data from the passed serializer
		virtual void    Load( const Serializer& _clSerializer );

        /// Sets the date and time
        void            SetDateTime( Integer64 _i64Day, Integer64 _i64Month, Integer64 _i64Year, 
					                 Integer64 _i64Hour, Integer64 _i64Min, Integer64 _i64Sec );

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

		//! Adds the number of seconds
		/*! \param i64Secs	Seconds to add (can be positive or negative)
		 */
		void            AddSeconds( Integer64 i64Secs );

		//! Adds the number of minutes
		/*! \param i64Mins	Minutes to add (can be positive or negative)
		 */
		void            AddMinutes( Integer64 i64Mins );

		//! Adds the number of hours
		/*! \param i64Hours	Hours to add (can be positive or negative)
		 */
		void            AddHours( Integer64 i64Hours );

        /// Returns the date
        Date&           GetDate();

        /// Returns the date
        const Date&     GetDate() const;

		/// Returns the Date in days, months and years
		void            GetDate( Unsigned8& _u8Day, Unsigned8& _u8Month, Integer64& _i64Year ) const;

        /// Sets the time
		void            SetTime( Integer64 _i64Hour, Integer64 _i64Min, Integer64 _i64Sec );	

        /// Returns the time
        Time&           GetTime();

        /// Returns the time
        const Time&     GetTime() const;

		//! Returns the Time in hours, minutes and seconds
		void            GetTime( Unsigned8& _u8Hour, Unsigned8& _u8Min, Unsigned8& _u8Sec ) const;

        /// Returns the current day
        Unsigned8       GetDay() const;

        /// Returns the current month
        Unsigned8       GetMonth() const;

        /// Returns the current year
        Integer64       GetYear() const;

        /// Return the current hour
        Unsigned8       GetHour() const;

        /// Returns the current minute
        Unsigned8       GetMinute() const;

		//! Assigns to rString the date and time in the form "dd.mm.yyyy, hh:mm:ss"
		void            ToString( String& rString ) const;

		//! Returns the date and time in the form "dd.mm.yyyy, hh:mm:ss"
		const String&   ToString() const;

		/// Indicates if the datetime is equal to the passed datetime
		bool            operator==( const DateTime& _rhs ) const;

		/// Indicates if the datetime is not equal to the passed datetime
		bool            operator!=( const DateTime& _rhs ) const;

		/// Indicates if the datetime is later than the passed datetime
		bool            operator>( const DateTime& _rhs ) const;

		/// Indicates if the datetime is later or equal than the passed datetime
		bool            operator>=( const DateTime& _rhs ) const;

		/// Indicates if the datetime is earlier than the passed datetime
		bool            operator<( const DateTime& _rhs ) const;

		/// Indicates if the datetime is earlier or equal than the passed datetime
		bool            operator<=( const DateTime& _rhs ) const;

	private:
		/// Date
		Date	        m_clDate;

		/// Time
		Time	        m_clTime;
	};
};

#endif
