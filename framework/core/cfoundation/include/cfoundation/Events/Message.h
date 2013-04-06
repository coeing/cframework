#ifndef CFOUNDATION_MESSAGE_H
#define CFOUNDATION_MESSAGE_H

#include "cfoundation/Events/MessageObject.h"
#include "cfoundation/RTTI/rtti.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Serializer;

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// MessageHeader
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class MessageHeader
    {
    public:
        /// Constructor
        MessageHeader()
        :   m_u32MsgSize( 0 )
        {
        }

        /// Message size
        Unsigned32      m_u32MsgSize;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BaseMessage
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BaseMessage
    {
    public:
        /// Serialization
        virtual void    Serialize( Serializer& _Serializer ) = 0;

    private:
        TYPE_DATA
    };

    INCLUDE_SHARED_CLASS( BaseMessage )

    class Message
    {
    public:
        //! Constructor
		Message();

        //! Destructor
		virtual ~Message();

        //! Clone this message
        virtual Message*    Clone() const = 0;

		virtual void        SetSender( MessageReceiverConstPtr _spSender );
		virtual void        SetReceiver( MessageReceiverConstPtr _spReceiver );

		virtual void        SendToReceiver();

		virtual bool        IsBroadcast() const;

		virtual bool        IsSender( MessageReceiverConstPtr _spObject ) const;
		virtual bool        IsReceiver( MessageReceiverConstPtr _spObject ) const;

		virtual void        SetHandled( bool bHandled = true ) const;
		virtual bool        IsHandled() const;

		virtual bool        IsMessage( Unsigned32 _u32Type ) const;
        
		virtual Unsigned32  GetType() const = 0;

        //! Tries to cast this message to
        //! the passed class
		template < class T >
		const T* CastTo() const
		{
            if ( GetType() == T::GetType() )
            {
                return static_cast< const T* >( this );
            }
            else
            {
                return NULL;
            }
		}

	private:
		mutable bool			    m_bHandled;

        //! Sender of this message
		MessageReceiverConstPtr     m_spSender;

        //! Receiver of this message
		MessageReceiverConstPtr     m_spReceiver;
    };

	//! The base class for Messages which can be sended between Nodes
	/*!
	 *
	 *  This class represents the base message which has only an ID,
	 *  a sender and a receiver. Every message has to be derived from
	 *  this class.
	 *
	 *  \author Christian Oeing
	 *
	 *  \date 20.10.06
	 *
	 */
    template < class T >
    class MessageTemplate : public Message
	{
	public:
		//! Clones this message
        virtual Message*    Clone() const
		{
			return new T( *static_cast< const T* >( this ) );
		}

        //! Returns the type of the message
        //! Derived classes have to implement
        //! a static GetType() method
		virtual Unsigned32  GetType() const
        {
            return T::GetType();
        }
	};
};

#endif
