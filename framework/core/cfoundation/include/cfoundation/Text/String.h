#ifndef CFOUNDATION_STRING_H
#define CFOUNDATION_STRING_H

#include <iostream>

#include "cfoundation/Base/Types.h"

namespace CFoundation
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
		String( const char _cChar );
		String( const wchar_t _wcChar );
		String( const char* _szString );
		String( const wchar_t* _szString );
		String( const String& _rhs );

		explicit String( Unsigned32 _u32Number );
		explicit String( Unsigned64 _u64Number );
		explicit String( Integer32 _i32Number );
		explicit String( Integer64 _i64Number );
		explicit String( Float32 _f32Number );
		~String();

        /// Returns the number of characters
		Unsigned32  GetLength() const;

        /// Returns if the string is empty (number of characters == 0)
        bool        IsEmpty() const;

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

        /// Converts all characters of the string to lower case
        void        ToLower();

        /// Converts all characters of the string to upper case
        void        ToUpper();

        /// Converts the string to an Integer8 value
		Integer8	ToInteger8() const;

        /// Converts the string to an Integer16 value
		Integer16	ToInteger16() const;

        /// Converts the string to an Integer32 value
		Integer32	ToInteger32() const;

        /// Converts the string to an Integer64 value
		Integer64	ToInteger64() const;

        /// Converts the string to an Unsigned8 value
		Unsigned8	ToUnsigned8() const;

        /// Converts the string to an Unsigned16 value
		Unsigned16	ToUnsigned16() const;

        /// Converts the string to an Unsigned32 value
		Unsigned32	ToUnsigned32() const;

        /// Converts the string to an Unsigned64 value
		Unsigned64	ToUnsigned64() const;

		Float16		ToFloat16() const;
		Float32		ToFloat32() const;

        //! Takes the string as a hex string and converts it to an integer
		Integer32	HexToInteger() const;

        //! Takes the string as a hex string and converts it to an integer
        /*! 
         *  \param  u32Digits   Filled with the number of digits in the hex string
         */
        Integer32   HexToInteger( Unsigned32& _u32Digits ) const;

        /// Converts the passed value to a hex string
        static String   ToHexString( Integer64 _i64Value );

        /// Returns the string as a hex string (string has to be a number)
		String		    GetHexString() const;

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
		void            Format( const wchar_t* format, ... );

		//! Replaces the function sprintf.
		/*! Everything you can do with sprintf you can do with
		 *  this method too.
		 *
		 *	\param format	The formatation String as you know it from sprintf
		 *	\param ...		The variables used in the format string
		 *
		 */
		void            Format( const char *format, ... );

		/*! Returns a substring from beginning till position
			u32To exclusive
		 */
		String          Head( Unsigned32 u32To ) const;

		/*! Returns a substring from position u32From till position
			u32To exclusive
		 */
		String          Middle( Unsigned32 u32From, Unsigned32 u32To ) const;

		/*! Returns a substring from position u32From till end
		 */
		String          Tail( Unsigned32 u32From ) const;

		//! Removes whitespace at head and tail of the string
		/*! Line breaks, Tabulators, etc. are handled as whitespace
		 *  too.
		 */
		void            Trim();

		//! Removes whitespace at head of the string
		/*! Line breaks, Tabulators, etc. are handled as whitespace
		 *  too.
		 */
		void            TrimHead();

		//! Removes whitespace at tail of the string
		/*! Line breaks, Tabulators, etc. are handled as whitespace
		 *  too.
		 */
		void            TrimTail();

		bool            Find( const String &sSubString ) const;

        /// Checks if the passed charater is whitespace
        static bool     IsWhitespace( wchar_t _ch );

        /// Indicates if the character on the passed position is whitespace
        bool            IsWhitespace( Unsigned32 _u32Position ) const;

        /// Makes a million string out of the passed number
        static String   MakeMillionString( Float32 _f32Number );

	private:
		static const Unsigned32 MAX_SIZE = 4096;

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

    std::ostream&   operator<<( std::ostream& rOut, const String& _strString );
    std::wostream&  operator<<( std::wostream& rOut, const String& _strString );
}

#endif
