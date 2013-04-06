#ifndef CFOUNDATION_SERIALIZER_H
#define CFOUNDATION_SERIALIZER_H

#include <map>
#include <vector>

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Base/Types.h"
#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( SerializationContext );

    class String;

	class Serializer
	{
	public:
        enum Mode
        {
            MODE_NONE,
            MODE_SAVE,
            MODE_LOAD,
            MODE_END
        };

        /// Constructor
		Serializer( Mode _eMode = MODE_NONE );

        /// Destructor
		~Serializer();

        /// Clears the serializer
        void        Clear();

        /// Resets the offset
        void        ResetOffset();

        /// Sets the mode
        void        SetMode( Mode _eMode );

		/// Saves the data to the passed file
		void        SaveFile( const String& _strFileName ) const;

		/// Loads the data from the passed file
		bool        LoadFile( const String& _strFileName );

        /// Loads the data from the passed buffer
        bool        LoadData( const void* _pBuffer, Unsigned32 _u32BufferSize );

        /// Indicates if the serializer is currently in saving mode
        bool        IsSaving() const;

        /// Indicates if the serializer is currently in loading mode
        bool        IsLoading() const;

        ///////////////////////////////////////////////////////
        // Context
        ///////////////////////////////////////////////////////

        /// Sets the serialization context
        void        SetContext( SerializationContextPtr _spContext );

        /// Returns the serialization context
        SerializationContextPtr GetContext() const;

        ///////////////////////////////////////////////////////
        // Serialization functions
        ///////////////////////////////////////////////////////

        void        SerializeVersion( Unsigned32& _u32Version, Unsigned32 _u32CurrentVersion );

        void        Serialize( Unsigned8& _u8Value );
        void        Serialize( Unsigned16& _u16Value );
        void        Serialize( Unsigned32& _u32Value );
        void        Serialize( Unsigned64& _u64Value );

        void        Serialize( Integer8& _i8Value );
        void        Serialize( Integer16& _i16Value );
        void        Serialize( Integer32& _i32Value );
        void        Serialize( Integer64& _i64Value );

        void        Serialize( Float16& _f16Value );
        void        Serialize( Float32& _f32Value );

        void        Serialize( bool& _bValue );
        void        Serialize( String& _strValue );

        template < class Enum >
        void        SerializeEnum( Enum& _eValue );

        template < class Wrapper, class Entry >
        void        SerializeProxy( Entry& _Entry );

        template < class Wrapper, class Entry >
        void        SerializeProxy( const Wrapper& _Wrapper, Entry& _Entry );

        template < class Wrapper, class Entry >
        void        SerializeArray( Entry* _apArray, Unsigned32 _u32Size );

        template < class Wrapper, class Entry >
        void        SerializeArray( const Wrapper& _Wrapper, Entry* _apArray, Unsigned32 _u32Size );

        template < class Wrapper, class Entry >
        void        SerializeDynArray( Entry*& _apArray, Unsigned32 _u32Size );

        template < class Wrapper, class Entry >
        void        SerializeDynArray( const Wrapper& _Wrapper, Entry*& _apArray, Unsigned32 _u32Size );

        template < class Wrapper, class Entry >
        void        SerializeVector( std::vector< Entry >& _aVector );

        template < class Wrapper, class Entry >
        void        SerializeVector( const Wrapper& _Wrapper, std::vector< Entry >& _aVector );

        template < class WrapperKey, class WrapperValue, class Key, class Value >
        void        SerializeMap( std::map< Key, Value >& _Map );

        ///////////////////////////////////////////////////////
		// Add Data Functions
        ///////////////////////////////////////////////////////

		void        AddData( const void* _pData, Unsigned32 _u32Size );

		void        Add( Unsigned8 _u8Value );
		void        Add( Unsigned16 _u16Value );
		void        Add( Unsigned32 _u32Value );
		void        Add( Unsigned64 _u64Value );
		
		void        Add( Integer8 _i8Value );
		void        Add( Integer16 _i16Value );
		void        Add( Integer32 _i32Value );
		void        Add( Integer64 _i64Value );

		void        Add( Float16 _f16Value );
		void        Add( Float32 _f32Value );

		void        Add( bool _bValue );
		
		void        Add( const String& _strValue );

        template < class Enum >
        void        AddEnum( const Enum& _eValue );

        ///////////////////////////////////////////////////////
		// Get Data Functions
        ///////////////////////////////////////////////////////

		const void* GetData( Unsigned32 u32Size ) const;
        
        void        Get( Unsigned8& _u8Value ) const;
        void        Get( Unsigned16& _u16Value ) const;
        void        Get( Unsigned32& _u32Value ) const;
        void        Get( Unsigned64& _u64Value ) const;

        void        Get( Integer8& _i8Value ) const;
        void        Get( Integer16& _i16Value ) const;
        void        Get( Integer32& _i32Value ) const;
        void        Get( Integer64& _i64Value ) const;

        void        Get( Float16& _f16Value ) const;
        void        Get( Float32& _f32Value ) const;

        void        Get( bool& _bValue ) const;
        void        Get( String& _strValue ) const;

        template < class Enum >
        void        GetEnum( Enum& _eValue ) const;

		Unsigned8	GetU8() const;
		Unsigned16	GetU16() const;
		Unsigned32	GetU32() const;
		Unsigned64	GetU64() const;

		Integer8	GetI8() const;
		Integer16	GetI16() const;
		Integer32	GetI32() const;
		Integer64	GetI64() const;

		Float16     GetF16() const;
		Float32     GetF32() const;

		bool		GetBool() const;

		const String GetString() const;

        template < class Enum >
        Enum        GetEnum() const;

        /// Returns the buffer
		const void* GetBuffer() const;

        /// Returns the buffer size
		Unsigned32  GetBufferSize() const;

        /// Returns the current offset
        /// I.e. the current data size while saving
		Unsigned32  GetOffset() const;

	private:
        /// Resizes the data buffer
		void ResizeBuffer();

        /// Mode
        Mode                m_eMode;

		void				*m_pBuffer;
		Unsigned32			m_u32BufferSize;
		mutable Unsigned32	m_u32Offset;

        /// Serialization context
        SerializationContextPtr m_spContext;
	};
}

namespace CFoundation
{
    class SerializeWrapper_Value
    {
    public:
        /// Serialize object
        template < class Object >
        static void     Serialize( Serializer& _Serializer, Object& _Object )
        {
            _Serializer.Serialize( _Object );
        }
    };

    class SerializeWrapper_Simple
    {
    public:
        /// Serialize object
        template < class Object >
        static void     Serialize( Serializer& _Serializer, Object& _Object )
        {
            _Object.Serialize( _Serializer );
        }
    };

    class SerializeWrapper_SharedPtrSimple
    {
    public:
        /// Serialize object
        template < class Object >
        static void     Serialize( Serializer& _Serializer, SharedPtr< Object >& _spObject )
        {
            bool bPresent = _spObject != NULL;
            _Serializer.Serialize( bPresent );
            if ( bPresent )
            {
                if ( _spObject == NULL )
                {
                    _spObject = SharedPtr< Object >( new Object() );
                }
                _spObject->Serialize( _Serializer );
            }
        }
    };

    template < class Enum >
    void Serializer::SerializeEnum( Enum& _eValue )
    {
        m_eMode == MODE_SAVE ? AddEnum( _eValue ) : GetEnum( _eValue );
    }

    template < class Enum >
    void Serializer::AddEnum( const Enum& _eValue )
    {
	    AddData( &_eValue, sizeof( Enum ) );
    }

    template < class Enum >
    void Serializer::GetEnum( Enum& _eValue ) const
    {
        _eValue = GetEnum< Enum >();
    }

    template < class Enum >
    Enum Serializer::GetEnum() const
    {
	    const void* pData = GetData( sizeof( Enum ) );
	    return *( static_cast< const Enum* >( pData ) );
    }

    template < class Wrapper, class Entry >
    void Serializer::SerializeProxy( Entry& _Entry )
    {
        Wrapper::Serialize( *this, _Entry );
    }
       
    template < class Wrapper, class Entry >
    void Serializer::SerializeProxy( const Wrapper& _Wrapper, Entry& _Entry )
    {
        _Wrapper.Serialize( *this, _Entry );
    }

    template < class Wrapper, class Entry >
    void Serializer::SerializeArray( Entry* _apArray, Unsigned32 _u32Size )
    {
        if ( IsSaving() )
        {
            // Store size
            Add( _u32Size );

            // Store entries
            for ( Unsigned32 u32Idx = 0; u32Idx < _u32Size; ++u32Idx )
            {
                Wrapper::Serialize( *this, _apArray[ u32Idx ] );
            }
        }
        else
        {
            // Load size
            Unsigned32 u32NumEntries = GetU32();
            CF_WARN_IF( u32NumEntries != _u32Size, "Serializer::SerializeArray: Stored size and current size differ." );

            // Load entries
            for ( Unsigned32 u32Idx = 0; u32Idx < u32NumEntries; ++u32Idx )
            {
                Entry entry;
                Wrapper::Serialize( *this, entry );

                if ( u32Idx < _u32Size )
                {
                    _apArray[ u32Idx ] = entry;
                }
            }

            // Fill remaining spots with default entries
            for ( Unsigned32 u32Idx = u32NumEntries; u32Idx < _u32Size; ++u32Idx )
            {
                _apArray[ u32Idx ] = Entry();
            }
        }
    }

    template < class Wrapper, class Entry >
    void Serializer::SerializeArray( const Wrapper& _Wrapper, Entry* _apArray, Unsigned32 _u32Size )
    {
        if ( IsSaving() )
        {
            // Store size
            Add( _u32Size );

            // Store entries
            for ( Unsigned32 u32Idx = 0; u32Idx < _u32Size; ++u32Idx )
            {
                _Wrapper.Serialize( *this, _apArray[ u32Idx ] );
            }
        }
        else
        {
            // Clear array
            memset( _apArray, 0, _u32Size * sizeof( Entry ) );

            // Load size
            Unsigned32 u32NumEntries = GetU32();
            CF_WARN_IF( u32NumEntries != _u32Size, "Serializer::SerializeArray: Stored size and current size differ." );

            // Load entries
            for ( Unsigned32 u32Idx = 0; u32Idx < u32NumEntries; ++u32Idx )
            {
                Entry entry;
                _Wrapper.Serialize( *this, entry );

                if ( u32Idx < _u32Size )
                {
                    _apArray[ u32Idx ] = entry;
                }
            }

            // Fill remaining spots with default entries
            for ( Unsigned32 u32Idx = u32NumEntries; u32Idx < _u32Size; ++u32Idx )
            {
                _apArray[ u32Idx ] = Entry();
            }
        }
    }

    template < class Wrapper, class Entry >
    void Serializer::SerializeDynArray( Entry*& _apArray, Unsigned32 _u32Size )
    {
        if ( IsLoading() )
        {
            // Delete old array
            SAFE_DELETE_ARRAY( _apArray );

            if ( _u32Size > 0 )
            {
                // Create new one
                _apArray = new Entry[ _u32Size ];
            }
        }

        SerializeArray< Wrapper, Entry >( _apArray, _u32Size );
    }

    template < class Wrapper, class Entry >
    void Serializer::SerializeDynArray( const Wrapper& _Wrapper, Entry*& _apArray, Unsigned32 _u32Size )
    {
        if ( IsLoading() )
        {
            // Delete old array
            SAFE_DELETE_ARRAY( _apArray );

            if ( _u32Size > 0 )
            {
                // Create new one
                _apArray = new Entry[ _u32Size ];
            }
        }

        SerializeArray< Wrapper, Entry >( _Wrapper, _apArray, _u32Size );
    }

    template < class Wrapper, class Entry >
    void Serializer::SerializeVector( std::vector< Entry >& _aVector )
    {
        if ( IsSaving() )
        {
            // Store size
            Unsigned32 u32NumEntries = _aVector.size();
            Add( u32NumEntries );

            // Store entries
            for ( Unsigned32 u32Idx = 0; u32Idx < u32NumEntries; ++u32Idx )
            {
                Wrapper::Serialize( *this, _aVector[ u32Idx ] );
            }
        }
        else
        {
            // Clear vector
            _aVector.clear();

            // Load size
            Unsigned32 u32NumEntries = GetU32();

            // Load entries
            for ( Unsigned32 u32Idx = 0; u32Idx < u32NumEntries; ++u32Idx )
            {
                Entry entry;
                Wrapper::Serialize( *this, entry );
                _aVector.push_back( entry );
            }
        }
    }

    template < class Wrapper, class Entry >
    void Serializer::SerializeVector( const Wrapper& _Wrapper, std::vector< Entry >& _aVector )
    {
        if ( IsSaving() )
        {
            // Store size
            Unsigned32 u32NumEntries = _aVector.size();
            Add( u32NumEntries );

            // Store entries
            for ( Unsigned32 u32Idx = 0; u32Idx < u32NumEntries; ++u32Idx )
            {
                _Wrapper.Serialize( *this, _aVector[ u32Idx ] );
            }
        }
        else
        {
            // Clear vector
            _aVector.clear();

            // Load size
            Unsigned32 u32NumEntries = GetU32();

            // Load entries
            for ( Unsigned32 u32Idx = 0; u32Idx < u32NumEntries; ++u32Idx )
            {
                Entry entry;
                _Wrapper.Serialize( *this, entry );
                _aVector.push_back( entry );
            }
        }
    }  
        
    template < class WrapperKey, class WrapperValue, class Key, class Value >        
    void Serializer::SerializeMap( std::map< Key, Value >& _Map )
    {
        if ( IsSaving() )
        {
            // Store size
            Unsigned32 u32NumEntries = _Map.size();
            Add( u32NumEntries );

            // Store entries
            for ( std::map< Key, Value >::iterator it = _Map.begin(); it != _Map.end(); ++it )
            {
                Key key = it->first;
                WrapperKey::Serialize( *this, key );
                WrapperValue::Serialize( *this, it->second );
            }
        }
        else
        {
            // Clear map
            _Map.clear();

            // Load size
            Unsigned32 u32NumEntries = GetU32();

            // Load entries
            for ( Unsigned32 u32Idx = 0; u32Idx < u32NumEntries; ++u32Idx )
            {
                Key key;
                WrapperKey::Serialize( *this, key );
                Value value;
                WrapperValue::Serialize( *this, value );

                _Map[ key ] = value;
            }
        }
    }
}

#endif
