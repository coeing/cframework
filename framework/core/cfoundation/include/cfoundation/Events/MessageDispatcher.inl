#include <algorithm>

namespace CFoundation
{     
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// MessageDispatcher
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////
    template<>
    SharedPtr< DelegateConnection > MessageDispatcher::RegisterDelegate( SharedPtr< Delegate2< MessageDispatcherPtr, SharedPtr< BaseMessage > > > _spDelegate );

    template<>
    SharedPtr< DelegateConnection > MessageDispatcher::RegisterDelegate( SharedPtr< Delegate1< SharedPtr< BaseMessage > > > _spDelegate );

    template<>
    void MessageDispatcher::RemoveDelegate( SharedPtr< Delegate2< MessageDispatcherPtr, SharedPtr< BaseMessage > > > _spDelegate );
    	
    template<>
    void MessageDispatcher::RemoveDelegate( SharedPtr< Delegate1< SharedPtr< BaseMessage > > > _spDelegate );

    template< class Message >
    SharedPtr< DelegateConnection > MessageDispatcher::RegisterDelegate( SharedPtr< Delegate2< MessageDispatcherPtr, SharedPtr< Message > > > _spDelegate )
    {
        RTTItypeid type_id = STATIC_TYPE_INFO( Message );
        DelegateVec* pDelegates = GetDelegates2( type_id );
        if ( pDelegates == NULL )
        {
            pDelegates = new DelegateVec();
            m_mapDelegates2[ type_id ] = pDelegates;
        }
	    pDelegates->push_back( _spDelegate );

        return SharedPtr< DelegateConnection >( new DelegateMessageDispatcherConnection< Message >( GetSharedPtr(), _spDelegate ) );
    }

    template< class Message >
    SharedPtr< DelegateConnection > MessageDispatcher::RegisterDelegate( SharedPtr< Delegate1< SharedPtr< Message > > > _spDelegate )
    {
        RTTItypeid type_id = STATIC_TYPE_INFO( Message );
        DelegateVec* pDelegates = GetDelegates1( type_id );
        if ( pDelegates == NULL )
        {
            pDelegates = new DelegateVec();
            m_mapDelegates1[ type_id ] = pDelegates;
        }
	    pDelegates->push_back( _spDelegate );

        return SharedPtr< DelegateConnection >( new DelegateMessageDispatcherConnection< Message >( GetSharedPtr(), _spDelegate ) );
    }
		
    template< class Message >
    void MessageDispatcher::RemoveDelegate( SharedPtr< Delegate2< MessageDispatcherPtr, SharedPtr< Message > > > _spDelegate )
    {
        if ( _spDelegate == NULL )
        {
            return;
        }

        RTTItypeid type_id = STATIC_TYPE_INFO( Message );
        MessageDelegatesMap::iterator itVec = m_mapDelegates2.find( type_id );
        if ( itVec == m_mapDelegates2.end() )
        {
            return;
        }
        DelegateVec* pDelegates = itVec->second;
        DelegateVec::iterator it = std::find( pDelegates->begin(), pDelegates->end(), _spDelegate );
        if ( it == pDelegates->end() )
        {
            return;
        }
        pDelegates->erase( it );

        // Check if empty
        if ( pDelegates->empty() )
        {
            delete pDelegates;
            m_mapDelegates2.erase( itVec );
        }
    }
		
    template< class Message >
    void MessageDispatcher::RemoveDelegate( SharedPtr< Delegate1< SharedPtr< Message > > > _spDelegate )
    {
        if ( _spDelegate == NULL )
        {
            return;
        }

        RTTItypeid type_id = STATIC_TYPE_INFO( Message );
        MessageDelegatesMap::iterator itVec = m_mapDelegates1.find( type_id );
        if ( itVec == m_mapDelegates1.end() )
        {
            return;
        }
        DelegateVec* pDelegates = itVec->second;
        DelegateVec::iterator it = std::find( pDelegates->begin(), pDelegates->end(), _spDelegate );
        if ( it == pDelegates->end() )
        {
            return;
        }
        pDelegates->erase( it );

        // Check if empty
        if ( pDelegates->empty() )
        {
            delete pDelegates;
            m_mapDelegates1.erase( itVec );
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// DelegateMessageDispatcherConnection
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    template< class Message >
    DelegateMessageDispatcherConnection< Message >::DelegateMessageDispatcherConnection( MessageDispatcherPtr _spMessageDispatcher, SharedPtr< Delegate2< MessageDispatcherPtr, SharedPtr< Message > > > _spDelegate )
    :   m_spMessageDispatcher( _spMessageDispatcher ),
        m_spDelegate2( _spDelegate )
    {
    }

    template< class Message >
    DelegateMessageDispatcherConnection< Message >::DelegateMessageDispatcherConnection( MessageDispatcherPtr _spMessageDispatcher, SharedPtr< Delegate1< SharedPtr< Message > > > _spDelegate )
    :   m_spMessageDispatcher( _spMessageDispatcher ),
        m_spDelegate1( _spDelegate )
    {
    }

    template< class Message >
    DelegateMessageDispatcherConnection< Message >::~DelegateMessageDispatcherConnection()
    {
    }
        
    template< class Message >
    void DelegateMessageDispatcherConnection< Message >::Disconnect()
    {
        if ( m_spDelegate2 != NULL )
        {
            m_spMessageDispatcher->RemoveDelegate( m_spDelegate2 );
        }
        else
        {
            m_spMessageDispatcher->RemoveDelegate( m_spDelegate1 );
        }
    }
}
