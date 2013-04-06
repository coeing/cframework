#ifndef CMADTV_CLIENT_SOCKET_H
#define CMADTV_CLIENT_SOCKET_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{    
    INCLUDE_SHARED_CLASS( BaseMessage );    
    INCLUDE_SHARED_CLASS( DelegateConnection );
    INCLUDE_SHARED_CLASS( MessageDispatcher );
    INCLUDE_SHARED_CLASS( MessageFactory );
    INCLUDE_SHARED_CLASS( SocketConnection );
    class String;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( DatabaseUpdateMsg );
    INCLUDE_SHARED_CLASS( StateMsg );
    INCLUDE_SHARED_CLASS( SerializationContext );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ClientSocket
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ClientSocket
    {
    public:
        /// Constructor
        ClientSocket();

        /// Destructor
        ~ClientSocket();

        /// Connects to server
        void            Connect( const CFoundation::String& _strHost, Unsigned32 _u32Port );

        /// Disconnects from server
        void            Disconnect();

        /// Sends the passed message over the connection
        void            Send( CFoundation::BaseMessagePtr _spMsg );

    protected:
        /// Message factory
        CFoundation::MessageFactoryPtr      m_spMsgFactory;
    
        /// TCP connection
        CFoundation::SocketConnectionPtr    m_spConnection;

        /// Serialization context
        SerializationContextPtr             m_spSerializationContext;

    private:
        /// Message handlers
        void            HandleDatabaseUpdate( DatabaseUpdateMsgPtr _spMsg );
        void            HandleStateUpdate( StateMsgPtr _spMsg );

        /// Handles messages
        void            HandleMessage( CFoundation::BaseMessagePtr _spMsg );

        /// Delegates
        CFoundation::DelegateConnectionPtr  m_spDelegateStateMsg;
    };
}

#endif
