#ifndef CFOUNDATION_MESSAGE_DISPATCHER_H
#define CFOUNDATION_MESSAGE_DISPATCHER_H

#include <map>
#include <vector>

#include "cfoundation/Events/Delegate.h"
#include "cfoundation/Events/Message.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( BaseMessage );
    INCLUDE_SHARED_CLASS( MessageDispatcher );
    typedef Delegate2< MessageDispatcherPtr, BaseMessagePtr > BaseMessageDelegate2;
    typedef Delegate1< BaseMessagePtr > BaseMessageDelegate1;
    typedef SharedPtr< BaseMessageDelegate2 > BaseMessageDelegate2Ptr;
    typedef SharedPtr< BaseMessageDelegate1 > BaseMessageDelegate1Ptr;

    template< class Message >
    class DelegateMessageDispatcherConnection;

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// MessageDispatcher
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

	/*! A object can register a delegate
	 *  to an MessageDispatcher to listen to messages it receives.
	 *  Than all messages of the registered type are forwarded also to the listener.
	 *
	 *  \author Christian Oeing
	 *
	 *  \date	16.10.10
	 *
	 */

    class MessageDispatcher : public SharedPtrBase< MessageDispatcher >
	{
	public:
        /// Constructor
		MessageDispatcher();
        
        /// Destructor
        ~MessageDispatcher();

		/// Removes all listeners
		virtual void                    Clear();

        ////////////////////////////////////////////////////////////////////////////////////////////
        ///
        /// DelegateMessageDispatcherConnection
        ///
        ////////////////////////////////////////////////////////////////////////////////////////////

		/// Adds an object which implements Message Handling to this objects listeners
		/*! From now on every message with the passed type this object 
		 *  receives is send also to the new listener.
		 *
		 *  \param _spDelegate      The delegate which wants to listen to this object
		 *
		 */
        template < class Message >
        SharedPtr< DelegateConnection > RegisterDelegate( SharedPtr< Delegate2< MessageDispatcherPtr, SharedPtr< Message > > > _spDelegate );

        template < class Message >
        SharedPtr< DelegateConnection > RegisterDelegate( SharedPtr< Delegate1< SharedPtr< Message > > > _spDelegate );

		/// Removes an existing delegate from this object
		/*! A delegate which don't want to listen to this
		 *  object any more must call this method.
         *  If the delegate itself shouldn't be stored the user should use the DelegateConnection object that was returned
         *  by RegisterDelegate to disconnect the delegate.
		 *
		 *  \param _spDelegate	An existing delegate
		 *
		 */
        template < class Message >
        void                            RemoveDelegate( SharedPtr< Delegate2< MessageDispatcherPtr, SharedPtr< Message > > > _spDelegate );

        template < class Message >
        void                            RemoveDelegate( SharedPtr< Delegate1< SharedPtr< Message > > > _spDelegate );

        /// Dispatches the passed message to all delegates
		virtual void                    Dispatch( BaseMessagePtr _spMsg );

	private:
        typedef SharedPtr< void > VoidPtr;
	    typedef std::vector< VoidPtr > DelegateVec;
        typedef std::map< RTTItypeid, DelegateVec* > MessageDelegatesMap;

        /// Returns the vector of delegates to the passed message
        DelegateVec*                    GetDelegates1( RTTItypeid _Type );

        /// Returns the vector of delegates to the passed message
        const DelegateVec*              GetDelegates1( RTTItypeid _Type ) const;

        /// Returns the vector of delegates to the passed message
        DelegateVec*                    GetDelegates2( RTTItypeid _Type );

        /// Returns the vector of delegates to the passed message
        const DelegateVec*              GetDelegates2( RTTItypeid _Type ) const;

        /// Delegates to all messages
        DelegateVec                     m_aDelegates1;
        DelegateVec                     m_aDelegates2;

        /// Delegates to specific messages
        MessageDelegatesMap             m_mapDelegates1;
        MessageDelegatesMap             m_mapDelegates2;
	};

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// DelegateMessageDispatcherConnection
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Returned when registering a delegate on a message dispatcher
    /// Could be used to disconnect the delegate

    template< class Message >
    class DelegateMessageDispatcherConnection : public DelegateConnection
    {
    public:
        typedef SharedPtr< Delegate2< MessageDispatcherPtr, SharedPtr< Message > > > Delegate2Ptr;
        typedef SharedPtr< Delegate1< SharedPtr< Message > > > Delegate1Ptr;

        /// Constructor
        DelegateMessageDispatcherConnection( MessageDispatcherPtr _spMessageDispatcher, Delegate2Ptr _spDelegate );

        /// Constructor
        DelegateMessageDispatcherConnection( MessageDispatcherPtr _spMessageDispatcher, Delegate1Ptr _spDelegate );

        /// Destructor
        ~DelegateMessageDispatcherConnection();

        /// Disconnects the delegate from the invoker
        void                            Disconnect();

    private:
        /// Message dispatcher
        MessageDispatcherPtr            m_spMessageDispatcher;

        /// Delegates
        Delegate2Ptr                    m_spDelegate2;
        Delegate1Ptr                    m_spDelegate1;
    };
};

#include "cfoundation/Events/MessageDispatcher.inl"

#endif
