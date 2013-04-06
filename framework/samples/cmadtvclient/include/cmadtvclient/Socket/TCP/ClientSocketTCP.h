#ifndef CMADTV_CLIENT_SOCKET_TCP_H
#define CMADTV_CLIENT_SOCKET_TCP_H

#include <boost/asio.hpp>

#include "cfoundation/Base/Types.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cmadtvclient/Socket/ClientSocket.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( BaseMessage )
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( StateMsg );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// ClientSocketTCP
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class ClientSocketTCP : public ClientSocket
    {
    public:
        /// Constructor
        ClientSocketTCP( boost::asio::io_service& _IOService );

        /// Destructor
        ~ClientSocketTCP();

    private:
    };
}

#endif
