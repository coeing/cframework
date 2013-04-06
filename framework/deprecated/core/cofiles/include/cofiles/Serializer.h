#ifndef COFILES_SERIALIZER_H
#define COFILES_SERIALIZER_H

#include "cofiles/Types.h"
#include "cofiles/String.h"

namespace CoFiles
{
	class Serializer
	{
	public:
		Serializer();

		// Add Data Functions
		void AddData( const void *pData, Unsigned32 u32Size );

		void AddU8( Unsigned8 u8Value );
		void AddU16( Unsigned16 u16Value );
		void AddU32( Unsigned32 u32Value );
		void AddU64( Unsigned64 u64Value );
		
		void AddI8( Integer8 i8Value );
		void AddI16( Integer16 i16Value );
		void AddI32( Integer32 i32Value );

		void AddBool( bool bValue );
		
		void AddString( const String &sValue );

		// Get Data Functions
		const void* GetData( Unsigned32 u32Size ) const;

		Unsigned8	GetU8() const;
		Unsigned16	GetU16() const;
		Unsigned32	GetU32() const;
		Unsigned64	GetU64() const;

		Integer8	GetI8() const;
		Integer16	GetI16() const;
		Integer32	GetI32() const;

		bool		GetBool() const;

		const String GetString() const;

		// Save/Load Functions
		void SaveFile( const String &sFileName ) const;
		bool LoadFile( const String &sFileName );

	private:

		void ResizeBuffer();

		void				*m_pBuffer;
		Unsigned32			m_u32BufferSize;
		mutable Unsigned32	m_u32Offset;
	};
}

#endif
