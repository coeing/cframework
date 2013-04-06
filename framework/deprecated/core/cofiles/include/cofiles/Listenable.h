#ifndef COFILES_LISTENABLE_H
#define COFILES_LISTENABLE_H

#include "MessageReceiver.h"

namespace CoFiles
{
    /* DEPRECATED
	class IListenable
	{
	public:
		//! Adds an object which implements Message Handling to this objects listeners
		/*! From now on every message this object receives is send also
		 *  to the new listener.
		 *
		 *  \param clListener	The Object which wants to listen to this object
		 *
		 */
    /* DEPRECATED
		virtual void AddListener( MessageReceiver &rListener ) = 0;

		//! Removes an existing listener from this object
		/*! A listener which don't want to listen to this
		 *  object any more must call this method.
		 *
		 *  \param clListener	An existing Listener
		 *
		 */
    /* DEPRECATED
		virtual void RemoveListener( MessageReceiver &rListener ) = 0;

		virtual void SendToListeners( const Message &clMessage ) const = 0;
	};*/
};

#endif
