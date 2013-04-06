namespace CFoundation
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// SerializationContext
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    template < class Object >
    void SerializationContext::Serialize( Serializer& _Serializer, SharedPtr< Object >& _spObject )
    {
        bool bPresent = _spObject != NULL;
        _Serializer.Serialize( bPresent );
        if ( bPresent )
        {
            String strType;
            SerializeMethodMap::const_iterator it = m_mapSerializeMethods.end();
            if ( _Serializer.IsSaving() )
            {
                // Get object type
                RTTItypeid type_id = STATIC_TYPE_INFO( Object );

                // Check if dynamic type
                bool bDynamic = type_id.num_subclasses() > 0;
                _Serializer.Serialize( bDynamic );

                // If dynamic store type name
                if ( bDynamic )
                {
                    type_id = TYPE_INFO( _spObject.get() );
                    strType = String( type_id.getname() );
                    _Serializer.Serialize( strType );
                }
                    
                // Get serialize method
                it = m_mapSerializeMethods.find( type_id );
            }
            else
            {
                // Check if dynamic type
                bool bDynamic = false;
                _Serializer.Serialize( bDynamic );

                if ( bDynamic )
                {
                    // Get object type
                    _Serializer.Serialize( strType );
                    RTTIdyntypeid dyn_type_id( strType.c_str() );

                    // Get serialize method
                    it = m_mapSerializeMethods.find( dyn_type_id );
                }
                else
                {
                    // Get object type
                    RTTItypeid type_id = STATIC_TYPE_INFO( Object );
                    strType = type_id.getname();

                    // Get serialize method
                    it = m_mapSerializeMethods.find( type_id );
                }
            }

            // Get serialize method for type
            if ( it != m_mapSerializeMethods.end() )
            {
                VoidPtr spDelegate = it->second;
                typedef Delegate2< Serializer&, SharedPtr< Object >& > BaseDelegate;
                BaseDelegate* pDelegate = static_cast< BaseDelegate* >( spDelegate.get() );
                (*pDelegate)( _Serializer, _spObject );
            }
            else
            {
                if ( _Serializer.IsLoading() )
                {
                    // Create object
                    RTTIdyntypeid dyn_type_id( strType.c_str() );
                    _spObject = SharedPtr< Object >( (Object*)STATIC_TYPE_INFO( Object ).create( dyn_type_id ) );
                    CF_WARN_IF( _spObject == NULL, "SerializeWrapper_SharedPtrDyn: Couldn't create an RTTI object while loading." );
                }

                SAFE_CALL( _spObject )->Serialize( _Serializer );
            }
        }
        else
        {
            _spObject.reset();
        }
    }

    template < class Object >
    void SerializationContext::RegisterSerializeMethod( SharedPtr< Delegate2< Serializer&, SharedPtr< Object >& > > _spDelegate )
    {
        RTTItypeid type_id = STATIC_TYPE_INFO( Object );
        m_mapSerializeMethods[ type_id ] = _spDelegate;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// SerializeWrapper_Context
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    template < class Object >
    void SerializeWrapper_Context::Serialize( Serializer& _Serializer, SharedPtr< Object >& _spObject )
    {
        SerializationContextPtr spContext = _Serializer.GetContext();
        if ( spContext != NULL )
        {
            spContext->Serialize( _Serializer, _spObject );
        }
        else
        {
            SerializeWrapper_SharedPtrDyn::Serialize( _Serializer, _spObject );
        }
    }
}
