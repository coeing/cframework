#include <algorithm>

template < class Enum, class Delegate >
CFoundation::DelegateInvokerConnection< Enum, Delegate >::DelegateInvokerConnection( DelegateInvoker< Enum, Delegate >* _pInvoker, Enum _eEvent, SharedPtr< Delegate > _spDelegate )
:   m_pInvoker( _pInvoker ),
    m_eEvent( _eEvent ),
    m_spDelegate( _spDelegate )
{
}

template < class Enum, class Delegate >
CFoundation::DelegateInvokerConnection< Enum, Delegate >::~DelegateInvokerConnection()
{
}

template < class Enum, class Delegate >
void CFoundation::DelegateInvokerConnection< Enum, Delegate >::Disconnect()
{
    m_pInvoker->RemoveDelegate( m_eEvent, m_spDelegate );
}

template < class Enum, class Delegate >
CFoundation::DelegateInvoker< Enum, Delegate >::~DelegateInvoker()
{
    for ( EventDelegatesMap::iterator it = m_mapEventDelegates.begin(); it != m_mapEventDelegates.end(); ++it )
    {
        DelegateVec* pDelegates = it->second;
        if ( pDelegates != NULL )
        {
            delete pDelegates;
        }
    }
    m_mapEventDelegates.clear();
}

template < class Enum, class Delegate >
std::vector< SharedPtr< Delegate > >* CFoundation::DelegateInvoker< Enum, Delegate >::GetDelegates( Enum _eEvent )
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
SharedPtr< CFoundation::DelegateInvokerConnection< Enum, Delegate > > CFoundation::DelegateInvoker< Enum, Delegate >::RegisterDelegate( Enum _eEvent, SharedPtr< Delegate > _spDelegate )
{
    DelegateVec* pDelegates = GetDelegates( _eEvent );
    if ( pDelegates == NULL )
    {
        pDelegates = new DelegateVec;
        m_mapEventDelegates[ _eEvent ] = pDelegates;
    }
    pDelegates->push_back( _spDelegate );

    return SharedPtr< DelegateInvokerConnection< Enum, Delegate > >( new DelegateInvokerConnection< Enum, Delegate >( this, _eEvent, _spDelegate ) );
}

template < class Enum, class Delegate >
void CFoundation::DelegateInvoker< Enum, Delegate >::RemoveDelegate( Enum _eEvent, SharedPtr< Delegate > _spDelegate )
{
    DelegateVec* pDelegates = GetDelegates( _eEvent );
    if ( pDelegates == NULL )
    {
        return;
    }
    DelegateVec::iterator it = std::find( pDelegates->begin(), pDelegates->end(), _spDelegate );
    if ( it != pDelegates->end() )
    {
        pDelegates->erase( it );
    }
}

template < class Enum, class RetType >
void CFoundation::Delegate0Invoker< Enum, RetType >::DispatchEvent( Enum _eEvent )
{
    DelegateVec* pDelegates = GetDelegates( _eEvent );
    if ( pDelegates == NULL )
    {
        return;
    }

    // Do Delegates
    DelegateVec aspDelegates = *pDelegates;
    for ( DelegateVec::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
    {
        SharedPtr< Delegate0< RetType > > spDelegate = *it;
        (*spDelegate)();
    }
}

template < class Enum, class Param1, class RetType >
void CFoundation::Delegate1Invoker< Enum, Param1, RetType >::DispatchEvent( Enum _eEvent, Param1 _param1 )
{
    Delegate1Base::DelegateVec* pDelegates = Delegate1Base::GetDelegates( _eEvent );
    if ( pDelegates != NULL )
    {
        // Do Delegates
        Delegate1Base::DelegateVec aspDelegates = *pDelegates;
        for ( std::vector< SharedPtr< Delegate1< Param1 > > >::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate1< Param1, RetType > > spDelegate = *it;
            (*spDelegate)( _param1 );
        }
    }

    Delegate0Base::DelegateVec* pDelegates0 = Delegate0Base::GetDelegates( _eEvent );
    if ( pDelegates0 != NULL )
    {
        // Do Delegates
        Delegate0Base::DelegateVec aspDelegates = *pDelegates0;
        for ( std::vector< SharedPtr< Delegate0<> > >::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate0< RetType > > spDelegate = *it;
            (*spDelegate)();
        }
    }
}

template < class Enum, class Param1, class Param2, class RetType >
void CFoundation::Delegate2Invoker< Enum, Param1, Param2, RetType >::DispatchEvent( Enum _eEvent, Param1 _param1, Param2 _param2 )
{
    Delegate2Base::DelegateVec* pDelegates2 = Delegate2Base::GetDelegates( _eEvent );
    if ( pDelegates2 != NULL )
    {
        // Do Delegates
        Delegate2Base::DelegateVec aspDelegates = *pDelegates2;
        for ( std::vector< SharedPtr< Delegate2< Param1, Param2 > > >::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate2< Param1, Param2, RetType > > spDelegate = *it;
            (*spDelegate)( _param1, _param2 );
        }
    }

    Delegate1Base::DelegateVec* pDelegates1 = Delegate1Base::GetDelegates( _eEvent );
    if ( pDelegates1 != NULL )
    {
        // Do Delegates
        Delegate1Base::DelegateVec aspDelegates = *pDelegates1;
        for ( std::vector< SharedPtr< Delegate1< Param1 > > >::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate1< Param1, RetType > > spDelegate = *it;
            (*spDelegate)( _param1 );
        }
    }

    Delegate0Base::DelegateVec* pDelegates0 = Delegate0Base::GetDelegates( _eEvent );
    if ( pDelegates0 != NULL )
    {
        // Do Delegates
        Delegate0Base::DelegateVec aspDelegates = *pDelegates0;
        for ( std::vector< SharedPtr< Delegate0<> > >::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate0< RetType > > spDelegate = *it;
            (*spDelegate)();
        }
    }
}

template < class Enum, class Param1, class Param2, class Param3, class RetType >
void CFoundation::Delegate3Invoker< Enum, Param1, Param2, Param3, RetType >::DispatchEvent( Enum _eEvent, Param1 _param1, Param2 _param2, Param3 _param3 )
{
    Delegate3Base::DelegateVec* pDelegates3 = Delegate3Base::GetDelegates( _eEvent );
    if ( pDelegates3 != NULL )
    {
        // Do Delegates
        Delegate3Base::DelegateVec aspDelegates = *pDelegates3;
        for ( Delegate3Base::DelegateVec::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate3< Param1, Param2, Param3, RetType > > spDelegate = *it;
            (*spDelegate)( _param1, _param2, _param3 );
        }
    }

    Delegate2Base::DelegateVec* pDelegates2 = Delegate2Base::GetDelegates( _eEvent );
    if ( pDelegates2 != NULL )
    {
        // Do Delegates
        Delegate2Base::DelegateVec aspDelegates = *pDelegates2;
        for ( Delegate2Base::DelegateVec::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate2< Param1, Param2, RetType > > spDelegate = *it;
            (*spDelegate)( _param1, _param2 );
        }
    }

    Delegate1Base::DelegateVec* pDelegates1 = Delegate1Base::GetDelegates( _eEvent );
    if ( pDelegates1 != NULL )
    {
        // Do Delegates
        Delegate1Base::DelegateVec aspDelegates = *pDelegates1;
        for ( Delegate1Base::DelegateVec::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate1< Param1, RetType > > spDelegate = *it;
            (*spDelegate)( _param1 );
        }
    }

    Delegate0Base::DelegateVec* pDelegates0 = Delegate0Base::GetDelegates( _eEvent );
    if ( pDelegates0 != NULL )
    {
        // Do Delegates
        Delegate0Base::DelegateVec aspDelegates = *pDelegates0;
        for ( Delegate0Base::DelegateVec::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate0< RetType > > spDelegate = *it;
            (*spDelegate)();
        }
    }
}

template < class Enum, class Param1, class Param2, class Param3, class Param4, class RetType >
void CFoundation::Delegate4Invoker< Enum, Param1, Param2, Param3, Param4, RetType >::DispatchEvent( Enum _eEvent, Param1 _param1, Param2 _param2, Param3 _param3, Param4 _param4 )
{
    Delegate4Base::DelegateVec* pDelegates4 = Delegate4Base::GetDelegates( _eEvent );
    if ( pDelegates4 != NULL )
    {
        // Do Delegates
        Delegate4Base::DelegateVec aspDelegates = *pDelegates4;
        for ( Delegate4Base::DelegateVec::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate4< Param1, Param2, Param3, Param4, RetType > > spDelegate = *it;
            (*spDelegate)( _param1, _param2, _param3, _param4 );
        }
    }

    Delegate3Base::DelegateVec* pDelegates3 = Delegate3Base::GetDelegates( _eEvent );
    if ( pDelegates3 != NULL )
    {
        // Do Delegates
        Delegate3Base::DelegateVec aspDelegates = *pDelegates3;
        for ( Delegate3Base::DelegateVec::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate3< Param1, Param2, Param3, RetType > > spDelegate = *it;
            (*spDelegate)( _param1, _param2, _param3 );
        }
    }

    Delegate2Base::DelegateVec* pDelegates2 = Delegate2Base::GetDelegates( _eEvent );
    if ( pDelegates2 != NULL )
    {
        // Do Delegates
        Delegate2Base::DelegateVec aspDelegates = *pDelegates2;
        for ( Delegate2Base::DelegateVec::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate2< Param1, Param2, RetType > > spDelegate = *it;
            (*spDelegate)( _param1, _param2 );
        }
    }

    Delegate1Base::DelegateVec* pDelegates1 = Delegate1Base::GetDelegates( _eEvent );
    if ( pDelegates1 != NULL )
    {
        // Do Delegates
        Delegate1Base::DelegateVec aspDelegates = *pDelegates1;
        for ( Delegate1Base::DelegateVec::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate1< Param1, RetType > > spDelegate = *it;
            (*spDelegate)( _param1 );
        }
    }

    Delegate0Base::DelegateVec* pDelegates0 = Delegate0Base::GetDelegates( _eEvent );
    if ( pDelegates0 != NULL )
    {
        // Do Delegates
        Delegate0Base::DelegateVec aspDelegates = *pDelegates0;
        for ( Delegate0Base::DelegateVec::iterator it = aspDelegates.begin(); it != aspDelegates.end(); ++it )
        {
            SharedPtr< Delegate0< RetType > > spDelegate = *it;
            (*spDelegate)();
        }
    }
}
