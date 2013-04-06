#ifndef COFILES_STRING_H
#define COFILES_STRING_H

#include <iostream>

#include "cofiles/Types.h"

namespace CoFiles
{
	//! Represents a string and should replace const char*
	/*! Implementation of a String. Should be used in every
	 *  situation when you want to use const char*. If there's
	 *  no possibility to use String, this class has a method
	 *  c_str() that gives you a const char*.
	 *
	 *  \author Christian Oeing
	 *
	 *  \date	20.10.06
	 *
	 */
	class String
	{
	public:
		String();
		String( const char cChar );
		String( const char* szString );
		String( const wchar_t* szString );
		String( const String& rhs );
		explicit String( Unsigned32 _u32Number );
		explicit String( Unsigned64 _u64Number );
		explicit String( Integer32 _i32Number );
		explicit String( Float32 _f32Number );
		~String();

		Unsigned32 GetLength() const;

		String& operator=( const String& rhs );
		String& operator=( const char* rhs );

		bool operator==( const String &rhs )	const;
		bool operator==( const char *rhs )		const;
		bool operator==( const wchar_t *rhs )		const;
		bool operator!=( const String &rhs )	const;
		bool operator!=( const char *rhs )		const;

		bool operator<( const String &rhs )		const;
		bool operator<( const char *rhs )		const;
		bool operator<( const wchar_t *rhs )	const;
		bool operator>( const String &rhs )		const;
		bool operator>( const char *rhs )		const;
		bool operator>( const wchar_t *rhs )	const;

		String& operator+=( const String &rhs );
		String& operator+=( const char *rhs );
		String& operator+=( const wchar_t *rhs );
		String& operator+=( const char &rhs );
		String& operator+=( const wchar_t &rhs );

		wchar_t operator[]( Unsigned32 u32Idx ) const;

		//! Returns a const wchar_t* with the content of this String
		const wchar_t* wc_str() const;
		//! Returns a const char* with the content of this String
		const char* c_str() const;

		Integer8	ToInteger8() const;
		Integer16	ToInteger16() const;
		Integer32	ToInteger32() const;

		Unsigned8	ToUnsigned8() const;
		Unsigned16	ToUnsigned16() const;
		Unsigned32	ToUnsigned32() const;

		Float16		ToFloat16() const;
		Float32		ToFloat32() const;

        //! Takes the string as a hex string and converts it to an integer
		Integer32	HexToInteger() const;

        //! Takes the string as a hex string and converts it to an integer
        /*! 
         *  \param  u32Digits   Filled with the number of digits in the hex string
         */
        Integer32   HexToInteger( Unsigned32& _u32Digits ) const;

		String		GetHexString() const;

        /// Builds a new string out of the passed format
        static const String&    Build( const wchar_t* _format, ... );

        /// Builds a new string out of the passed format
        static const String&    Build( const char* _format, ... );

		//! Replaces the function swprintf.
		/*! Everything you can do with swprintf you can do with
		 *  this method too.
		 *
		 *	\param format	The formatation string as you know it from swprintf
		 *	\param ...		The variables used in the format string
		 *
		 */
		void        Format( const wchar_t* format, ... );

		//! Replaces the function sprintf.
		/*! Everything you can do with sprintf you can do with
		 *  this method too.
		 *
		 *	\param format	The formatation String as you know it from sprintf
		 *	\param ...		The variables used in the format string
		 *
		 */
		void        Format( const char *format, ... );

		/*! Returns a substring from beginning till position
			u32To exclusive
		 */
		String Head( Unsigned32 u32To ) const;

		/*! Returns a substring from position u32From till position
			u32To exclusive
		 */
		String Middle( Unsigned32 u32From, Unsigned32 u32To ) const;

		/*! Returns a substring from position u32From till end
		 */
		String Tail( Unsigned32 u32From ) const;

		//! Removes whitespace at head and tail of the string
		/*! Line breaks, Tabulators, etc. are handled as whitespace
		 *  too.
		 */
		void Trim();

		//! Removes whitespace at head of the string
		/*! Line breaks, Tabulators, etc. are handled as whitespace
		 *  too.
		 */
		void TrimHead();

		//! Removes whitespace at tail of the string
		/*! Line breaks, Tabulators, etc. are handled as whitespace
		 *  too.
		 */
		void TrimTail();

		bool Find( const String &sSubString ) const;

        /// Checks if the passed charater is whitespace
        static bool IsWhitespace( wchar_t _ch );

        /// Indicates if the character on the passed position is whitespace
        bool        IsWhitespace( Unsigned32 _u32Position ) const;

	private:
		static const Unsigned32 MAX_SIZE = 1000;

		void AssignString( const char* szString );
		void AssignString( const wchar_t* szString );
		void Clear();

		wchar_t*	m_pString;
		char*		m_pCString;
	};

	const String operator+( const String &lhs, const String &rhs );
	const String operator+( const String &lhs, Integer32 rhs );
	const String operator+( const String &lhs, const char *rhs );
	const String operator+( const String &lhs, const wchar_t *rhs );
	const String operator+( const char lhs, const String &rhs );
	const String operator+( const char *lhs, const String &rhs );
	const String operator+( const wchar_t *lhs, const String &rhs );
	const String operator+( const String &lhs, const char &rhs );
	const String operator+( const String &lhs, const wchar_t &rhs );
}

std::ostream& operator<<( std::ostream& rOut, const CoFiles::String& sString );
std::wostream& operator<<( std::wostream& rOut, const CoFiles::String& sString );

#endif
