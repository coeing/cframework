#include "cmadtv/Data/Serialization/SerializationContext.h"

#include <algorithm>

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/XML/XMLParser.h"

#include "csystem/Files/File.h"

#include "cmadtv/Data/Advert.h"
#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Genre.h"
#include "cmadtv/Data/Betty/Present.h"
#include "cmadtv/Data/News/News.h"
#include "cmadtv/Data/News/NewsChannel.h"
#include "cmadtv/Data/News/NewsInstance.h"
#include "cmadtv/Data/Spectators/Map.h"
#include "cmadtv/Data/Spectators/Transmitter.h"
#include "cmadtv/Data/World/Country.h"

#include "cmadtv/Data/Network/NetworkMessages.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// SerializationContext
///
////////////////////////////////////////////////////////////////////////////////////////////    
SerializationContext::SerializationContext( DatabasePtr _spDatabase, GamePtr _spGame )
:   m_spDatabase( _spDatabase ),
    m_spGame( _spGame )   
{
    // Register serialization methods
    RegisterSerializeMethod( CFoundation::CreateDelegate( this, &SerializationContext::SerializeDatabaseUpdateMsg ) );
    RegisterSerializeMethod( CFoundation::CreateDelegate( this, &SerializationContext::SerializeAdvert ) );
    RegisterSerializeMethod( CFoundation::CreateDelegate( this, &SerializationContext::SerializeBroadcast ) );
    RegisterSerializeMethod( CFoundation::CreateDelegate( this, &SerializationContext::SerializeGenre ) );
    RegisterSerializeMethod( CFoundation::CreateDelegate( this, &SerializationContext::SerializeNews ) );
    RegisterSerializeMethod( CFoundation::CreateDelegate( this, &SerializationContext::SerializeNewsChannel ) );
    RegisterSerializeMethod( CFoundation::CreateDelegate( this, &SerializationContext::SerializeNewsInstance ) );
    RegisterSerializeMethod( CFoundation::CreateDelegate( this, &SerializationContext::SerializePresent ) );
    RegisterSerializeMethod( CFoundation::CreateDelegate( this, &SerializationContext::SerializeStateUpdateMsg ) );
    RegisterSerializeMethod( CFoundation::CreateDelegate( this, &SerializationContext::SerializeTransmitter ) );
}

void SerializationContext::SetDatabase( DatabasePtr _spDatabase )
{
    m_spDatabase = _spDatabase;
}

void SerializationContext::SetGame( GamePtr _spGame )
{
    m_spGame = _spGame;
}
       
void SerializationContext::SerializeDatabaseUpdateMsg( CFoundation::Serializer& _Serializer, DatabaseUpdateMsgPtr& _spMsg )
{
    if ( _Serializer.IsLoading() )
    {
        _spMsg = DatabaseUpdateMsgPtr( new DatabaseUpdateMsg() );
    }

    DatabasePtr spOriginalDatabase = m_spDatabase;
    DatabasePtr& spDatabase = _spMsg->m_Database;
    bool bPresent = spDatabase != NULL;
    _Serializer.Serialize( bPresent );
    if ( bPresent )
    {
        if ( _Serializer.IsLoading() )
        {
            spDatabase = DatabasePtr( new Database() );
            m_spDatabase = spDatabase;
        }
        spDatabase->Serialize( _Serializer );
    }
    m_spDatabase = spOriginalDatabase;
}
       
void SerializationContext::SerializeStateUpdateMsg( CFoundation::Serializer& _Serializer, StateMsgPtr& _spMsg )
{
    if ( _Serializer.IsLoading() )
    {
        _spMsg = StateMsgPtr( new StateMsg() );
    }

    //GamePtr spOriginalGame = m_spGame;
    GamePtr& spGame = _spMsg->m_Game;
    bool bPresent = spGame != NULL;
    _Serializer.Serialize( bPresent );
    if ( bPresent )
    {
        if ( _Serializer.IsLoading() )
        {
            spGame = GamePtr( new Game() );
            if ( m_spGame == NULL )
            {
                m_spGame = spGame;
            }
        }
        spGame->Serialize( _Serializer );
    }
    //m_spGame = spOriginalGame;
}

void SerializationContext::SerializeGenre( CFoundation::Serializer& _Serializer, GenrePtr& _spGenre )
{
    if ( _Serializer.IsSaving() )
    {
        _Serializer.Add( _spGenre != NULL ? _spGenre->GetID() : MAX_U32 );
    }
    else
    {
        Unsigned32 u32ID = _Serializer.GetU32();
        _spGenre = u32ID != MAX_U32 && m_spDatabase != NULL ? m_spDatabase->GetGenre( u32ID ) : GenrePtr();
    }
}

void SerializationContext::SerializeBroadcast( CFoundation::Serializer& _Serializer, BroadcastPtr& _spBroadcast )
{
    if ( _Serializer.IsSaving() )
    {
        _Serializer.Add( _spBroadcast != NULL ? _spBroadcast->GetID() : MAX_U32 );
    }
    else
    {
        Unsigned32 u32ID = _Serializer.GetU32();
        _spBroadcast = u32ID != MAX_U32 && m_spDatabase != NULL ? m_spDatabase->GetBroadcast( u32ID ) : BroadcastPtr();
    }
}
    
void SerializationContext::SerializeAdvert( CFoundation::Serializer& _Serializer, AdvertPtr& _spAdvert )
{
    if ( _Serializer.IsSaving() )
    {
        _Serializer.Add( _spAdvert != NULL ? _spAdvert->GetID() : MAX_U32 );
    }
    else
    {
        Unsigned32 u32ID = _Serializer.GetU32();
        _spAdvert = u32ID != MAX_U32 && m_spDatabase != NULL ? m_spDatabase->GetAdvert( u32ID ) : AdvertPtr();
    }
}
   
void SerializationContext::SerializeNewsChannel( CFoundation::Serializer& _Serializer, NewsChannelPtr& _spNewsChannel )
{
    if ( _Serializer.IsSaving() )
    {
        _Serializer.Add( _spNewsChannel != NULL ? _spNewsChannel->GetID() : MAX_U32 );
    }
    else
    {
        Unsigned32 u32ID = _Serializer.GetU32();
        _spNewsChannel = u32ID != MAX_U32 && m_spDatabase != NULL ? m_spDatabase->GetNewsChannel( u32ID ) : NewsChannelPtr();
    }
}
      
void SerializationContext::SerializeNews( CFoundation::Serializer& _Serializer, NewsPtr& _spNews )
{
    if ( _Serializer.IsSaving() )
    {
        _Serializer.Add( _spNews != NULL ? _spNews->GetID() : MAX_U32 );
    }
    else
    {
        Unsigned32 u32ID = _Serializer.GetU32();
        _spNews = u32ID != MAX_U32 && m_spDatabase != NULL ? m_spDatabase->GetNews( u32ID ) : NewsPtr();
    }
}
      
void SerializationContext::SerializeNewsInstance( CFoundation::Serializer& _Serializer, NewsInstancePtr& _spNewsInstance )
{
    if ( _Serializer.IsSaving() )
    {
        _Serializer.Add( _spNewsInstance != NULL ? _spNewsInstance->GetID() : MAX_U32 );
    }
    else
    {
        Unsigned32 u32ID = _Serializer.GetU32();
        _spNewsInstance = u32ID != MAX_U32 && m_spGame != NULL ? m_spGame->GetNewsInstance( u32ID ) : NewsInstancePtr();
    }
}
            
void SerializationContext::SerializeTransmitter( CFoundation::Serializer& _Serializer, TransmitterPtr& _spTransmitter )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    // Serialize type
    Transmitter::Type eType = _spTransmitter != NULL ? _spTransmitter->GetType() : Transmitter::TYPE_NONE;
    _Serializer.SerializeEnum( eType );

    switch ( eType )
    {
    case Transmitter::TYPE_ANTENNA:
        {
            Unsigned32 u32ID = _spTransmitter != NULL ? _spTransmitter->GetID() : MAX_U32;
            _Serializer.Serialize( u32ID );
            if ( _Serializer.IsLoading() )
            {
                _spTransmitter = u32ID != MAX_U32 && m_spGame != NULL ? m_spGame->GetTransmitter( u32ID ) : TransmitterPtr();
            }
        }
        break;
    case Transmitter::TYPE_SATELLITE:
        {
            // Get satellite index
            Unsigned32 u32SatelliteIdx = MAX_U32;
            if ( m_spDatabase != NULL )
            {
                u32SatelliteIdx = m_spDatabase->GetSatelliteIdx( _spTransmitter );
            }
            _Serializer.Serialize( u32SatelliteIdx );

            _spTransmitter = u32SatelliteIdx != MAX_U32 && m_spDatabase != NULL ? m_spDatabase->GetSatellite( u32SatelliteIdx ) : TransmitterPtr();
        }
        break;
    }
}

void SerializationContext::SerializePresent( CFoundation::Serializer& _Serializer, PresentPtr& _spPresent )
{
    if ( _Serializer.IsSaving() )
    {
        _Serializer.Add( _spPresent != NULL ? _spPresent->GetID() : MAX_U32 );
    }
    else
    {
        Unsigned32 u32ID = _Serializer.GetU32();
        _spPresent = u32ID != MAX_U32 && m_spDatabase != NULL ? m_spDatabase->GetPresent( u32ID ) : PresentPtr();
    }
}
