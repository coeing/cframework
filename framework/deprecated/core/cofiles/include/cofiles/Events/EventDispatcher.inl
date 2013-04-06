
template < class Enum >
void CoFiles::EventDispatcher< Enum >::DispatchEvent( Enum _eEvent )
{
    CallbackVector* pCallbacks = GetCallbacks( _eEvent );
    if ( pCallbacks == NULL )
    {
        return;
    }

    // Do callbacks
    for ( CallbackVector::iterator it = pCallbacks->begin(); it != pCallbacks->end(); ++it )
    {
        CallbackBasePtr pCallback = *it;
        pCallback->DoCallback();
    }
}

template < class Enum >
void CoFiles::EventDispatcher< Enum >::RegisterCallback( Enum _eEvent, CallbackBasePtr _pCallback )
{
    CallbackVector* pCallbacks = GetCallbacks( _eEvent );
    if ( pCallbacks == NULL )
    {
        pCallbacks = new CallbackVector;
        m_mapEventCallbacks[ _eEvent ] = pCallbacks;
    }
    pCallbacks->push_back( _pCallback );
}

template < class Enum >
void CoFiles::EventDispatcher< Enum >::RemoveCallback( Enum _eEvent, CallbackBasePtr _pCallback )
{
    CallbackVector* pCallbacks = GetCallbacks( _eEvent );
    if ( pCallbacks == NULL )
    {
        return;
    }
    CallbackVector::iterator it = std::find( pCallbacks->begin(), pCallbacks->end(), _pCallback );
    if ( it != pCallbacks->end() )
    {
        pCallbacks->erase( it );
    }
}
