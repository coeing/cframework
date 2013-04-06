#ifndef CFoundation_TIME_H
#define CFoundation_TIME_H

#include "cfoundation/Serialization/Serializable.h"
#include "cfoundation/Base/Types.h"

namespace CFoundation
{
    class String;

	//! A time class
	/*! Use this class to handle a time
	 *   - Hours go from 0 to 23
	 *   - Minutes go from 0 to 59
	 *   - Seconds go from 0 to 59
	 *   .
	 *  
	 *  \author	Christian Oeing
	 *
	 *  \date	22.10.06
	 *
	 */
	class Time : public Serializable
	{
	public:
		Time();
		Time( Integer64 i64Sec );	
		Time( Integer64 i64Hour, Integer64 i64Min, Integer64 i64Sec );

        /// Serialization
        virtual void        Serialize( CFoundation::Serializer& _Serializer );

        /// Saves the object data with the passed serializer
		virtual void        Save( Serializer& _clSerializer ) const;

        /// Loads the object data from the passed serializer
		virtual void        Load( const Serializer& _clSerializer );

        /// Sets the time
		void                SetTime( Integer64 _i64Hour, Integer64 _i64Min, Integer64 _i64Sec );		

		//! Adds the number of seconds
		/*! \param i64Secs	Seconds to add (can be positive or negative)
		 */
		void                AddSeconds( Integer64 i64Secs );

		//! Adds the number of minutes
		/*! \param i64Mins	Minutes to add (can be positive or negative)
		 */
		void                AddMinutes( Integer64 i64Mins );

		//! Adds the number of hours
		/*! \param i64Hours	Hours to add (can be positive or negative)
		 */
		void                AddHours( Integer64 i64Hours );

        /// Return the current hour
        Unsigned8           GetHour() const;

        /// Returns the current minute
        Unsigned8           GetMinute() const;

        /// Returns the current second
        Unsigned8           GetSecond() const;

        /// Returns amount of seconds
        Unsigned64          GetTime() const;

		//! Returns the Time in hours, minutes and seconds
		void                GetTime( Unsigned8 &u8Hour, Unsigned8 &u8Min, Unsigned8 &u8Sec ) const;

		//! Assigns to rString the time in the form "hh:mm:ss"
		void                ToString( String& rString ) const;
		
		//! Returns the time in the form "hh:mm:ss"
		const String        ToString() const;

        /// Subtracts the passed time from this time
        /// Returns the number of seconds between the two times
        Integer64           operator-( const Time& _rhs ) const;

		/// Indicates if the time is equal to the passed time
		bool                operator==( const Time& _rhs ) const;

		/// Indicates if the time is not equal to the passed time
		bool                operator!=( const Time& _rhs ) const;

		/// Indicates if the time is later than the passed time
		bool                operator>( const Time& _rhs ) const;

		/// Indicates if the time is earlier than the passed time
		bool                operator<( const Time& _rhs ) const;

		static Unsigned64   GetTimeMs();

	private:
        /// Seconds
		Unsigned64 m_u64Secs;
	};
};

#endif
