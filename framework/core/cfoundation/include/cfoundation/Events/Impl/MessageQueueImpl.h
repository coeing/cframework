#ifndef CFOUNDATION_MESSAGE_QUEUE_IMPL_H
#define CFOUNDATION_MESSAGE_QUEUE_IMPL_H

#include <list>

#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>

#include "cfoundation/Events/MessageQueue.h"

namespace CFoundation
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// MessageQueueImpl
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// This is a thread-safe implementation of a message queue

    class MessageQueueImpl : public MessageQueue
	{
	public:
        /// Sends a message
		virtual void                    Send( BaseMessagePtr _spMsg );
        
        /// Clears all messages
        virtual void                    Clear();

        /// Receives a message
        /// Blocks until a message is available
        virtual BaseMessagePtr          Receive();

        /// Tries to receive a message
        /// If queue is empty the method returns a NULL pointer
        virtual BaseMessagePtr          TryReceive();

        /// Returns the number of left messages
        virtual Unsigned32              GetNumMessages() const;

    private:
        /// Messages to send
		std::list< BaseMessagePtr >     m_aspMessages;

        /// Mutex to make message queue thread-safe
        boost::mutex                    m_Mutex;

        /// Condition to inform threads that are waiting to receive a message
        boost::condition                m_CondReceive;
	};
}

#endif
