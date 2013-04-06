#include "cfoundation/Events/MessageDispatcher.h"

using namespace CFoundation;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// MessageDispatcher
///
////////////////////////////////////////////////////////////////////////////////////////////

MessageDispatcher::MessageDispatcher()
{
}

MessageDispatcher::~MessageDispatcher()
{
    Clear();
}

void MessageDispatcher::Clear()
{
    for ( MessageDelegatesMap::iterator it = m_mapDelegates1.begin(); it != m_mapDelegates1.end(); ++it )
    {
        DelegateVec* pDelegates = it->second;
        if ( pDelegates != NULL )
        {
            delete pDelegates;
            pDelegates = NULL;
        }
    }
    m_mapDelegates1.clear();

    for ( MessageDelegatesMap::iterator it = m_mapDelegates2.begin(); it != m_mapDelegates2.end(); ++it )
    {
        DelegateVec* pDelegates = it->second;
        if ( pDelegates != NULL )
        {
            delete pDelegates;
            pDelegates = NULL;
        }
    }
    m_mapDelegates2.clear();
}

void MessageDispatcher::Dispatch( BaseMessagePtr _spMsg )
{
    if ( _spMsg == NULL )
    {
        return;
    }

    RTTItypeid type_id = TYPE_INFO( _spMsg.get() );
    const DelegateVec* pDelegates1 = GetDelegates1( type_id );
    const DelegateVec* pDelegates2 = GetDelegates2( type_id );

    // Send to delegates
    for ( DelegateVec::const_iterator it = m_aDelegates1.begin(); it != m_aDelegates1.end(); ++it )
    {
        VoidPtr spDelegate = *it;
        BaseMessageDelegate1* pDelegate = static_cast< BaseMessageDelegate1* >( spDelegate.get() );
        (*pDelegate)( _spMsg );
    }
    for ( DelegateVec::const_iterator it = m_aDelegates2.begin(); it != m_aDelegates2.end(); ++it )
    {
        VoidPtr spDelegate = *it;
        BaseMessageDelegate2* pDelegate = static_cast< BaseMessageDelegate2* >( spDelegate.get() );
        (*pDelegate)( GetSharedPtr(), _spMsg );
    }
        
    if ( pDelegates1 != NULL )
    {
        for ( DelegateVec::const_iterator it = pDelegates1->begin(); it != pDelegates1->end(); ++it )
        {
            VoidPtr spDelegate = *it;
            BaseMessageDelegate1* pDelegate = static_cast< BaseMessageDelegate1* >( spDelegate.get() );
            (*pDelegate)( _spMsg );
        }
    }
    if ( pDelegates2 != NULL )
    {
        for ( DelegateVec::const_iterator it = pDelegates2->begin(); it != pDelegates2->end(); ++it )
        {
            VoidPtr spDelegate = *it;
            BaseMessageDelegate2* pDelegate = static_cast< BaseMessageDelegate2* >( spDelegate.get() );
            (*pDelegate)( GetSharedPtr(), _spMsg );
        }
    }
}

template<>
SharedPtr< DelegateConnection > MessageDispatcher::RegisterDelegate( SharedPtr< Delegate2< MessageDispatcherPtr, SharedPtr< BaseMessage > > > _spDelegate )
{
    m_aDelegates2.push_back( _spDelegate );
    return SharedPtr< DelegateConnection >( new DelegateMessageDispatcherConnection< BaseMessage >( GetSharedPtr(), _spDelegate ) );
}

template<>
SharedPtr< DelegateConnection > MessageDispatcher::RegisterDelegate( SharedPtr< Delegate1< SharedPtr< BaseMessage > > > _spDelegate )
{
    m_aDelegates1.push_back( _spDelegate );
    return SharedPtr< DelegateConnection >( new DelegateMessageDispatcherConnection< BaseMessage >( GetSharedPtr(), _spDelegate ) );
}
	
template<>
void MessageDispatcher::RemoveDelegate( SharedPtr< Delegate2< MessageDispatcherPtr, SharedPtr< BaseMessage > > > _spDelegate )
{
    DelegateVec::iterator it = std::find( m_aDelegates2.begin(), m_aDelegates2.end(), _spDelegate );
    if ( it == m_aDelegates2.end() )
    {
        return;
    }
    m_aDelegates2.erase( it );
}
	
template<>
void MessageDispatcher::RemoveDelegate( SharedPtr< Delegate1< SharedPtr< BaseMessage > > > _spDelegate )
{
    DelegateVec::iterator it = std::find( m_aDelegates1.begin(), m_aDelegates1.end(), _spDelegate );
    if ( it == m_aDelegates1.end() )
    {
        return;
    }
    m_aDelegates1.erase( it );
}

MessageDispatcher::DelegateVec* MessageDispatcher::GetDelegates1( RTTItypeid _Type )
{
    MessageDelegatesMap::iterator it = m_mapDelegates1.find( _Type );
    if ( it != m_mapDelegates1.end() )
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}

const MessageDispatcher::DelegateVec* MessageDispatcher::GetDelegates1( RTTItypeid _Type ) const
{
    return const_cast< MessageDispatcher* >( this )->GetDelegates1( _Type );
}

MessageDispatcher::DelegateVec* MessageDispatcher::GetDelegates2( RTTItypeid _Type )
{
    MessageDelegatesMap::iterator it = m_mapDelegates2.find( _Type );
    if ( it != m_mapDelegates2.end() )
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}

const MessageDispatcher::DelegateVec* MessageDispatcher::GetDelegates2( RTTItypeid _Type ) const
{
    return const_cast< MessageDispatcher* >( this )->GetDelegates2( _Type );
}
