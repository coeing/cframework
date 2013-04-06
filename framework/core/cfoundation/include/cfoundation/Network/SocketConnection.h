#ifndef CFOUNDATION_SOCKET_CONNECTION_H
#define CFOUNDATION_SOCKET_CONNECTION_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Events/MessageDispatcher.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( BaseMessage )
    INCLUDE_SHARED_CLASS( MessageFactory )
    INCLUDE_SHARED_CLASS( SerializationContext )

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// SocketConnection
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class SocketConnection : public MessageDispatcher
    {
    public:
        /// Constructor
        SocketConnection( MessageFactoryPtr _spMsgFactory );

        /// Connects to the passed socket
        virtual void        Connect( const char* _szHost, const char* _szPort );

        /// Indicates if the socket is connected
        bool                IsConnected() const;

        /// Disconnects the socket
        virtual void        Disconnect();

        /// Sets the serialization context that's used to serialize the messages
        virtual void        SetSerializationContext( SerializationContextPtr _spContext );

        /// Sends a message
        virtual void        Send( BaseMessagePtr _spMsg ) = 0;

    protected:
        /// Handles the sending of a message
        void                HandleSend( BaseMessagePtr _spMsg, const void*& _pBuffer, Unsigned32& _u32Size );

        /// Handles the receiving of a message
        void                HandleReceive( char* _pData, Unsigned32 _u32Size );

    private:
        /// Indicates if the socket is connected
        bool                m_bConnected;

        /// Serializer
        Serializer          m_SerializerSend;
        Serializer          m_SerializerReceive;

        /// Message factory
        MessageFactoryPtr   m_spMsgFactory;
    };
}

#endif
