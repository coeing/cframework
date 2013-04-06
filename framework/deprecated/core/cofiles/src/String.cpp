#include "cofiles/String.h"

#include <wchar.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "cofiles/Defines.h"
#include "cofiles/Makros.h"

using namespace CoFiles;

String::String()
:	m_pString( NULL ),
	m_pCString( NULL )
{
	AssignString( "" );
}

String::String( const char cChar )
:	m_pString( NULL ),
	m_pCString( NULL )
{
	Format( "%c", cChar );
}

String::String( const char* szString )
:	m_pString( NULL ),
	m_pCString( NULL )
{
	AssignString( szString );
}

String::String( const wchar_t* szString )
:	m_pString( NULL ),
	m_pCString( NULL )
{
	AssignString( szString );
}

String::String( const CoFiles::String &rhs )
:	m_pString( NULL ),
	m_pCString( NULL )
{
	AssignString( rhs.m_pString );
}

String::String( Unsigned32 u32Number )
:	m_pString( NULL ),
	m_pCString( NULL )
{
	Format( "%u", u32Number );
}

String::String( Unsigned64 _u64Number )
:	m_pString( NULL ),
	m_pCString( NULL )
{
	Format( "%lu", _u64Number );
}

String::String( Integer32 i32Number )
:	m_pString( NULL ),
	m_pCString( NULL )
{
	Format( "%d", i32Number );
}

String::String( Float32 f32Number )
:	m_pString( NULL ),
	m_pCString( NULL )
{
	Format( "%f", f32Number );
}

String::~String()
{
	Clear();
}

Unsigned32 String::GetLength() const
{
	return wcslen( m_pString );
}

String& String::operator=( const CoFiles::String &rhs )
{
    if ( this != &rhs )
    {
        Clear();
        AssignString( rhs.m_pString );
    }

	return *this;
}

String& String::operator=( const char* rhs )
{
	Clear();
	AssignString( rhs );

	return *this;
}

bool String::operator ==( const String &rhs ) const
{
	return *this == rhs.wc_str();
}

bool String::operator ==( const char *rhs ) const
{
    return wcscmp( m_pString, String( rhs ).wc_str() ) == 0;
}

bool String::operator ==( const wchar_t *rhs ) const
{
	return wcscmp( m_pString, rhs ) == 0;
}

bool String::operator !=( const String &rhs ) const
{
	return !( *this == rhs.wc_str() );
}

bool String::operator !=( const char *rhs ) const
{
	return !( *this == rhs );
}

bool String::operator<( const String &rhs ) const
{
	return *this < rhs.wc_str();
}

bool String::operator<( const char *rhs ) const
{
	return wcscmp( wc_str(), String( rhs ).wc_str() ) < 0;
}

bool String::operator<( const wchar_t *rhs ) const
{
	return wcscmp( wc_str(), rhs ) < 0;
}


bool String::operator>( const String &rhs ) const
{
	return *this < rhs.wc_str();
}

bool String::operator>( const char *rhs ) const
{
	return wcscmp( wc_str(), String( rhs ).wc_str() ) > 0;
}

bool String::operator>( const wchar_t *rhs ) const
{
	return wcscmp( wc_str(), rhs ) > 0;
}

String& String::operator+=( const String &rhs )
{
	*this += rhs.m_pString;

	return *this;
}

String& String::operator+=( const char *rhs )
{
	*this += String( rhs ).wc_str();

	return *this;
}

String& String::operator+=( const wchar_t *rhs )
{
	Unsigned32 u32Length = wcslen( m_pString ) + wcslen( rhs );
	wchar_t *szTmp = new wchar_t[ u32Length + 1 ];
	swprintf( szTmp, u32Length + 1, L"%s%s", m_pString, rhs );
	Clear();
	AssignString( szTmp );
	delete [] szTmp;

	return *this;
}

String& String::operator+=( const char &rhs )
{
	*this += String( rhs ).wc_str();

	return *this;
}

String& String::operator+=( const wchar_t &rhs )
{
	Unsigned32 u32Length = wcslen( m_pString ) + 1;
	wchar_t *szTmp = new wchar_t[ u32Length + 1 ];
	swprintf( szTmp, u32Length + 1, L"%s%c", m_pString, rhs );
	Clear();
	AssignString( szTmp );
	delete [] szTmp;

	return *this;
}

const String CoFiles::operator+( const String &lhs, const String &rhs )
{
	return String( lhs ) += rhs;
}

const String CoFiles::operator+( const String &lhs, Integer32 rhs )
{
	return String( lhs ) += String( rhs );
}

const String CoFiles::operator+( const String &lhs, const char *rhs )
{
	return String( lhs ) += rhs;
}

const String CoFiles::operator+( const String &lhs, const wchar_t *rhs )
{
	return String( lhs ) += rhs;
}

const String CoFiles::operator+( const char lhs, const String &rhs )
{
	return String( lhs ) += rhs;
}

const String CoFiles::operator+( const char *lhs, const String &rhs )
{
	return String( lhs ) += rhs;
}

const String CoFiles::operator+( const wchar_t *lhs, const String &rhs )
{
	return String( lhs ) += rhs;
}

const String CoFiles::operator+( const String &lhs, const char &rhs )
{
	return String( lhs ) += rhs;
}

const String CoFiles::operator+( const String &lhs, const wchar_t &rhs )
{
	return String( lhs ) += rhs;
}

wchar_t String::operator[]( Unsigned32 u32Idx ) const
{
	CO_ASSERT( u32Idx >= 0 && u32Idx < GetLength(), L"String::operator[]: Index out of bounds" );
	return m_pString[ u32Idx ];
}

const wchar_t* String::wc_str() const
{
	return m_pString;
}

const char* String::c_str() const
{
	return m_pCString;
}

Integer8 String::ToInteger8() const
{
	Integer32 i32Value = ToInteger32();
	CO_ERROR_IF( i32Value < MIN_I8 || i32Value > MAX_I8, L"String::ToInteger8: Value out of range." );
	return static_cast< Integer8 >( i32Value );
}

Integer16 String::ToInteger16() const
{
	Integer32 i32Value = ToInteger32();
	CO_ERROR_IF( i32Value < MIN_I16 || i32Value > MAX_I16, L"String::ToInteger16: Value out of range." );
	return static_cast<Integer16>( i32Value );
}

Integer32 String::ToInteger32() const
{
	Integer32 i32Value = 0;
	Integer8  i8Sign = 1;
	for( Unsigned8 u8Pos = 0; u8Pos < wcslen( m_pString ); ++u8Pos )
	{
		wchar_t c = m_pString[ u8Pos ];
		if( u8Pos == 0 && c == '-' )
		{
			i8Sign = -1;
		}
		else if( '0' <= c && c <= '9' )
		{
			i32Value = ( i32Value * 10 ) + ( c - '0' );
		}
		else
		{
			return i32Value * i8Sign;
		}
	}

	return i32Value * i8Sign;
}

Unsigned8 String::ToUnsigned8() const
{
	Integer32 i32Value = ToInteger32();
	CO_ERROR_IF( i32Value < 0 || i32Value > MAX_U8, L"String::ToUnsigned8: Value out of range." );
	return static_cast<Unsigned8>( i32Value );
}

Unsigned16 String::ToUnsigned16() const
{
	Integer32 i32Value = ToInteger32();
	CO_ERROR_IF( i32Value < 0 || i32Value > MAX_U16, L"String::ToUnsigned16: Value out of range." );
	return static_cast<Unsigned16>( i32Value );
}

Unsigned32 String::ToUnsigned32() const
{
	Integer32 i32Value = ToInteger32();
	CO_ERROR_IF( i32Value < 0 || i32Value > MAX_U32, L"String::ToUnsigned32: Value out of range." );
	return static_cast<Unsigned32>( i32Value );
}

Float16 String::ToFloat16() const
{
	Float32 f32Value = ToFloat32();
	return static_cast<Float16>( f32Value );
}

Float32 String::ToFloat32() const
{
	return wcstod( m_pString, NULL );
}

Integer32 String::HexToInteger() const
{
    Unsigned32 u32Digits = 0;
    return HexToInteger( u32Digits );
}
        
Integer32 String::HexToInteger( Unsigned32& _u32Digits ) const
{
	Integer32 i32Value = 0;
    _u32Digits = 0;
	for( Unsigned8 u8Pos = 0; u8Pos < wcslen( m_pString ); ++u8Pos )
	{
		wchar_t c = m_pString[ u8Pos ];
        ++_u32Digits;
		if ( u8Pos == 0 && 
             c == '0' )
        {
            continue;
        }
        else if ( u8Pos == 1 && 
                 ( c == 'x' || c == 'X' ) )
		{            
            _u32Digits = 0;
			continue;
		}
		else if( '0' <= c && c <= '9' )
		{
			i32Value = ( i32Value * 16 ) + ( c - '0' );
		}
		else if( c == 'a' || c == 'A' )
		{
			i32Value = ( i32Value * 16 ) + 10;
		}
		else if( c == 'b' || c == 'B' )
		{
			i32Value = ( i32Value * 16 ) + 11;
		}
		else if( c == 'c' || c == 'C' )
		{
			i32Value = ( i32Value * 16 ) + 12;
		}
		else if( c == 'd' || c == 'D' )
		{
			i32Value = ( i32Value * 16 ) + 13;
		}
		else if( c == 'e' || c == 'E' )
		{
			i32Value = ( i32Value * 16 ) + 14;
		}
		else if( c == 'f' || c == 'F' )
		{
			i32Value = ( i32Value * 16 ) + 15;
		}
		else
		{
			return i32Value;
		}
	}

	return i32Value;
}

String	  String::GetHexString() const
{
	Integer32 i32Value = ToInteger32();
	bool bNegative = false;
	if( i32Value < 0 )
	{
		bNegative = true;
		i32Value = -i32Value;
	}
	else if( i32Value == 0 )
	{
		return "0";
	}

	String sHex = "";
	while( i32Value != 0 )
	{
		Integer32 i32Rest = i32Value % 16;

		switch( i32Rest )
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			sHex = String( i32Rest ) + sHex;
			break;
		case 10:
			sHex = 'a' + sHex;
			break;
		case 11:
			sHex = 'b' + sHex;
			break;
		case 12:
			sHex = 'c' + sHex;
			break;
		case 13:
			sHex = 'd' + sHex;
			break;
		case 14:
			sHex = 'e' + sHex;
			break;
		case 15:
			sHex = 'f' + sHex;
			break;
		}

		i32Value = i32Value / 16;
	}

	if( bNegative )
	{
		return "f" + sHex;
	}
	else
	{
		return sHex;
	}
}

const String& String::Build( const wchar_t* _format, ... )
{
    static String strTmp;

	va_list argList;
	va_start( argList, _format );

	wchar_t tmp[ MAX_SIZE ];
	memset( tmp, 0, MAX_SIZE );
#ifdef WIN32
	vswprintf_s( tmp, _format, argList );
#else
	vswprintf( tmp, _format, argList );
#endif

	va_end( argList );

	// assign to string
	strTmp.AssignString( tmp );

    return strTmp;
}

const String& String::Build( const char* _format, ... )
{
    static String strTmp;

	va_list argList;
	va_start( argList, _format );

	char tmp[ MAX_SIZE ];
	memset( tmp, 0, MAX_SIZE );
#ifdef WIN32
	vsprintf_s( tmp, _format, argList );
#else
	vsprintf( tmp, _format, argList );
#endif

	va_end( argList );

	// assign to this string
	strTmp.AssignString( tmp );

    return strTmp;
}

void String::Format( const wchar_t* _format, ... )
{
	va_list argList;
	va_start( argList, _format );

	wchar_t tmp[ MAX_SIZE ];
	memset( tmp, 0, MAX_SIZE );
#ifdef WIN32
	vswprintf_s( tmp, _format, argList );
#else
	vswprintf( tmp, _format, argList );
#endif

	va_end( argList );

	// assign to this string
	AssignString( tmp );
}

void String::Format( const char *format, ... )
{
	va_list argList;
	va_start( argList, format );

	char tmp[ MAX_SIZE ];
	memset( tmp, 0, MAX_SIZE );
#ifdef WIN32
	vsprintf_s( tmp, format, argList );
#else
	vsprintf( tmp, format, argList );
#endif

	va_end( argList );

	// assign to this string
	AssignString( tmp );
}

String String::Head( Unsigned32 u32To ) const
{
	return Middle( 0, u32To );
}

String String::Middle( Unsigned32 u32From, Unsigned32 u32To ) const
{
	CO_ASSERT( u32From >= 0 && u32From <= u32To && u32To <= GetLength(), L"String::Middle: Index out of bounds" );

	Unsigned32 u32Length = u32To - u32From + 1;
	wchar_t *szTmp = new wchar_t[ u32Length ];
#ifdef WIN32
	wcsncpy_s( szTmp, u32Length, m_pString + u32From, u32Length - 1 );
#else
	wcsncpy( szTmp, m_pString + u32From, u32To - u32From );
#endif
	szTmp[ u32To - u32From ] = '\0';

	String sNewString( szTmp );
	delete [] szTmp;

	return sNewString;
}

String String::Tail( Unsigned32 u32From ) const
{
    if ( u32From >= GetLength() )
    {
        return String();
    }
    else
    {
	    return Middle( u32From, GetLength() );
    }
}

void String::Trim()
{
	TrimHead();
	TrimTail();
}

void String::TrimHead()
{
	Unsigned32 u32Pos = 0;

	bool bChanged = false;
	while ( IsWhitespace( u32Pos ) )
	{
		bChanged = true;
		++u32Pos;
	}

	if( bChanged )
	{
		*this = Tail( u32Pos );
	}
}

void String::TrimTail()
{
	Unsigned32 u32Pos = GetLength();

	bool bChanged = false;
	while ( IsWhitespace( u32Pos - 1 ) )
	{
        --u32Pos;
		bChanged = true;
	}

	if( bChanged )
	{
		*this = Head( u32Pos );
	}
}

bool String::Find( const String &sSubString ) const
{
	return wcsstr( m_pString, sSubString.m_pString ) != NULL;
}
        
bool String::IsWhitespace( wchar_t _ch )
{
	switch ( _ch )
	{
		case L' ':
		case L'\t':
		case L'\n':
		case L'\r':
			return true;
		default:
			return false;
	}
}
        
bool String::IsWhitespace( Unsigned32 _u32Position ) const
{
    if ( _u32Position >= GetLength() )
    {
        return true;
    }

    return IsWhitespace( *( m_pString + _u32Position ) );
}

void String::AssignString( const char* szString )
{
	if( szString != NULL )
	{
		Unsigned32 length = strlen( szString );

		m_pString = new wchar_t[ length + 1 ];
#ifdef WIN32
		size_t numOfCharConverted = 0;
		mbstowcs_s( &numOfCharConverted, m_pString, length + 1, szString, length + 1 );
#else
		mbstowcs( m_pString, szString, length + 1 );
#endif

		m_pCString = new char[ length + 1 ];
#ifdef WIN32
		strcpy_s( m_pCString, length + 1, szString );
#else
		strcpy( m_pCString, szString );
#endif
	}
	else
	{
		m_pString = NULL;
	}
}

void String::AssignString( const wchar_t* szString )
{
	if( szString != NULL )
	{
		Unsigned32 length = wcslen( szString );
		m_pString = new wchar_t[ length + 1 ];
#ifdef WIN32
		wcscpy_s( m_pString, length + 1, szString );
#else
		wcscpy( m_pString, szString );
#endif

		m_pCString = new char[ length + 1 ];
#ifdef WIN32
		size_t numOfCharConverted = 0;
		wcstombs_s( &numOfCharConverted, m_pCString, length + 1, szString, length + 1 );
#else
		wcstombs( m_pCString, szString, length + 1 );
#endif
	}
	else
	{
		m_pString = NULL;
		m_pCString = NULL;
	}
}

void String::Clear()
{
    if ( m_pString != NULL )
    {
	    delete[] m_pString;
	    m_pString = NULL;
    }
    if ( m_pCString != NULL )
    {
        delete[] m_pCString;
        m_pCString = NULL;
    }
}

std::ostream& operator<<( std::ostream& rOut, const String& sString )
{
	rOut << sString.c_str();
	return rOut;
}

std::wostream& operator<<(std::wostream& rOut, const String& rString)
{
	rOut << rString.wc_str();
	return rOut;
}
