#ifndef COFILES_MESSAGESYSTEM_H
#define COFILES_MESSAGESYSTEM_H

#include <list>

#include "cofiles/Message.h"
#include "cofiles/Singleton.h"

namespace CoFiles
{
    class MessageSystem : public Singleton< MessageSystem >
	{
	public:
		//! Processes the next open message
		/*!
         *
		 *	\returns	true if a message was open and handled, false else
		 */
		bool                            ProcessMessage();

        //! Sends a message
		void                            Send( const CoFiles::Message &clMessage );

        /// Returns the number of left messages
        Unsigned32                      GetNumMessages() const;

        //! Informs the message system that a message receiver has been deleted
        void                            DeletedReceiver( MessageReceiverConstPtr _spMessageReceiver );

    private:
        //! Messages to send
		std::list< Message* >       m_pMessages;
	};
}

#endif
