#include "cmadtv/Logic/Game/GameLogic.h"

#include "cfoundation/Math/Random.h"
#include "cfoundation/Math/Utils.h"

#include "cmadtv/Data/Advert.h"
#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Game/GameSettings.h"
#include "cmadtv/Data/Game/Mission.h"
#include "cmadtv/Data/Game/Sammy/Sammy.h"
#include "cmadtv/Data/Genre.h"
#include "cmadtv/Data/News/News.h"
#include "cmadtv/Data/News/NewsChannel.h"
#include "cmadtv/Data/News/NewsInstance.h"
#include "cmadtv/Data/Player/AdvertContract.h"
#include "cmadtv/Data/Player/BlockStat.h"
#include "cmadtv/Data/Player/DayStat.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerCompNews.h"
#include "cmadtv/Data/Player/PlayerCompSpectators.h"
#include "cmadtv/Data/Player/PlayerCompStats.h"
#include "cmadtv/Data/Player/Schedule.h"
#include "cmadtv/Data/Player/Scheduling.h"
#include "cmadtv/Data/Spectators/Transmitter.h"
#include "cmadtv/Data/World/AdvertDealer.h"
#include "cmadtv/Data/World/BroadcastDealer.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// GameLogic
///
////////////////////////////////////////////////////////////////////////////////////////////
typedef std::vector< PlayerPtr > PlayerVec;
 
void GameLogic::Setup( DatabasePtr _spDatabase, GamePtr _spGame )
{
    m_spDatabase = _spDatabase;
    m_spGame = _spGame;
}

void GameLogic::StartGame( const GameSettings& _Settings )
{
    // Set mission
    MissionPtr spMission = _Settings.GetMission();
    m_spGame->SetMission( spMission );

    // Get initial antennas
    const TransmitterVec& aspInitialAntennas = m_spDatabase->GetInitialAntennas();

    // Create players
    Unsigned32 u32NumPlayers = _Settings.GetNumPlayers();
    Unsigned8 u8InitialImage = 0;
    Unsigned8 u8ImageLeftOver = 0;
    if ( u32NumPlayers > 0 )
    {
        u8InitialImage = static_cast< Unsigned8 >( 100 / u32NumPlayers );
        u8ImageLeftOver = 100 % u32NumPlayers;
    }
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        // Create player
        PlayerPtr spPlayer( new Player() );
        spPlayer->Init( m_spGame );

        // Set name and color
        spPlayer->SetName( _Settings.GetPlayerName( u32Idx ) );
        spPlayer->SetColor( _Settings.GetPlayerColor( u32Idx ) );

        // Add some start money
        CFoundation::Money startCredit( 400000, CFoundation::CURRENCY_DOLLAR );
        CFoundation::DateTime prevGameDateTime = m_spGame->GetDateTime();
        prevGameDateTime.AddSeconds( -1 );
        spPlayer->GetBudget().Book( ACCOUNT_SUNDRY_INCOME, ACCOUNT_CASH, startCredit, prevGameDateTime );
        spPlayer->GetBudget().AddCredit( startCredit );

        // Add initial antenna
        if ( u32Idx < aspInitialAntennas.size() )
        {
            TransmitterPtr spTransmitter = m_spGame->CreateTransmitter();
            spTransmitter->Init( Transmitter::TYPE_ANTENNA, aspInitialAntennas[ u32Idx ]->GetX(), aspInitialAntennas[ u32Idx ]->GetY() );
            spPlayer->GetCompSpectators()->AddTransmitter( spTransmitter );
        }

        // Subscribe to first news channel
        spPlayer->GetCompNews()->AddChannel( m_spDatabase->GetNewsChannel( 0 ) );

        // Set as active player if human
        Unsigned8 u8Image = u8InitialImage;
        if ( _Settings.IsHumanPlayer( u32Idx ) )
        {
            u8Image += u8ImageLeftOver; // Human player gets a bit more image on startup

            // Initialize mission
            if ( spMission != NULL )
            {
                spMission->SetPlayer( spPlayer );
            }
        }

        // Set image
        spPlayer->SetImage( u8Image );

        // Add player
        m_spGame->AddPlayer( spPlayer );
    }  

    // Reset
    m_spGame->Reset(); 

    // Dispatch event
    m_spGame->DispatchEvent( WORLD_EVENT_GAME_STARTED, m_spGame );

    // Start new day
    StartDay();
}     
       
void GameLogic::UpdateGame( Float32 _f32TimeDifference )
{
    if ( m_spGame->GetState() == Game::STATE_PRE_GAME )
    {
        return;
    }

    // Add minutes
    Float32 f32Secs = m_spGame->GetUnusedTime();
    f32Secs += _f32TimeDifference * m_spGame->GetSpeed();
    Unsigned32 u32Minutes = static_cast< Unsigned32 >( f32Secs / 60.0f );
    for ( Unsigned32 u32Idx = 0; u32Idx < u32Minutes; ++u32Idx )
    {
        UpdateMinute();
    }
    f32Secs -= u32Minutes * 60.0;
    m_spGame->SetUnusedTime( f32Secs );
}

void GameLogic::UpdateMinute()
{
    CFoundation::Time newTime = m_spGame->GetTime();
    newTime.AddMinutes( 1 );
    m_spGame->SetTime( newTime );

    // Check if check is necessary
    Unsigned8 u8Hour = newTime.GetHour();
    Hour eHour = m_spGame->GetHour();
    Unsigned8 u8Minute = newTime.GetMinute();
    switch ( m_spGame->GetState() )
    {
    case Game::STATE_PRE_BROADCASTING:
        {
            if ( u8Hour == 18 )
            {
                StartNews( eHour );

                m_spGame->SetState( Game::STATE_NEWS );

                // Dispatch event
                m_spGame->DispatchEvent( WORLD_EVENT_HOUR_STAGE_CHANGED, m_spGame );

                // Dispatch event
                m_spGame->DispatchEvent( WORLD_EVENT_HOUR_CHANGED, m_spGame );
            }
        }
        break;
    case Game::STATE_NEWS:
        {
            if ( u8Minute == 5 )
            {
                // Do transition to broadcast
                StartBroadcasts( eHour );

                m_spGame->SetState( Game::STATE_BROADCAST );

                // Dispatch event
                m_spGame->DispatchEvent( WORLD_EVENT_HOUR_STAGE_CHANGED, m_spGame );
            }
        }
        break;
    case Game::STATE_BROADCAST:
        {
            if ( u8Minute == 55 )
            {
                // Do transition to advert
                StartAdverts( eHour );

                m_spGame->SetState( Game::STATE_ADVERT );

                // Dispatch event
                m_spGame->DispatchEvent( WORLD_EVENT_HOUR_STAGE_CHANGED, m_spGame );
            }
        }
        break;
    case Game::STATE_ADVERT:
        {
            if ( u8Minute == 0 )
            {
                if ( u8Hour == 25 )
                {
                    // End day
                    EndDay();

                    // Start new day
                    StartDay();
                }
                else
                {
                    // Do transition to news
                    StartNews( eHour );
    
                    m_spGame->SetState( Game::STATE_NEWS );
                }

                // Dispatch event
                m_spGame->DispatchEvent( WORLD_EVENT_HOUR_STAGE_CHANGED, m_spGame );

                // Dispatch event
                m_spGame->DispatchEvent( WORLD_EVENT_HOUR_CHANGED, m_spGame );
            }
        }
        break;
    }

    // Check if mission succeeded or failed
    MissionPtr spMission = m_spGame->GetMission();
    if ( spMission != NULL )
    {
        if ( spMission->IsSuccessful() )
        {
            // End game
            m_spGame->SetState( Game::STATE_PRE_GAME );

            // Dispatch event
            m_spGame->DispatchEvent( WORLD_EVENT_GAME_WON, m_spGame );
        }
        if ( spMission->IsFailed() )
        {
            // End game
            m_spGame->SetState( Game::STATE_PRE_GAME );

            // Dispatch event
            m_spGame->DispatchEvent( WORLD_EVENT_GAME_LOST, m_spGame );
        }
    }
}

void GameLogic::StartDay()
{
    // Set time back
    m_spGame->SetTime( CFoundation::Time( 17, 0, 0 ) );

    // Update broadcasts
    Unsigned32 u32NumBroadcasts = m_spDatabase->GetNumBroadcasts();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBroadcasts; ++u32Idx )
    {
        BroadcastPtr spBroadcast = m_spDatabase->GetBroadcast( u32Idx );
        if ( spBroadcast == NULL )
        {
            continue;
        }

        // Compute up2dateness gain
        Unsigned8 u8Up2dateGain = static_cast< Unsigned8 >( 0.15f * 100 );

        // Update up2dateness
        Unsigned8 u8Up2date = std::min< Unsigned8 >( 100, spBroadcast->GetUp2date() + u8Up2dateGain );
        spBroadcast->SetUp2date( u8Up2date ); 
    }

    // Create new news
    Unsigned32 u32NumNewNews = CFoundation::Random::GetU32( 3, 6 );
    CreateNews( u32NumNewNews );

    // Get passed days
    Unsigned32 u32PassedDays = m_spGame->GetDays();
    
    Unsigned32 u32NumPlayers = m_spGame->GetNumPlayers();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerPtr spPlayer = m_spGame->GetPlayer( u32Idx );

        if ( u32PassedDays <= 1 )
        {
            continue;
        }

        // Pay interest
        CFoundation::Money interest = spPlayer->GetBudget().GetCredit() * 0.05f;
        spPlayer->GetBudget().Book( ACCOUNT_CASH, ACCOUNT_INTEREST, interest, m_spGame->GetDateTime(), "Interest" );

        PlayerCompNewsPtr spCompNews = spPlayer->GetCompNews();
        if ( spCompNews != NULL )
        {
            // Pay news channels
            const std::vector< NewsChannelPtr >& aspChannels = spCompNews->GetChannels();
            Unsigned32 u32NumChannels = aspChannels.size();
            for ( Unsigned32 u32Idx = 0; u32Idx < u32NumChannels; ++u32Idx )
            {
                NewsChannelPtr spChannel = aspChannels[ u32Idx ];
                spPlayer->GetBudget().Book( ACCOUNT_CASH, ACCOUNT_NEWS_COSTS, spChannel->GetPrice(), m_spGame->GetDateTime() );
            }

            // Remove old news
            std::vector< NewsInstancePtr > aspNews = spCompNews->GetNews();
            Unsigned32 u32NumNews = aspNews.size();
            for ( Unsigned32 u32Idx = 0; u32Idx < u32NumNews; ++u32Idx )
            {
                NewsInstancePtr spNews = aspNews[ u32Idx ];
                if ( spNews->GetDateTime().GetDate() - m_spGame->GetDate() < 0 )
                {
                    spCompNews->RemoveNews( spNews );
                }
            }
        }

        PlayerCompSpectatorsPtr spCompSpectators = spPlayer->GetCompSpectators();
        
        // Pay transmitter fees
        const std::vector< TransmitterPtr >& aspTransmitters = spCompSpectators->GetTransmitters();
        Unsigned32 u32NumTransmitters = aspTransmitters.size();
        spPlayer->GetBudget().Book( ACCOUNT_CASH, ACCOUNT_TRANSMITTER_FEES, u32NumTransmitters * CFoundation::Money( 20000, CFoundation::CURRENCY_DOLLAR ), m_spGame->GetDateTime() );
    }

    // Start sammy
    SammyPtr spNextSammy = m_spGame->GetNextSammy();
    m_spGame->SetSammy( spNextSammy );

    // Check if to start sammy tomorrow
    Unsigned32 u32Type = CFoundation::Random::GetU32( 0, Sammy::TYPE_END );
    spNextSammy = Sammy::Create( static_cast< Sammy::Type >( u32Type ) );
    if ( spNextSammy != NULL )
    {
        spNextSammy->SetGame( m_spGame );
    }
    m_spGame->SetNextSammy( spNextSammy );

    // Fill up broadcast dealer
    FillBroadcastDealer( true );

    // Fill up advert dealer
    FillAdvertDealer( true );

    // Dispatch event
    m_spGame->DispatchEvent( WORLD_EVENT_DAY_STARTED, m_spGame );

    m_spGame->SetState( Game::STATE_PRE_BROADCASTING );
}

void GameLogic::StartNews( Hour _eHour )
{
    const PlayerVec& aspPlayers = m_spGame->GetPlayers();
    Unsigned32 u32NumPlayers = aspPlayers.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerPtr spPlayer = aspPlayers[ u32Idx ];
        
        // Compute rating
        Float32 f32Rating = ComputeNewsRating( spPlayer, _eHour );

        // Compute spectators
        Unsigned32 u32NumSpectators = static_cast< Unsigned32 >( f32Rating * spPlayer->GetCompSpectators()->GetMaxNumSpectators( m_spDatabase->GetMap() ) );

        // Update statistics
        PlayerCompStatsPtr spCompStats = spPlayer->GetCompStats();
        BlockStatPtr spStat = spCompStats->GetDayStat( m_spGame->GetDate() )->GetBlockStat( _eHour );
        spStat->SetNewsRating( f32Rating );
        spStat->SetNewsSpectators( u32NumSpectators );
    }

    // Create new news
    Unsigned32 u32NumNewNews = CFoundation::Random::GetU32( 0, 5 );
    CreateNews( u32NumNewNews );

    // Inform sammy
    SAFE_CALL( m_spGame->GetSammy() )->OnNewsStarted( m_spGame->GetDate(), _eHour );
}

void GameLogic::StartBroadcasts( Hour _eHour )
{
    if ( _eHour < HOUR_BEGIN )
    {
        CF_WARN( "Game::StartBroadcasts: Invalid hour." );
        return;
    }

    // Get current date
    const CFoundation::Date& date = m_spGame->GetDate();

    // Collect best and worst ratings
    std::vector< PlayerRating > aPlayerRatings;

    const PlayerVec& aspPlayers = m_spGame->GetPlayers();
    Unsigned32 u32NumPlayers = aspPlayers.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerPtr spPlayer = aspPlayers[ u32Idx ];

        // Get schedule
        SchedulePtr spSchedule = spPlayer->GetScheduling()->GetSchedule( SCHEDULE_DAY_TODAY );

        // Get current broadcast
        BroadcastPtr spBroadcast = spSchedule->GetBroadcast( _eHour );
        
        // Compute rating
        Float32 f32Rating = ComputeRating( spPlayer, spBroadcast, date, _eHour );

        // Compute spectators
        Unsigned32 u32NumSpectators = static_cast< Unsigned32 >( f32Rating * spPlayer->GetCompSpectators()->GetMaxNumSpectators( m_spDatabase->GetMap() ) );

        // Update statistic for this hour
        PlayerCompStatsPtr spCompStats = spPlayer->GetCompStats();
        BlockStatPtr spStat = spCompStats->GetDayStat( m_spGame->GetDate() )->GetBlockStat( _eHour );
        spStat->SetBroadcast( spBroadcast );
        spStat->SetBroadcastRating( f32Rating );
        spStat->SetBroadcastSpectators( u32NumSpectators );
        spStat->SetBroadcastOrder( spSchedule->GetBroadcastBlockNumber( _eHour ) );

        // Finalize broadcast if last block
        if ( spSchedule->DoesBroadcastEnd( _eHour ) )
        {
            // Get average rating
            Float32 f32AvgRating = ComputeAvgRating( spPlayer, spBroadcast, date, _eHour );

            // Finalize
            FinalizeBroadcast( spPlayer, spBroadcast, f32AvgRating ); 
        }

        // Add rating
        aPlayerRatings.push_back( PlayerRating( spPlayer, f32Rating ) );
    }

    // Move one image point
    MoveImage( aPlayerRatings, 1 );

    // Set block image effects
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        const PlayerRating& rating = aPlayerRatings[ u32Idx ];
        PlayerPtr spPlayer = rating.m_spPlayer;        

        if ( rating.m_i8ImageChange < 0 )
        {
            // Set block stat image effect
            spPlayer->GetCompStats()->GetDayStat( date )->GetBlockStat( _eHour )->SetImageEffect( BlockStat::IMAGE_EFFECT_DECREASE );
        }
        else if ( rating.m_i8ImageChange > 0 )
        {
            // Set block stat image effect
            spPlayer->GetCompStats()->GetDayStat( date )->GetBlockStat( _eHour )->SetImageEffect( BlockStat::IMAGE_EFFECT_INCREASE );
        }
    }

    // Inform sammy
    SAFE_CALL( m_spGame->GetSammy() )->OnBroadcastsStarted( date, _eHour );
}

void GameLogic::StartAdverts( Hour _eHour )
{
    if ( _eHour < HOUR_BEGIN )
    {
        CF_WARN( "Game::StartAdverts: Invalid hour." );
        return;
    }

    const PlayerVec& aspPlayers = m_spGame->GetPlayers();
    Unsigned32 u32NumPlayers = aspPlayers.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerPtr spPlayer = aspPlayers[ u32Idx ];

        // Get schedule
        SchedulingPtr spScheduling = spPlayer->GetScheduling();
        SchedulePtr spSchedule = spScheduling->GetSchedule( SCHEDULE_DAY_TODAY );

        // Get current advert
        AdvertPtr spAdvert = spSchedule->GetAdvert( _eHour );
        if ( spAdvert == NULL )
        {
            continue;
        }

        // Get contract
        AdvertContractPtr spContract = spPlayer->GetAdvertContracts().GetContract( spAdvert );
        if ( spContract == NULL )
        {
            CF_WARN( "Sended advert that player had no contract for." );
            continue;
        }

        // Get statistic for this hour
        PlayerCompStatsPtr spCompStats = spPlayer->GetCompStats();
        BlockStatPtr spStat = spCompStats->GetDayStat( m_spGame->GetDate() )->GetBlockStat( _eHour );

        // Check advert
        bool bSuccess = CheckAdvert( spAdvert, spStat->GetBroadcast(), spStat->GetBroadcastSpectators() );
        if ( bSuccess )
        {
            // Remove one spot
            Unsigned32 u32NumSpotsLeft = spContract->RemoveSpot();

            // Check if contract succeeded
            if ( u32NumSpotsLeft == 0 )
            {
                FinalizeAdvert( spPlayer, spAdvert, true );
                
                // Remove contract
                spPlayer->GetAdvertContracts().RemoveContract( spContract );

                // Remove all adverts after the this hour
                Hour eNextHour = _eHour; ++eNextHour;
                spScheduling->RemoveAdverts( spAdvert, SCHEDULE_DAY_TODAY, eNextHour );
            }
        }

        // Update statistics for this hour
        spStat->SetAdvert( spAdvert );
        spStat->SetAdvertSuccessful( bSuccess );
        spStat->SetAdvertOrder( spContract->GetNumSpotsSent() - 1 );

        // Dispatch event
        spScheduling->DispatchEvent( SCHEDULING_EVENT_ADVERT_SENT, spAdvert, bSuccess );
    }
}

void GameLogic::EndDay()
{
    const CFoundation::Date& date = m_spGame->GetDate();

    const PlayerVec& aspPlayers = m_spGame->GetPlayers();
    Unsigned32 u32NumPlayers = aspPlayers.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerPtr spPlayer = aspPlayers[ u32Idx ];

        AdvertContracts& clContracts = spPlayer->GetAdvertContracts();

        // Remove outdated advert contracts
        std::vector< AdvertContractPtr > aspOutdatedContracts;
        Unsigned32 u32NumAdverts = clContracts.GetNumContracts();
        for ( Unsigned32 u32AdvertIdx = 0; u32AdvertIdx < u32NumAdverts; ++u32AdvertIdx )
        {
            AdvertContractPtr spContract = clContracts.GetContract( u32AdvertIdx );
            if ( spContract->GetNumDaysLeft( date ) == 0 )
            {
                aspOutdatedContracts.push_back( spContract );
            }
        }

        Unsigned32 u32NumOutdatedContracts = aspOutdatedContracts.size();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumOutdatedContracts; ++u32Idx )
        {
            AdvertContractPtr spContract = aspOutdatedContracts[ u32Idx ];

            // Finalize advert
            FinalizeAdvert( spPlayer, spContract->GetAdvert(), false );

            // Remove contract
            clContracts.RemoveContract( spContract );
        }

        // Copy schedule from tomorrow to today
        // and clear tomorrow schedule
        SchedulingPtr spScheduling = spPlayer->GetScheduling();
        spScheduling->SetSchedule( SCHEDULE_DAY_TODAY, spScheduling->GetSchedule( SCHEDULE_DAY_TOMORROW ) );
        spScheduling->SetSchedule( SCHEDULE_DAY_TOMORROW, SchedulePtr( new Schedule() ) );
    }

    // End current sammy
    SammyPtr spSammy = m_spGame->GetSammy();
    if ( spSammy != NULL )
    {
        EndSammy();
    }

    // Dispatch event
    m_spGame->DispatchEvent( WORLD_EVENT_DAY_ENDED, m_spGame );

    // Increase date
    CFoundation::Date newDate = m_spGame->GetDate();
    newDate.AddDays( 1 );
    m_spGame->SetDate( newDate );
}         

void GameLogic::CollectFreeBroadcasts( BroadcastVec& _aspBroadcasts ) const
{
    // Get broadcast dealer
    BroadcastDealerPtr spBroadcastDealer = m_spGame->GetBroadcastDealer();

    // Get all available broadcasts
    const BroadcastVec& aspBroadcasts = m_spDatabase->GetBroadcasts();

    // Get all player broadcasts
    BroadcastVec aspPlayerBroadcasts;
    CollectPlayerBroadcasts( aspPlayerBroadcasts );

    // Check all broadcasts
    Unsigned32 u32NumBroadcasts = aspBroadcasts.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBroadcasts; ++u32Idx )
    {
        BroadcastPtr spBroadcast = aspBroadcasts[ u32Idx ];
        if ( spBroadcast == NULL )
        {
            continue;
        }

        // Check if already at broadcast dealer
        if ( spBroadcastDealer->HasBroadcast( spBroadcast ) )
        {
            continue;
        }

        // Check if owned
        if ( std::find( aspPlayerBroadcasts.begin(), aspPlayerBroadcasts.end(), spBroadcast ) != aspPlayerBroadcasts.end() )
        {
            continue;
        }

        // Add broadcast
        _aspBroadcasts.push_back( spBroadcast );
    }
}
       
void GameLogic::CollectFreeAdverts( AdvertVec& _aspAdverts ) const
{
    // Get advert dealer
    AdvertDealerPtr spAdvertDealer = m_spGame->GetAdvertDealer();

    // Get all available adverts
    const AdvertVec& aspAdverts = m_spDatabase->GetAdverts();

    // Get all player adverts
    AdvertVec aspPlayerAdverts;
    CollectPlayerAdverts( aspPlayerAdverts );

    // Check all adverts
    Unsigned32 u32NumAdverts = aspAdverts.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumAdverts; ++u32Idx )
    {
        AdvertPtr spAdvert = aspAdverts[ u32Idx ];
        if ( spAdvert == NULL )
        {
            continue;
        }

        // Check if already at advert dealer
        if ( spAdvertDealer->HasAdvert( spAdvert ) )
        {
            continue;
        }

        // Check if owned
        if ( std::find( aspPlayerAdverts.begin(), aspPlayerAdverts.end(), spAdvert ) != aspPlayerAdverts.end() )
        {
            continue;
        }

        // Add advert
        _aspAdverts.push_back( spAdvert );
    }
}
               
void GameLogic::CreateNews( Unsigned32 _u32NumNewNews )
{
    // Check number of new news
    if ( _u32NumNewNews == 0 )
    {
        return;
    }

    // Get unused news
    NewsVec& aspUnusedNews = m_spGame->GetUnusedNews();

    for ( Unsigned32 u32Idx = 0; u32Idx < _u32NumNewNews; ++u32Idx )
    {
        // Check if to refill unused news
        if ( aspUnusedNews.size() == 0 )
        {
            aspUnusedNews = m_spDatabase->GetNews();
            std::random_shuffle( aspUnusedNews.begin(), aspUnusedNews.end() );

            // Check if any news available
            if ( aspUnusedNews.size() == 0 )
            {
                return;
            }
        }
         
        // Get unused news from stack
        NewsPtr spNews;   
        spNews = aspUnusedNews.back();
        aspUnusedNews.pop_back();

        // Create news instance
        NewsInstancePtr spNewsInstance = m_spGame->CreateNewsInstance();
        spNewsInstance->Init( m_spGame, spNews, m_spGame->GetDateTime() );

        // Add to all players that are subscribed to this channel
        const PlayerVec& aspPlayers = m_spGame->GetPlayers();
        Unsigned32 u32NumPlayers = aspPlayers.size();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
        {
            PlayerPtr spPlayer = aspPlayers[ u32Idx ];
            PlayerCompNewsPtr spCompNews = spPlayer->GetCompNews();
            if ( spCompNews->IsSubscribed( spNews->GetChannel() ) )
            {
                spCompNews->AddNews( spNewsInstance );
            }
        }
    }
}

Float32 GameLogic::ComputeNewsRating( PlayerPtr _spPlayer, Hour _eHour ) const
{
    PlayerCompNewsPtr spCompNews = _spPlayer->GetCompNews();
    if ( spCompNews == NULL )
    {
        return 0.0f;
    }

    Float32 f32NewsRating = 0.0f;
    for ( Unsigned32 u32Idx = 0; u32Idx < PlayerCompNews::NUM_PLANNED_NEWS; ++u32Idx )
    {
        NewsInstancePtr spNews = spCompNews->GetPlannedNews( u32Idx );
        if ( spNews == NULL )
        {
            continue;
        }

        // Don't consider news that are older than 4 hours
        Integer64 i64Days = 0;
        Integer32 i32Hours = 0;
        spNews->GetAge( i64Days, i32Hours );
        if ( i64Days > 0 ||
             i32Hours > 3 )
        {
            continue;
        }

        // Get current price
        const CFoundation::Money& currentPrice = spNews->GetCurrentPrice();

        // Compute Rating 
        Float32 f32Rating = currentPrice.GetAmount( CFoundation::CURRENCY_DOLLAR ) / 1000 * 2;

        // Add to news Rating
        f32NewsRating += f32Rating / 100.0;
    }

    // Get base rating
    Float32 f32TimeRating = m_spDatabase->GetRating( _eHour ) / 100.0;

    // Clamp Rating to maximum spectators at current hour
    CLAMP( f32NewsRating, 0.0f, f32TimeRating );

    return f32NewsRating;
}
        
Float32 GameLogic::ComputeRating( PlayerPtr _spPlayer, BroadcastPtr _spBroadcast, const CFoundation::Date& _Date, Hour _eHour ) const
{
    if ( _spBroadcast == NULL )
    {
        return 0.0f;
    }

    // Get genre
    GenrePtr spGenre = _spBroadcast->GetGenre();
    if ( spGenre == NULL )
    {
        CF_WARN( "Game::GetRating: No genre." );
        return 0.0f;
    }

    // Get base rating
    Float32 f32TimeRating = m_spDatabase->GetRating( _eHour ) / 100.0;

    // Get genre rating
    Float32 f32GenreRating = spGenre->GetRating( _eHour ) / 100.0;

    // Compute broadcast rating
    Float32 f32BroadcastUp2dateRating = 0.5 + 0.5 * _spBroadcast->GetUp2date() / 100.0;
    Float32 f32BroadcastBlockPrice = _spBroadcast->GetPrice().GetAmount() / _spBroadcast->GetNumBlocks();
    Float32 f32BroadcastPriceRating = std::min( 0.1845 + f32BroadcastBlockPrice / 1000.0 * 0.003825, 1.0 );
        
    // Compute additional news rating
    PlayerCompStatsPtr spCompStats = _spPlayer->GetCompStats();
    BlockStatPtr spStat = spCompStats->GetDayStat( _Date )->GetBlockStat( _eHour );
    Float32 f32AdditionalNewsRating = CFoundation::Math::Round( ( spStat->GetNewsRating() - 0.5 ) / 5 );

    // Compute final rating
    Float32 f32Rating = f32TimeRating * f32GenreRating * f32BroadcastUp2dateRating * f32BroadcastPriceRating + f32AdditionalNewsRating;
    CLAMP( f32Rating, 0, 1 );

    return f32Rating;
}
        
Float32 GameLogic::ComputeAvgRating( PlayerPtr _spPlayer, BroadcastPtr _spBroadcast, const CFoundation::Date& _Date, Hour _eHourEnd ) const
{
    // Get day stat
    PlayerCompStatsPtr spCompStats = _spPlayer->GetCompStats();
    DayStatPtr spDayStat = spCompStats->GetDayStat( _Date );

    // Compute average rating
    Unsigned32 u32NumBlocks = _spBroadcast->GetNumBlocks();
    Float32 f32AvgRating = 0.0f;
    Unsigned32 u32NumBlock = 0;
    for ( Hour eHour = _eHourEnd; u32NumBlock < u32NumBlocks; --eHour, ++u32NumBlock )
    {
        // Get block stat
        BlockStatPtr spStat = spDayStat->GetBlockStat( eHour );

        // Check if broadcast was sended
        CF_WARN_IF( spStat->GetBroadcast() != _spBroadcast, "ComputeAvgRating: Wrong broadcast." );

        // Get broadcast rating
        Float32 f32Rating = spStat->GetBroadcastRating();

        f32AvgRating += f32Rating;
    }
    f32AvgRating /= u32NumBlocks;

    return f32AvgRating;
}
        
void GameLogic::FinalizeBroadcast( PlayerPtr _spPlayer, BroadcastPtr _spBroadcast, Float32 _f32AvgRating ) const
{
    // Compute up2dateness loss
    Unsigned8 u8Up2dateLoss = static_cast< Unsigned8 >( _f32AvgRating * 2.0f * 100 );

    // Update up2dateness
    Unsigned8 u8Up2date = std::max< Unsigned8 >( 0, _spBroadcast->GetUp2date() - u8Up2dateLoss );
    _spBroadcast->SetUp2date( u8Up2date ); 
}
       
bool GameLogic::CheckAdvert( AdvertPtr _spAdvert, BroadcastPtr _spBroadcast, Unsigned32 _u32NumSpectators ) const
{
    if ( _u32NumSpectators < _spAdvert->GetMinSpectators() )
    {
        return false;
    }

    return true;
}
        
void GameLogic::FinalizeAdvert( PlayerPtr _spPlayer, AdvertPtr _spAdvert, bool _bSuccess ) const
{
    // Add money
    if ( _bSuccess )
    {
        _spPlayer->GetBudget().Book( ACCOUNT_ADVERT_INCOME, ACCOUNT_CASH, _spAdvert->GetBonus(), m_spGame->GetDateTime() );
    }
    else
    {
        _spPlayer->GetBudget().Book( ACCOUNT_LIQUIDATION_DAMAGES, ACCOUNT_CASH, -_spAdvert->GetPenalty(), m_spGame->GetDateTime() );
    }
}     

void GameLogic::CollectPlayerBroadcasts( BroadcastVec& _aspBroadcasts ) const
{
    const PlayerVec& aspPlayers = m_spGame->GetPlayers();
    Unsigned32 u32NumPlayers = aspPlayers.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerPtr spPlayer = aspPlayers[ u32Idx ];
        const BroadcastRack& clBroadcastRack = spPlayer->GetBroadcastRack();
        Unsigned32 u32NumBroadcasts = clBroadcastRack.GetNumBroadcasts();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBroadcasts; ++u32Idx )
        {
            _aspBroadcasts.push_back( clBroadcastRack.GetBroadcast( u32Idx ) );
        }
        const BroadcastBag& clBroadcastBag = spPlayer->GetBroadcastBag();
        u32NumBroadcasts = clBroadcastBag.GetNumBroadcasts();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBroadcasts; ++u32Idx )
        {
            BroadcastPtr spBroadcast = clBroadcastBag.GetBroadcast( u32Idx );
            if ( std::find( _aspBroadcasts.begin(), _aspBroadcasts.end(), spBroadcast ) == _aspBroadcasts.end() )
            {
                _aspBroadcasts.push_back( spBroadcast );
            }
        }
    }
}
          
void GameLogic::CollectPlayerAdverts( AdvertVec& _aspAdverts ) const
{
    const PlayerVec& aspPlayers = m_spGame->GetPlayers();
    Unsigned32 u32NumPlayers = aspPlayers.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerPtr spPlayer = aspPlayers[ u32Idx ];

        const AdvertContracts& clContracts = spPlayer->GetAdvertContracts();
        Unsigned32 u32NumContracts = clContracts.GetNumContracts();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumContracts; ++u32Idx )
        {
            _aspAdverts.push_back( clContracts.GetContract( u32Idx )->GetAdvert() );
        }

        const AdvertBag& clBag = spPlayer->GetAdvertBag();
        Unsigned32 u32NumAdverts = clBag.GetNumAdverts();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumAdverts; ++u32Idx )
        {
            _aspAdverts.push_back( clBag.GetAdvert( u32Idx ) );
        }
    }
}
        
void GameLogic::FillBroadcastDealer( bool _bClear )
{
    // Get broadcast dealer
    BroadcastDealerPtr spBroadcastDealer = m_spGame->GetBroadcastDealer();

    // Clear broadcasts if neccessary
    if ( _bClear )
    {
        spBroadcastDealer->RemoveAllBroadcasts();
    }

    // Get free slots
    Unsigned32 u32NumFreeSlots = spBroadcastDealer->GetNumFreeSlots();

    // Check if already full
    if ( u32NumFreeSlots == 0 )
    {
        return;
    }

    // Get all free broadcasts
    BroadcastVec aspBroadcasts;
    CollectFreeBroadcasts( aspBroadcasts );

    // Check if any free broadcasts
    Unsigned32 u32NumBroadcasts = aspBroadcasts.size();
    if ( u32NumBroadcasts == 0 )
    {
        return;
    }

    // Get start index
    Unsigned32 u32StartIdx = CFoundation::Random::GetU32( 0, u32NumBroadcasts );

    // Check all broadcasts
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBroadcasts && u32NumFreeSlots > 0; ++u32Idx )
    {
        BroadcastPtr spBroadcast = aspBroadcasts[ ( u32StartIdx + u32Idx ) % u32NumBroadcasts ];
        if ( spBroadcast == NULL )
        {
            continue;
        }

        // TODO CO Check if too expensive

        // Add to dealer
        spBroadcastDealer->AddBroadcast( spBroadcast );
        --u32NumFreeSlots;
    }
}
        
void GameLogic::FillAdvertDealer( bool _bClear )
{
    // Get advert dealer
    AdvertDealerPtr spAdvertDealer = m_spGame->GetAdvertDealer();

    // Clear adverts if neccessary
    if ( _bClear )
    {
        spAdvertDealer->RemoveAllAdverts();
    }

    // Get free slots
    Unsigned32 u32NumFreeSlots = spAdvertDealer->GetNumFreeSlots();

    // Check if already full
    if ( u32NumFreeSlots == 0 )
    {
        return;
    }

    // Get all free adverts
    AdvertVec aspAdverts;
    CollectFreeAdverts( aspAdverts );

    // Check if any free adverts
    Unsigned32 u32NumAdverts = aspAdverts.size();
    if ( u32NumAdverts == 0 )
    {
        return;
    }

    // Get start index
    Unsigned32 u32StartIdx = CFoundation::Random::GetU32( 0, u32NumAdverts );

    // Check all adverts
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumAdverts && u32NumFreeSlots > 0; ++u32Idx )
    {
        AdvertPtr spAdvert = aspAdverts[ ( u32StartIdx + u32Idx ) % u32NumAdverts ];
        if ( spAdvert == NULL )
        {
            continue;
        }

        // TODO CO Check if too expensive

        // Add to dealer
        spAdvertDealer->AddAdvert( spAdvert );
        --u32NumFreeSlots;
    }
}
       
void GameLogic::MoveImage( std::vector< PlayerRating >& _aPlayerRatings, Unsigned8 _u8ImagePoints ) const
{
    // Sort ratings
    std::sort( _aPlayerRatings.begin(), _aPlayerRatings.end() );

    // Give best player an image point from worst player
    Unsigned32 u32NumPlayers = _aPlayerRatings.size();
    if ( u32NumPlayers <= 1 )
    {
        return;
    }

    // Collect worst players
    std::vector< PlayerRating* > aWorstPlayers;
    Float32 f32WorstRating = 1.0f;
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerRating& rating = _aPlayerRatings[ u32Idx ];
        if ( rating.m_f32Rating <= f32WorstRating )
        {
            f32WorstRating = rating.m_f32Rating;
            aWorstPlayers.push_back( &rating );
        }
        else
        {
            break;
        }
    }

    // Collect best players
    std::vector< PlayerRating* > aBestPlayers;
    Float32 f32BestRating = 0.0f;
    for ( Unsigned32 u32Idx = u32NumPlayers; u32Idx > 0; --u32Idx )
    {
        PlayerRating& rating = _aPlayerRatings[ u32Idx - 1 ];
        if ( rating.m_f32Rating >= f32BestRating )
        {
            f32BestRating = rating.m_f32Rating;
            aBestPlayers.push_back( &rating );
        }
        else
        {
            break;
        }
    }

    // Check if best rating is better than worst one
    // and there are more or equal worst player than best ones (maximum 1 image point should be subtracted)
    if ( f32BestRating > f32WorstRating &&
         aWorstPlayers.size() >= aBestPlayers.size() )
    {
        Unsigned8 u8ImagePoints = 0;

        // Subtract one point from worst players
        Unsigned32 u32NumWorstPlayers = aWorstPlayers.size();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumWorstPlayers; ++u32Idx )
        {
            PlayerRating* pRating = aWorstPlayers[ u32Idx ];
            PlayerPtr spPlayer = pRating->m_spPlayer;
            Integer8 i8SubPoints = _u8ImagePoints;
            spPlayer->AddImage( -i8SubPoints );
            u8ImagePoints += _u8ImagePoints;

            pRating->m_i8ImageChange = -i8SubPoints;
        }

        // Add points to best players
        Unsigned32 u32NumBestPlayers = aBestPlayers.size();
        Unsigned8 u8AddPoints = static_cast< Unsigned8 >( u8ImagePoints / u32NumBestPlayers );
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBestPlayers; ++u32Idx )
        {
            PlayerRating* pRating = aBestPlayers[ u32Idx ];
            PlayerPtr spPlayer = pRating->m_spPlayer;
            spPlayer->AddImage( u8AddPoints );

            pRating->m_i8ImageChange = u8AddPoints;
        }
    }
}
        
void GameLogic::EndSammy() const
{
    SammyPtr spSammy = m_spGame->GetSammy();

    // Setup player ratings   
    std::vector< PlayerRating > aPlayerRatings;
    Unsigned32 u32NumPlayers = m_spGame->GetNumPlayers();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerPtr spPlayer = m_spGame->GetPlayer( u32Idx );
        aPlayerRatings.push_back( PlayerRating( spPlayer, spSammy->GetPoints( u32Idx ) ) );
    }

    // Move image
    MoveImage( aPlayerRatings, 5 );

    // Reset sammy
    m_spGame->SetSammy( SammyPtr() );
}
