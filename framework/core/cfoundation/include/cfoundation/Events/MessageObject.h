#ifndef CFoundation_MESSAGEOBJECT_H
#define CFoundation_MESSAGEOBJECT_H

#include <set>
#include <map>

#include "cfoundation/Events/MessageReceiver.h"
#include "cfoundation/Base/Types.h"

namespace CFoundation
{
    class Message;

	//! Adds listener functionality to an Object
	/*! A object which can receive messages can register
	 *  to an MessageObject to listen to messages it receives.
	 *  Than all messages are forwarded also to the listener.
	 *  This can be very useful when screens want
	 *  to listen to their controls. Or Controls (like a sidebar) to their
	 *  sub-controls (the two buttons).
	 *
	 *  \author Christian Oeing
	 *
	 *  \date	20.10.06
	 *
	 */
	class MessageObject : public MessageReceiver
	{
	public:
		MessageObject();
		virtual ~MessageObject() = 0;

		//! Removes all listeners
		virtual void    Clear();

		//! Processes the messages by forward them to its listeners
		virtual void    ProcessMessage( const Message& _clMessage );

		//! Adds an object which implements Message Handling to this objects listeners
		/*! From now on every message this object receives is send also
		 *  to the new listener.
		 *
		 *  \param clListener	The Object which wants to listen to this object
		 *
		 */
		virtual void    AddListener( MessageReceiverPtr _spListener );

		//! Adds an object which implements Message Handling to this objects listeners
		/*! From now on every message with the passed ID this object 
		 *  receives is send also to the new listener.
		 *
		 *  \param _spListener      The object which wants to listen to this object
		 *  \param _u32MessageType  The type of the message the listener wants to listen to
		 *
		 */
		virtual void    AddListener( MessageReceiverPtr _spListener, Unsigned32 _u32MessageType );

		//! Removes an existing listener from this object
		/*! A listener which don't want to listen to this
		 *  object any more must call this method.
		 *
		 *  \param _spListener	An existing Listener
		 *
		 */
		virtual void    RemoveListener( MessageReceiverPtr _spListener );

		//! Removes an existing listener from this object
		/*! A listener which don't want to listen to this
		 *  object any more must call this method.
		 *
		 *  \param _spListener	    An existing Listener
		 *  \param _u32MessageType  The type of the message the listener listened to
		 *
		 */
		virtual void    RemoveListener( MessageReceiverPtr _spListener, Unsigned32 _u32MessageType );

        //! Sends the passed message
        virtual void    Send( const Message& _clMessage ) const;

        //! Sends the passed message to all listeners
		virtual void    SendToListeners( const Message& _clMessage ) const;

		//! Checks if the object sends messages of the passed ID
		virtual bool    IsMessageEnabled( Unsigned32 u32ID ) const;

	private:
	    typedef std::set< MessageReceiverPtr > ListenerSet;
        typedef std::map< Unsigned32, ListenerSet* > MessageListenerMap;

        //! Returns the list of listeners to the passed message
        ListenerSet*                GetListeners( Unsigned32 _u32MessageID );

        //! Returns the list of listeners to the passed message
        const ListenerSet*          GetListeners( Unsigned32 _u32MessageID ) const;

		//! The listeners of the object
		ListenerSet	                m_aListenersAll;

        //! The listeners to specific messages
        MessageListenerMap          m_aListeners;
	};
};

#endif
