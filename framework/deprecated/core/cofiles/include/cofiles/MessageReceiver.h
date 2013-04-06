#ifndef COFILES_MESSAGE_RECEIVER_H
#define COFILES_MESSAGE_RECEIVER_H

//#include "cofiles/RCObject.h"
#include "cofiles/Utils/SharedPtr.h"

namespace CoFiles
{
    class Message;

	//! The base class to implement Message Sending and Receiving
	/*! If a class wants to receive Messages it has to inherit from
	 *  this class. Then it can register at a class which inherits
	 *  from a MessageObject class.
	 *
	 *  \author Christian Oeing
	 * 
	 *  \date	20.10.06
	 *
	 */
    class MessageReceiver : public SharedPtrBase< MessageReceiver >
	{
	public:
		virtual void ProcessMessage( const Message& _clMessage ) = 0;
	};

    typedef SharedPtr< MessageReceiver > MessageReceiverPtr;
    typedef SharedPtr< const MessageReceiver > MessageReceiverConstPtr;
}

#endif
