#include "cofiles/Serializer.h"

#include <iostream>
#include <fstream>

#include "cofiles/Makros.h"

using namespace CoFiles;

Serializer::Serializer()
:	m_pBuffer( NULL ),
	m_u32BufferSize( 0 ),
	m_u32Offset( 0 )
{
}

void Serializer::AddU8( Unsigned8 u8Value )
{
	AddData( &u8Value, sizeof( Unsigned8 ) );
}

void Serializer::AddU16( Unsigned16 u16Value )
{
	AddData( &u16Value, sizeof( Unsigned16 ) );
}

void Serializer::AddU32( Unsigned32 u32Value )
{
	AddData( &u32Value, sizeof( Unsigned32 ) );
}

void Serializer::AddU64( Unsigned64 u64Value )
{
	AddData( &u64Value, sizeof( Unsigned64 ) );
}

void Serializer::AddI8( Integer8 i8Value )
{
	AddData( &i8Value, sizeof( Integer8 ) );
}

void Serializer::AddI16( Integer16 i16Value )
{
	AddData( &i16Value, sizeof( Integer16 ) );
}

void Serializer::AddI32( Integer32 i32Value )
{
	AddData( &i32Value, sizeof( Integer32 ) );
}

void Serializer::AddBool( bool bValue )
{
	AddData( &bValue, sizeof( bool ) );
}

void Serializer::AddString( const String &sValue )
{
	Unsigned32 u32Length = sValue.GetLength();
	AddU32( u32Length );
	AddData( sValue.wc_str(), sizeof( wchar_t ) * u32Length );
}

void Serializer::AddData( const void *pData, Unsigned32 u32Size )
{
	if( u32Size == 0 )
	{
		return;
	}

	while( m_u32Offset + u32Size > m_u32BufferSize )
	{
		ResizeBuffer();
	}

	memcpy( static_cast<char*>( m_pBuffer ) + m_u32Offset, pData, u32Size );
	m_u32Offset += u32Size;
}

Unsigned8	Serializer::GetU8() const
{
	const void *pData = GetData( sizeof( Unsigned8 ) );
	return *( static_cast<const Unsigned8*>( pData ) );
}

Unsigned16	Serializer::GetU16() const
{
	const void *pData = GetData( sizeof( Unsigned16 ) );
	return *( static_cast<const Unsigned16*>( pData ) );
}

Unsigned32	Serializer::GetU32() const
{
	const void *pData = GetData( sizeof( Unsigned32 ) );
	return *( static_cast<const Unsigned32*>( pData ) );
}

Unsigned64	Serializer::GetU64() const
{
	const void *pData = GetData( sizeof( Unsigned64 ) );
	return *( static_cast<const Unsigned64*>( pData ) );
}

Integer8	Serializer::GetI8() const
{
	const void *pData = GetData( sizeof( Integer8 ) );
	return *( static_cast<const Integer8*>( pData ) );
}

Integer16	Serializer::GetI16() const
{
	const void *pData = GetData( sizeof( Integer16 ) );
	return *( static_cast<const Integer16*>( pData ) );
}

Integer32	Serializer::GetI32() const
{
	const void *pData = GetData( sizeof( Integer32 ) );
	return *( static_cast<const Integer32*>( pData ) );
}

bool		Serializer::GetBool() const
{
	const void *pData = GetData( sizeof( bool ) );
	return *( static_cast<const bool*>( pData ) );
}

const String Serializer::GetString() const
{
	Unsigned32 u32Length = GetU32();
	const void *pData = GetData( sizeof( wchar_t ) * u32Length );
	return String( static_cast<const wchar_t*>( pData ) );
}

const void* Serializer::GetData( Unsigned32 u32Size ) const
{
	Unsigned32 u32OldOffset = m_u32Offset;
	m_u32Offset += u32Size;
	return static_cast<const char*>( m_pBuffer ) + u32OldOffset;
}

void Serializer::SaveFile( const String &sFileName ) const
{
	// open file
	const char *szPath = sFileName.c_str();
	std::ofstream fFile( szPath );

	if ( fFile.is_open() )
	{
		// write buffer to file
		fFile.write( static_cast<char*>( m_pBuffer ), m_u32Offset );
	}
	else
	{
        CO_WARN( L"Serializer::SaveFile(): Could not open file %s.", sFileName.wc_str() );
	}

	// close file
	fFile.close();
}

bool Serializer::LoadFile( const String &sFileName )
{
	// open file
	const char *szPath = sFileName.c_str();
	std::ifstream fFile( szPath );
	if( fFile.is_open() )
	{
		// get file size
		std::streampos tBegin = fFile.tellg();
		fFile.seekg( 0, std::ios::end );
		std::streampos tEnd = fFile.tellg();
		Unsigned32 u32FileSize = static_cast<Unsigned32>( tEnd - tBegin );
		fFile.seekg( 0 );

		// resize Buffer
		while( m_u32BufferSize < u32FileSize )
		{
			ResizeBuffer();
		}

		// load buffer to file
		fFile.read( static_cast<char*>( m_pBuffer ), u32FileSize );

		// close file
		fFile.close();

		return true;
	}
	else
	{
        CO_WARN( L"Serializer::LoadFile(): Could not open file %s.", sFileName.wc_str() );

		return false;
	}
}

void Serializer::ResizeBuffer()
{
	Unsigned32 u32NewBufferSize = 1024;
	if( m_u32BufferSize > 0 )
	{
		u32NewBufferSize = m_u32BufferSize * 3 / 2;
	}

	char *pNewBuffer = new char[ u32NewBufferSize ];

	if( m_pBuffer != NULL )
	{
		// rescue old data
		memcpy( pNewBuffer, m_pBuffer, m_u32Offset );

		// delete old data
		delete [] (char*)m_pBuffer;
	}

	m_pBuffer = pNewBuffer;
	m_u32BufferSize = u32NewBufferSize;
}
