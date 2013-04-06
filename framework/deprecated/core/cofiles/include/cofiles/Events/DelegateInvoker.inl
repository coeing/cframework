template < class Enum, class Delegate >
std::vector< SharedPtr< Delegate > >* CoFiles::DelegateInvoker< Enum, Delegate >::GetDelegates( Enum _eEvent )
{
    EventDelegatesMap::iterator it = m_mapEventDelegates.find( _eEvent );
    if ( it != m_mapEventDelegates.end() )
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}

template < class Enum, class Delegate >
void CoFiles::DelegateInvoker< Enum, Delegate >::RegisterDelegate( Enum _eEvent, SharedPtr< Delegate > _spDelegate )
{
    DelegateVector* pDelegates = GetDelegates( _eEvent );
    if ( pDelegates == NULL )
    {
        pDelegates = new DelegateVector;
        m_mapEventDelegates[ _eEvent ] = pDelegates;
    }
    pDelegates->push_back( _spDelegate );
}

template < class Enum, class Delegate >
void CoFiles::DelegateInvoker< Enum, Delegate >::RemoveDelegate( Enum _eEvent, SharedPtr< Delegate > _spDelegate )
{
    DelegateVector* pDelegates = GetDelegates( _eEvent );
    if ( pDelegates == NULL )
    {
        return;
    }
    DelegateVector::iterator it = std::find( pDelegates->begin(), pDelegates->end(), _spDelegate );
    if ( it != pDelegates->end() )
    {
        pDelegates->erase( it );
    }
}

template < class Enum, class RetType >
void CoFiles::Delegate0Invoker< Enum, RetType >::DispatchEvent( Enum _eEvent )
{
    DelegateVector* pDelegates = GetDelegates( _eEvent );
    if ( pDelegates == NULL )
    {
        return;
    }

    // Do Delegates
    DelegateVector aspDelegates = *pDelegates;
    for ( DelegateVector::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
    {
        SharedPtr< Delegate0< RetType > > spDelegate = *it;
        (*spDelegate)();
    }
}

template < class Enum, class Param1, class RetType >
void CoFiles::Delegate1Invoker< Enum, Param1, RetType >::DispatchEvent( Enum _eEvent, Param1 _param1 )
{
    DelegateVector* pDelegates = GetDelegates( _eEvent );
    if ( pDelegates == NULL )
    {
        return;
    }

    // Do Delegates
    DelegateVector aspDelegates = *pDelegates;
    for ( DelegateVector::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
    {
        SharedPtr< Delegate1< Param1, RetType > > spDelegate = *it;
        (*spDelegate)( _param1 );
    }
}

template < class Enum, class Param1, class Param2, class RetType >
void CoFiles::Delegate2Invoker< Enum, Param1, Param2, RetType >::DispatchEvent( Enum _eEvent, Param1 _param1, Param2 _param2 )
{
    DelegateVector* pDelegates = GetDelegates( _eEvent );
    if ( pDelegates == NULL )
    {
        return;
    }

    // Do Delegates
    DelegateVector aspDelegates = *pDelegates;
    for ( DelegateVector::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
    {
        SharedPtr< Delegate2< Param1, Param2, RetType > > spDelegate = *it;
        (*spDelegate)( _param1, _param2 );
    }
}

template < class Enum, class Param1, class Param2, class Param3, class RetType >
void CoFiles::Delegate3Invoker< Enum, Param1, Param2, Param3, RetType >::DispatchEvent( Enum _eEvent, Param1 _param1, Param2 _param2, Param3 _param3 )
{
    DelegateVector* pDelegates = GetDelegates( _eEvent );
    if ( pDelegates == NULL )
    {
        return;
    }

    // Do Delegates
    DelegateVector aspDelegates = *pDelegates;
    for ( DelegateVector::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
    {
        SharedPtr< Delegate3< Param1, Param2, Param3, RetType > > spDelegate = *it;
        (*spDelegate)( _param1, _param2, _param3 );
    }
}
