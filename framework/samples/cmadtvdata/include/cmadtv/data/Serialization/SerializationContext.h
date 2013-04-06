#ifndef CMADTV_SERIALIZATION_CONTEXT_H
#define CMADTV_SERIALIZATION_CONTEXT_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Math/Vector.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Money;
    class Serializer;
    INCLUDE_SHARED_CLASS( XMLElement );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );
    INCLUDE_SHARED_CLASS( Broadcast );
    INCLUDE_SHARED_CLASS( Database );
    INCLUDE_SHARED_CLASS( DatabaseUpdateMsg );
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( Genre );
    INCLUDE_SHARED_CLASS( News );
    INCLUDE_SHARED_CLASS( NewsChannel );
    INCLUDE_SHARED_CLASS( NewsInstance );
    INCLUDE_SHARED_CLASS( Present );
    INCLUDE_SHARED_CLASS( StateMsg );
    INCLUDE_SHARED_CLASS( Transmitter );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// SerializationContext
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class SerializationContext : public CFoundation::SerializationContext
    {
    public:
        /// Constructor
        explicit SerializationContext( DatabasePtr _spDatabase = DatabasePtr(), GamePtr _spGame = GamePtr() );

        /// Sets the database
        void                SetDatabase( DatabasePtr _spDatabase );

        /// Set the game
        void                SetGame( GamePtr _spGame );

        /////////////////////////////////////////////////////////////////
        // Messages
        /////////////////////////////////////////////////////////////////

        /// Serializes a database update msg pointer
        void                SerializeDatabaseUpdateMsg( CFoundation::Serializer& _Serializer, DatabaseUpdateMsgPtr& _spMsg );
        void                SerializeStateUpdateMsg( CFoundation::Serializer& _Serializer, StateMsgPtr& _spMsg );

        /////////////////////////////////////////////////////////////////
        // Genre
        /////////////////////////////////////////////////////////////////

        /// Serializes a genre pointer
        void                SerializeGenre( CFoundation::Serializer& _Serializer, GenrePtr& _spGenre );

        /////////////////////////////////////////////////////////////////
        // Broadcasts
        /////////////////////////////////////////////////////////////////

        /// Serializes a broadcast pointer
        void                SerializeBroadcast( CFoundation::Serializer& _Serializer, BroadcastPtr& _spBroadcast );

        /////////////////////////////////////////////////////////////////
        // Adverts
        /////////////////////////////////////////////////////////////////

        /// Serializes an advert pointer
        void                SerializeAdvert( CFoundation::Serializer& _Serializer, AdvertPtr& _spAdvert );

        /////////////////////////////////////////////////////////////////
        // News
        /////////////////////////////////////////////////////////////////

        /// Serializes a news pointer
        void                SerializeNews( CFoundation::Serializer& _Serializer, NewsPtr& _spNews );
        void                SerializeNewsChannel( CFoundation::Serializer& _Serializer, NewsChannelPtr& _spNewsChannel );
        void                SerializeNewsInstance( CFoundation::Serializer& _Serializer, NewsInstancePtr& _spNewsInstance );

        /////////////////////////////////////////////////////////////////
        // Spectators
        /////////////////////////////////////////////////////////////////

        /// Serializes a transmitter pointer
        void                SerializeTransmitter( CFoundation::Serializer& _Serializer, TransmitterPtr& _spTransmitter );

        /////////////////////////////////////////////////////////////////
        // Presents
        /////////////////////////////////////////////////////////////////

        /// Serializes a present pointer
        void                SerializePresent( CFoundation::Serializer& _Serializer, PresentPtr& _spPresent );

    private:
        /// Database
        DatabasePtr         m_spDatabase;

        /// Game
        GamePtr             m_spGame;
    };

    INCLUDE_SHARED_CLASS( SerializationContext );
}

#endif
