#include "cmadtv/Data/Game/Game.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Betty/Present.h"
#include "cmadtv/Data/Game/Mission.h"
#include "cmadtv/Data/Game/Sammy/Sammy.h"
#include "cmadtv/Data/News/News.h"
#include "cmadtv/Data/News/NewsInstance.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Spectators/Transmitter.h"

#include "cmadtv/Data/World/AdvertDealer.h"
#include "cmadtv/Data/World/BroadcastDealer.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Game
///
////////////////////////////////////////////////////////////////////////////////////////////

Game::Game()
:   m_u32NextNewsInstanceID( 0 ),
    m_u32NextTransmitterID( 0 ),
    m_spBroadcastDealer( new BroadcastDealer() ),
    m_spAdvertDealer( new AdvertDealer() ),
    m_eState( STATE_PRE_GAME ),
    m_f32Speed( 60.0f ),
    m_StartDate( 1, 1, 2009 ),
    m_Date( 1, 1, 2009 ),
    m_Time( 0, 0, 0 ),
    m_f32Secs( 0.0f )
{
}
 
#define SHARED_PTR_COPY( spDest, spSrc )    if ( spDest == NULL || spSrc == NULL ) {    \
                                                spDest = spSrc; }                       \
                                            else {                                      \
                                                *spDest = *spSrc; }

#define SHARED_PTR_COPY_VEC( spDestVec, spSrcVec )  spDestVec.resize( spSrcVec.size() );    \
                                                    for ( Unsigned32 u32Idx = 0; u32Idx < spSrcVec.size(); ++u32Idx ) { \
                                                        SHARED_PTR_COPY( spDestVec[ u32Idx ], spSrcVec[ u32Idx ] ); }

void Game::operator=( const Game& _rhs )
{
    SetState( _rhs.m_eState );
    m_StartDate = _rhs.m_StartDate;
    m_f32Secs = _rhs.m_f32Secs;
    m_f32Speed = _rhs.m_f32Speed;
    SHARED_PTR_COPY( m_spBroadcastDealer, _rhs.m_spBroadcastDealer );
    SHARED_PTR_COPY( m_spAdvertDealer, _rhs.m_spAdvertDealer );
    SHARED_PTR_COPY( m_spMission, _rhs.m_spMission );
    SHARED_PTR_COPY_VEC( m_aspUnusedNews, _rhs.m_aspUnusedNews );
    m_u32NextNewsInstanceID = _rhs.m_u32NextNewsInstanceID;
    SHARED_PTR_COPY_VEC( m_aspNewsInstances, _rhs.m_aspNewsInstances );
    m_u32NextTransmitterID = _rhs.m_u32NextTransmitterID;
    SHARED_PTR_COPY_VEC( m_aspTransmitters, _rhs.m_aspTransmitters );
    SHARED_PTR_COPY( m_spSammy, _rhs.m_spSammy );
    SHARED_PTR_COPY( m_spNextSammy, _rhs.m_spNextSammy );

    SHARED_PTR_COPY_VEC( m_aspPlayers, _rhs.m_aspPlayers );

    SetDate( _rhs.m_Date );
    SetTime( _rhs.m_Time );

    /*    
    /// Present counters
    PresentCounterMap               m_mapPresentCounters;*/

}
              
void Game::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 3 );

    _Serializer.SerializeEnum( m_eState );
    m_Date.Serialize( _Serializer );
    m_Time.Serialize( _Serializer );
    _Serializer.Serialize( m_f32Secs );
    _Serializer.Serialize( m_f32Speed );

    // Serialize broadcast dealer
    m_spBroadcastDealer->Serialize( _Serializer );

    // Serialize advert dealer
    m_spAdvertDealer->Serialize( _Serializer );

    // Serialzize news
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_Context >( m_aspUnusedNews );
    _Serializer.Serialize( m_u32NextNewsInstanceID );
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_SharedPtrSimple >( m_aspNewsInstances );

    // Serialize transmitters
    _Serializer.Serialize( m_u32NextTransmitterID );
    _Serializer.SerializeVector< CFoundation::SerializeWrapper_SharedPtrSimple >( m_aspTransmitters );

    if ( _Serializer.IsSaving() )
    {
        Unsigned32 u32NumPlayers = m_aspPlayers.size();
        _Serializer.Add( u32NumPlayers );
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
        {
            PlayerPtr spPlayer = m_aspPlayers[ u32Idx ];

            // Serialize player
            spPlayer->Serialize( _Serializer );
        }
    }
    else
    {
        // Clear old data
        m_aspPlayers.clear();

        Unsigned32 u32NumPlayers = _Serializer.GetU32();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
        {
            // Create player
            PlayerPtr spPlayer( new Player() );
            spPlayer->Init( GetSharedPtr() );

            // Serialize player
            spPlayer->Serialize( _Serializer );

            // Add player
            m_aspPlayers.push_back( spPlayer );
        }
    }

    if ( u32Version >= 2 )
    {
        _Serializer.SerializeMap< CFoundation::SerializeWrapper_Context, CFoundation::SerializeWrapper_Value >( m_mapPresentCounters );
    }

    bool bMission = m_spMission != NULL;
    _Serializer.Serialize( bMission );
    if ( bMission )
    {
        if ( _Serializer.IsLoading() )
        {
            m_spMission = MissionPtr( new Mission() );
        }
        m_spMission->Serialize( _Serializer );
    }

    if ( u32Version >= 3 )
    {
        // Serialize sammy
        _Serializer.SerializeProxy( *this, m_spSammy );
        _Serializer.SerializeProxy( *this, m_spNextSammy );
    }
}

void Game::Reset()
{
    m_Date.SetDate( 1, 1, 2009 );
    m_Time.SetTime( 17, 0, 0 );
    m_StartDate = m_Date; 
}
       
CFoundation::DateTime Game::GetDateTime() const
{
    return CFoundation::DateTime( m_Date, m_Time );
}

const CFoundation::Date& Game::GetDate() const
{
    return m_Date;
}
       
const CFoundation::Time& Game::GetTime() const
{
    return m_Time;
}
        
Unsigned32 Game::GetDays() const
{
    return static_cast< Unsigned32 >( GetDate() - m_StartDate );
}
        
Hour Game::GetHour( const CFoundation::Time& _Time )
{
    Unsigned8 u8Hour = _Time.GetHour();
    if ( u8Hour >= 18 )
    {
        return static_cast< Hour >( u8Hour + HOUR_BEGIN - 18 );
    }
    else
    {
        return HOUR_NONE;
    }
}
        
Hour Game::GetHour() const
{
    return GetHour( m_Time );
}

HourStage Game::GetHourStage( const CFoundation::Time& _Time )
{
    Unsigned8 u8Minute = _Time.GetMinute();
    if ( u8Minute < 5 )
    {
        return HOUR_STAGE_NEWS;
    }
    else if ( u8Minute < 55 )
    {
        return HOUR_STAGE_BROADCAST;
    }
    else if ( u8Minute < 60 )
    {
        return HOUR_STAGE_ADVERT;
    }
    else
    {
        return HOUR_STAGE_NONE;
    }
}
        
HourStage Game::GetHourStage() const
{
    return GetHourStage( m_Time );
}
        
void Game::SetUnusedTime( Float32 _f32UnusedSecs )
{
    m_f32Secs = _f32UnusedSecs;
}
        
Float32 Game::GetUnusedTime() const
{
    return m_f32Secs;
}
        
void Game::SetSpeed( Float32 _f32Speed )
{
    m_f32Speed = _f32Speed;
}
        
Float32 Game::GetSpeed() const
{
    return m_f32Speed;
}
        
void Game::AddPlayer( PlayerPtr _spPlayer )
{
    m_aspPlayers.push_back( _spPlayer );
}
        
Unsigned32 Game::GetNumPlayers() const
{
    return m_aspPlayers.size();
}

PlayerPtr Game::GetPlayer( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < m_aspPlayers.size() )
    {
        return m_aspPlayers[ _u32Idx ];
    }
    else
    {
        return PlayerPtr();
    }
}
        
Unsigned32 Game::GetPlayerIdx( PlayerPtr _spPlayer ) const
{
    PlayerVec::const_iterator it = std::find( m_aspPlayers.begin(), m_aspPlayers.end(), _spPlayer );
    if ( it != m_aspPlayers.end() )
    {
        return it - m_aspPlayers.begin();
    }
    else
    {
        return MAX_U32;
    }
}
        
const std::vector< PlayerPtr >& Game::GetPlayers() const
{
    return m_aspPlayers;
}
        
void Game::IncreasePresentCounter( PresentPtr _spPresent )
{
    ++m_mapPresentCounters[ _spPresent ];
}

Unsigned32 Game::GetPresentCounter( PresentPtr _spPresent ) const
{
    PresentCounterMap::const_iterator it = m_mapPresentCounters.find( _spPresent );
    if ( it != m_mapPresentCounters.end() )
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}

Unsigned8 Game::ComputePresentLoveIncrease( PresentPtr _spPresent ) const
{
    Float32 f32PriceAmount = _spPresent->GetPrice().GetAmount( CFoundation::CURRENCY_DOLLAR );
    
    // TODO Present counter should influence increase

    return static_cast< Unsigned8 >( f32PriceAmount * 0.00009 );
}
       
void Game::SetMission( MissionPtr _spMission )
{
    m_spMission = _spMission;
}

MissionPtr Game::GetMission() const
{
    return m_spMission;
}
        
BroadcastDealerPtr Game::GetBroadcastDealer()
{
    return m_spBroadcastDealer;
}
        
AdvertDealerPtr Game::GetAdvertDealer()
{
    return m_spAdvertDealer;
}
   
NewsInstancePtr Game::CreateNewsInstance()
{
    // Create news instance
    NewsInstancePtr spNewsInstance( new NewsInstance( m_u32NextNewsInstanceID++ ) );
    m_aspNewsInstances.push_back( spNewsInstance );
    return spNewsInstance;
}
       
void Game::SetUnusedNews( const std::vector< NewsPtr >& _aspUnusedNews )
{
    m_aspUnusedNews = _aspUnusedNews;
}
       
std::vector< NewsPtr >& Game::GetUnusedNews()
{
    return m_aspUnusedNews;
}
        
const std::vector< NewsPtr >& Game::GetUnusedNews() const
{
    return m_aspUnusedNews;
}
 
NewsInstancePtr Game::GetNewsInstance( Unsigned32 _u32ID ) const
{
    if ( _u32ID < m_aspNewsInstances.size() )
    {
        return m_aspNewsInstances[ _u32ID ];
    }
    else
    {
        return NewsInstancePtr();
    }
}
        
TransmitterPtr Game::CreateTransmitter()
{
    TransmitterPtr spTransmitter( new Transmitter( m_u32NextTransmitterID++ ) );
    m_aspTransmitters.push_back( spTransmitter );
    return spTransmitter;
}

TransmitterPtr Game::GetTransmitter( Unsigned32 _u32ID ) const
{
    if ( _u32ID < m_aspTransmitters.size() )
    {
        return m_aspTransmitters[ _u32ID ];
    }
    else
    {
        return TransmitterPtr();
    }
}
        
void Game::SetSammy( SammyPtr _spSammy )
{
    m_spSammy = _spSammy;
}
        
SammyPtr Game::GetSammy() const
{
    return m_spSammy;
}
        
void Game::SetNextSammy( SammyPtr _spSammy )
{
    m_spNextSammy = _spSammy;
}

SammyPtr Game::GetNextSammy() const
{
    return m_spNextSammy;
}

void Game::Serialize( CFoundation::Serializer& _Serializer, SammyPtr& _spSammy ) const
{
    bool bSammy = _spSammy != NULL;
    _Serializer.Serialize( bSammy );
    if ( bSammy )
    {
        Sammy::Type eType = _spSammy != NULL ? _spSammy->GetType() : Sammy::TYPE_NONE;
        _Serializer.SerializeEnum( eType );

        if ( _Serializer.IsLoading() )
        {
            // Create sammy
            _spSammy = Sammy::Create( eType );
            _spSammy->SetGame( GetSharedPtr() );
        }

        // Serialize sammy
        _spSammy->Serialize( _Serializer );
    }
    else
    {
        _spSammy.reset();
    }
}
     
void Game::SetState( State _eState )
{
    if ( _eState == m_eState )
    {
        return;
    }

    m_eState = _eState;
}
        
Game::State Game::GetState() const
{
    return m_eState;
}
        
void Game::SetDate( const CFoundation::Date& _Date )
{
    if ( _Date == m_Date )
    {
        return;
    }

    m_Date = _Date;

    // Dispatch event
    DispatchEvent( EVENT_GAME_DATE_CHANGED, GetSharedPtr() );
}

void Game::SetTime( const CFoundation::Time& _Time )
{
    // We're not interested if the seconds change
    if ( _Time.GetHour() == m_Time.GetHour() &&
         _Time.GetMinute() == m_Time.GetMinute() )
    {
        return;
    }

    CFoundation::Time oldTime = m_Time;
    m_Time = _Time;

    // Dispatch event
    DispatchEvent( EVENT_GAME_TIME_CHANGED, GetSharedPtr() );

    // Check if hour changed
    if ( GetHour( m_Time ) != GetHour( oldTime ) )
    {        
        // Dispatch event
        DispatchEvent( EVENT_GAME_HOUR_CHANGED, GetSharedPtr() );
    }

    // Check if hour stage changed
    if ( GetHourStage( m_Time ) != GetHourStage( oldTime ) )
    {       
        // Dispatch event
        DispatchEvent( EVENT_GAME_HOUR_STAGE_CHANGED, GetSharedPtr() );
    }    
}
