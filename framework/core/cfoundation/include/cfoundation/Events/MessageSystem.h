#ifndef CFoundation_MESSAGESYSTEM_H
#define CFoundation_MESSAGESYSTEM_H

#include <list>

#include "cfoundation/Events/Message.h"
#include "cfoundation/Utils/Singleton.h"

namespace CFoundation
{
    class MessageSystem : public Singleton< MessageSystem >
	{
	public:
        /// Deinitialization
        virtual void                    Deinitialize();

		//! Processes the next open message
		/*!
         *
		 *	\returns	true if a message was open and handled, false else
		 */
		bool                            ProcessMessage();

        //! Sends a message
		void                            Send( const Message &clMessage );

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
