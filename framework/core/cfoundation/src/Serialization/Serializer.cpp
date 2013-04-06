#include "cfoundation/Serialization/Serializer.h"

#include <iostream>
#include <fstream>

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Text/String.h"

using namespace CFoundation;

Serializer::Serializer( Mode _eMode )
:	m_eMode( _eMode ),
    m_pBuffer( NULL ),
	m_u32BufferSize( 0 ),
	m_u32Offset( 0 )
{
}

Serializer::~Serializer()
{
    Clear();
}
      
void Serializer::Clear()
{
    if ( m_pBuffer != NULL )
    {
		delete [] (char*)m_pBuffer;
        m_pBuffer = NULL;
    }
    m_u32BufferSize = 0;
    m_u32Offset = 0;
}
        
void Serializer::ResetOffset()
{
    m_u32Offset = 0;
}

void Serializer::SetMode( Mode _eMode )
{
    m_eMode = _eMode;
}
        
bool Serializer::IsSaving() const
{
    return m_eMode == MODE_SAVE;
}

bool Serializer::IsLoading() const
{
    return m_eMode == MODE_LOAD;
}
        
void Serializer::SetContext( SerializationContextPtr _spContext )
{
    m_spContext = _spContext;
}
        
SerializationContextPtr Serializer::GetContext() const
{
    return m_spContext;
}
        
void Serializer::SerializeVersion( Unsigned32& _u32Version, Unsigned32 _u32CurrentVersion )
{
    if ( m_eMode == MODE_SAVE )
    {
        _u32Version = _u32CurrentVersion;
    }
    Serialize( _u32Version );
    CF_WARN_IF( _u32CurrentVersion < _u32Version, "Serialization: Program version is smaller than serialized version." );
}

void Serializer::Serialize( Unsigned8& _u8Value )
{
    CF_WARN_IF( m_eMode != MODE_SAVE && m_eMode != MODE_LOAD, "Invalid mode." );
    m_eMode == MODE_SAVE ? Add( _u8Value ) : Get( _u8Value );
}

void Serializer::Serialize( Unsigned16& _u16Value )
{
    CF_WARN_IF( m_eMode != MODE_SAVE && m_eMode != MODE_LOAD, "Invalid mode." );
    m_eMode == MODE_SAVE ? Add( _u16Value ) : Get( _u16Value );
}

void Serializer::Serialize( Unsigned32& _u32Value )
{
    CF_WARN_IF( m_eMode != MODE_SAVE && m_eMode != MODE_LOAD, "Invalid mode." );
    m_eMode == MODE_SAVE ? Add( _u32Value ) : Get( _u32Value );
}

void Serializer::Serialize( Unsigned64& _u64Value )
{
    CF_WARN_IF( m_eMode != MODE_SAVE && m_eMode != MODE_LOAD, "Invalid mode." );
    m_eMode == MODE_SAVE ? Add( _u64Value ) : Get( _u64Value );
}

void Serializer::Serialize( Integer8& _i8Value )
{
    CF_WARN_IF( m_eMode != MODE_SAVE && m_eMode != MODE_LOAD, "Invalid mode." );
    m_eMode == MODE_SAVE ? Add( _i8Value ) : Get( _i8Value );
}

void Serializer::Serialize( Integer16& _i16Value )
{
    CF_WARN_IF( m_eMode != MODE_SAVE && m_eMode != MODE_LOAD, "Invalid mode." );
    m_eMode == MODE_SAVE ? Add( _i16Value ) : Get( _i16Value );
}

void Serializer::Serialize( Integer32& _i32Value )
{
    CF_WARN_IF( m_eMode != MODE_SAVE && m_eMode != MODE_LOAD, "Invalid mode." );
    m_eMode == MODE_SAVE ? Add( _i32Value ) : Get( _i32Value );
}

void Serializer::Serialize( Integer64& _i64Value )
{
    CF_WARN_IF( m_eMode != MODE_SAVE && m_eMode != MODE_LOAD, "Invalid mode." );
    m_eMode == MODE_SAVE ? Add( _i64Value ) : Get( _i64Value );
}

void Serializer::Serialize( Float16& _f16Value )
{
    CF_WARN_IF( m_eMode != MODE_SAVE && m_eMode != MODE_LOAD, "Invalid mode." );
    m_eMode == MODE_SAVE ? Add( _f16Value ) : Get( _f16Value );
}

void Serializer::Serialize( Float32& _f32Value )
{
    CF_WARN_IF( m_eMode != MODE_SAVE && m_eMode != MODE_LOAD, "Invalid mode." );
    m_eMode == MODE_SAVE ? Add( _f32Value ) : Get( _f32Value );
}

void Serializer::Serialize( bool& _bValue )
{
    CF_WARN_IF( m_eMode != MODE_SAVE && m_eMode != MODE_LOAD, "Invalid mode." );
    m_eMode == MODE_SAVE ? Add( _bValue ) : Get( _bValue );
}

void Serializer::Serialize( String& _strValue )
{
    CF_WARN_IF( m_eMode != MODE_SAVE && m_eMode != MODE_LOAD, "Invalid mode." );
    m_eMode == MODE_SAVE ? Add( _strValue ) : Get( _strValue );
}

void Serializer::Add( Unsigned8 u8Value )
{
	AddData( &u8Value, sizeof( Unsigned8 ) );
}

void Serializer::Add( Unsigned16 u16Value )
{
	AddData( &u16Value, sizeof( Unsigned16 ) );
}

void Serializer::Add( Unsigned32 u32Value )
{
	AddData( &u32Value, sizeof( Unsigned32 ) );
}

void Serializer::Add( Unsigned64 u64Value )
{
	AddData( &u64Value, sizeof( Unsigned64 ) );
}

void Serializer::Add( Integer8 i8Value )
{
	AddData( &i8Value, sizeof( Integer8 ) );
}

void Serializer::Add( Integer16 i16Value )
{
	AddData( &i16Value, sizeof( Integer16 ) );
}

void Serializer::Add( Integer32 i32Value )
{
	AddData( &i32Value, sizeof( Integer32 ) );
}

void Serializer::Add( Integer64 _i64Value )
{
	AddData( &_i64Value, sizeof( Integer64 ) );
}

void Serializer::Add( Float16 _f16Value )
{
	AddData( &_f16Value, sizeof( Float16 ) );
}

void Serializer::Add( Float32 _f32Value )
{
	AddData( &_f32Value, sizeof( Float32 ) );
}

void Serializer::Add( bool bValue )
{
	AddData( &bValue, sizeof( bool ) );
}

void Serializer::Add( const String& _strValue )
{
	Unsigned32 u32Length = _strValue.GetLength();
	Add( u32Length );
    if ( u32Length > 0 )
    {
	    AddData( _strValue.wc_str(), sizeof( wchar_t ) * ( u32Length + 1 ) );
    }
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

void Serializer::Get( Unsigned8& _u8Value ) const
{
    _u8Value = GetU8();
}

void Serializer::Get( Unsigned16& _u16Value ) const
{
    _u16Value = GetU16();
}

void Serializer::Get( Unsigned32& _u32Value ) const
{
    _u32Value = GetU32();
}

void Serializer::Get( Unsigned64& _u64Value ) const
{
    _u64Value = GetU64();
}

void Serializer::Get( Integer8& _i8Value ) const
{
    _i8Value = GetI8();
}

void Serializer::Get( Integer16& _i16Value ) const
{
    _i16Value = GetI16();
}

void Serializer::Get( Integer32& _i32Value ) const
{
    _i32Value = GetI32();
}

void Serializer::Get( Integer64& _i64Value ) const
{
    _i64Value = GetI64();
}

void Serializer::Get( Float16& _f16Value ) const
{
    _f16Value = GetF16();
}

void Serializer::Get( Float32& _f32Value ) const
{
    _f32Value = GetF32();
}

void Serializer::Get( bool& _bValue ) const
{
    _bValue = GetBool();
}

void Serializer::Get( String& _strValue ) const
{
    _strValue = GetString();
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

Integer64 Serializer::GetI64() const
{
	const void *pData = GetData( sizeof( Integer64 ) );
	return *( static_cast<const Integer64*>( pData ) );
}

Float16 Serializer::GetF16() const
{
	const void *pData = GetData( sizeof( Float16 ) );
	return *( static_cast< const Float16* >( pData ) );
}

Float32 Serializer::GetF32() const
{
	const void *pData = GetData( sizeof( Float32 ) );
	return *( static_cast< const Float32* >( pData ) );
}

bool Serializer::GetBool() const
{
	const void *pData = GetData( sizeof( bool ) );
	return *( static_cast<const bool*>( pData ) );
}

const String Serializer::GetString() const
{
	Unsigned32 u32Length = GetU32();
    if ( u32Length > 0 )
    {
	    const void *pData = GetData( sizeof( wchar_t ) * ( u32Length + 1 ) );
	    return String( static_cast<const wchar_t*>( pData ) );
    }
    else
    {
        return String();
    }
}

const void* Serializer::GetData( Unsigned32 u32Size ) const
{
	Unsigned32 u32OldOffset = m_u32Offset;
	m_u32Offset += u32Size;
	return static_cast<const char*>( m_pBuffer ) + u32OldOffset;
}
		
const void* Serializer::GetBuffer() const
{
    return m_pBuffer;
}

Unsigned32 Serializer::GetBufferSize() const
{
    return m_u32BufferSize;
}
		
Unsigned32 Serializer::GetOffset() const
{
    return m_u32Offset;
}

void Serializer::SaveFile( const String &sFileName ) const
{
	// open file
	const char *szPath = sFileName.c_str();
    std::ofstream fFile( szPath, std::fstream::out | std::fstream::binary );

	if ( fFile.is_open() )
	{
		// write buffer to file
		fFile.write( static_cast<char*>( m_pBuffer ), m_u32Offset );
	}
	else
	{
        CF_WARN( L"Serializer::SaveFile(): Could not open file %s.", sFileName.wc_str() );
	}

	// close file
	fFile.close();
}

bool Serializer::LoadFile( const String &sFileName )
{
	// open file
	const char *szPath = sFileName.c_str();
	std::ifstream fFile( szPath, std::fstream::in | std::fstream::binary );
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

        // Reset offset
        ResetOffset();

		return true;
	}
	else
	{
        //CF_WARN( L"Serializer::LoadFile(): Could not open file %s.", sFileName.wc_str() );
		return false;
	}
}
       
bool Serializer::LoadData( const void* _pBuffer, Unsigned32 _u32BufferSize )
{
	// resize Buffer
	while ( m_u32BufferSize < _u32BufferSize )
	{
		ResizeBuffer();
	}

    // Copy buffer
    memcpy( m_pBuffer, _pBuffer, _u32BufferSize );

    // Reset offset
    ResetOffset();

    return true;
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
