#ifndef CFOUNDATION_MESSAGE_FACTORY_H
#define CFOUNDATION_MESSAGE_FACTORY_H

#include "cfoundation/Events/Message.h"

namespace CFoundation
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// MessageFactory
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class MessageFactory
    {
    public:
        /// Creates a message of the passed type
        virtual BaseMessagePtr  Create( const String& _strType ) = 0;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// PluggableMessageFactory
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// A factory pattern that allows to add new messages without changing existing code

    template< class KeyType >
    class PluggableMessageFactory : public MessageFactory
    {
    public:
        class Maker
        {
        public:
            /// Constructor
            Maker( const KeyType& _Key );

            virtual BaseMessagePtr  OnCreate() const = 0;
        };

        /// Creates a message of the passed type
        virtual BaseMessagePtr  Create( const KeyType& _strType );

    private:
        typedef std::map< KeyType, Maker* > MessageMap;
        static MessageMap       m_mapRegistry;
    };
};

#include "cfoundation/Events/MessageFactory.inl"

#endif
