#ifndef CFOUNDATION_MESSAGE_QUEUE_H
#define CFOUNDATION_MESSAGE_QUEUE_H

#include "cfoundation/Events/Message.h"

namespace CFoundation
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// MessageQueue
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class MessageQueue
	{
	public:
        /// Sends a message
		virtual void                    Send( BaseMessagePtr _spMsg ) = 0;
        
        /// Clears all messages
        virtual void                    Clear() = 0;

        /// Receives a message
        /// Blocks until a message is available
        virtual BaseMessagePtr          Receive() = 0;

        /// Tries to receive a message
        /// If queue is empty the method returns a NULL pointer
        virtual BaseMessagePtr          TryReceive() = 0;

        /// Returns the number of left messages
        virtual Unsigned32              GetNumMessages() const = 0;
	};
}

#endif
