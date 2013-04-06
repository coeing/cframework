#ifndef COFILES_TIME_H
#define COFILES_TIME_H

#include "cofiles/Serializable.h"
#include "cofiles/String.h"
#include "cofiles/Types.h"

namespace CoFiles
{
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

		//! Returns the Time in hours, minutes and seconds
		void GetTime( Unsigned8 &u8Hour, Unsigned8 &u8Min, Unsigned8 &u8Sec ) const;

		//! Assigns to rString the time in the form "hh:mm:ss"
		void ToString( String& rString ) const;
		
		//! Returns the time in the form "hh:mm:ss"
		const String ToString() const;

		static Unsigned64 GetTimeMs();
		
		virtual void Save( CoFiles::Serializer &clSerializer ) const;
		virtual void Load( const CoFiles::Serializer &clSerializer );

	private:
		void SetTime( Integer64 i64Hour, Integer64 i64Min, Integer64 i64Sec );

		Unsigned64 m_u64Secs;
	};
};

#endif
