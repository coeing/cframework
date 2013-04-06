#ifndef CMADTV_GAME_H
#define CMADTV_GAME_H

#include <map>
#include <vector>

#include "cfoundation/Base/Types.h"
#include "cfoundation/DateTime/DateTime.h"
#include "cfoundation/Utils/SharedPtr.h"
#include "cfoundation/Utils/Singleton.h"

#include "cmadtv/Data/World/Hour.h"
#include "cmadtv/Data/World/WorldEvents.h"

namespace CFoundation
{
    class Date;
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( AdvertContract );
    INCLUDE_SHARED_CLASS( AdvertDealer );
    INCLUDE_SHARED_CLASS( BroadcastDealer );
    INCLUDE_SHARED_CLASS( Mission );
    INCLUDE_SHARED_CLASS( News );
    INCLUDE_SHARED_CLASS( NewsInstance );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( Present );
    INCLUDE_SHARED_CLASS( Sammy );
    INCLUDE_SHARED_CLASS( Transmitter );

    class GameSettings;
    enum Hour;

    enum GameEvent
    {
        EVENT_GAME_DATE_CHANGED,
        EVENT_GAME_TIME_CHANGED,
        EVENT_GAME_HOUR_CHANGED,
        EVENT_GAME_HOUR_STAGE_CHANGED,
    };
    TYPEDEF_DELEGATE1( GameEvent, GamePtr );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// Game
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class Game : public CFoundation::SharedPtrBase< Game >, 
                 public CFoundation::Delegate1Invoker< WorldEvent, GamePtr >, 
                 public CFoundation::Delegate1Invoker< WorldEventGame, GamePtr >,
                 public CFoundation::Delegate1Invoker< GameEvent, GamePtr >                 
    {
    public:
        DELEGATE1_INVOKER( WorldEvent, GamePtr );
        DELEGATE1_INVOKER( WorldEventGame, GamePtr );
        DELEGATE1_INVOKER( GameEvent, GamePtr );

        /// States
        enum State
        {
            STATE_NONE,

            STATE_PRE_GAME,

            STATE_PRE_BROADCASTING,

            STATE_NEWS,
            STATE_BROADCAST,
            STATE_ADVERT,

            STATE_END
        };

        /// Constructor
        Game();

        /// Assignment operator
        void                            operator=( const Game& _rhs );

        /// Serialization
        void                            Serialize( CFoundation::Serializer& _Serializer );

        /// Resets the game data
        void                            Reset();

        /// Sets the state
        void                            SetState( State _eState );

        /// Returns the state
        State                           GetState() const;

        /// Sets the mission
        void                            SetMission( MissionPtr _spMission );

        /// Returns the current mission
        MissionPtr                      GetMission() const;

        //////////////////////////////////////////////////////////////////////
        /// Date/Time
        //////////////////////////////////////////////////////////////////////

        /// Returns the hour for the passed time
        static Hour                     GetHour( const CFoundation::Time& _Time );

        /// Returns the hour stage for the passed time
        static HourStage                GetHourStage( const CFoundation::Time& _Time );
        
        /// Returns the current date and time
        CFoundation::DateTime           GetDateTime() const;

        /// Sets the current date
        void                            SetDate( const CFoundation::Date& _Date );

        /// Sets the current time
        void                            SetTime( const CFoundation::Time& _Time );

        /// Returns the current date
        const CFoundation::Date&        GetDate() const;

        /// Returns the current time
        const CFoundation::Time&        GetTime() const;

        /// Returns the passed days
        Unsigned32                      GetDays() const;

        /// Returns the current hour
        Hour                            GetHour() const;

        /// Returns the current hour stage
        HourStage                       GetHourStage() const;

        /// Returns the unused time (in secs)
        void                            SetUnusedTime( Float32 _f32UnusedSecs );

        /// Returns the unused time (in secs)
        Float32                         GetUnusedTime() const;

        /// Sets the speed
        void                            SetSpeed( Float32 _f32Speed );

        /// Returns the current speed
        Float32                         GetSpeed() const;

        //////////////////////////////////////////////////////////////////////
        /// Players
        //////////////////////////////////////////////////////////////////////

        /// Adds a player
        void                            AddPlayer( PlayerPtr _spPlayer );

        /// Returns the number of players
        Unsigned32                      GetNumPlayers() const;

        /// Returns the player with the passed index
        PlayerPtr                       GetPlayer( Unsigned32 _u32Idx ) const;

        /// Returns the player index of the passed player
        Unsigned32                      GetPlayerIdx( PlayerPtr _spPlayer ) const;

        /// Returns the players
        const std::vector< PlayerPtr >& GetPlayers() const;

        //////////////////////////////////////////////////////////////////////
        /// Broadcast dealer
        //////////////////////////////////////////////////////////////////////

        /// Returns the broadcast dealer
        BroadcastDealerPtr              GetBroadcastDealer();

        //////////////////////////////////////////////////////////////////////
        /// Advert dealer
        //////////////////////////////////////////////////////////////////////

        /// Returns the advert dealer
        AdvertDealerPtr                 GetAdvertDealer();

        //////////////////////////////////////////////////////////////////////
        /// News
        //////////////////////////////////////////////////////////////////////

        /// Creates a new news instance
        NewsInstancePtr                 CreateNewsInstance();

        /// Returns the news instance with the passed ID
        NewsInstancePtr                 GetNewsInstance( Unsigned32 _u32ID ) const;

        /// Sets the unused news
        void                            SetUnusedNews( const std::vector< NewsPtr >& _aspUnusedNews );

        /// Returns the unused news
        std::vector< NewsPtr >&         GetUnusedNews();

        /// Returns the unused news
        const std::vector< NewsPtr >&   GetUnusedNews() const;

        //////////////////////////////////////////////////////////////////////
        /// Transmitters
        //////////////////////////////////////////////////////////////////////

        /// Creates a new transmitter
        TransmitterPtr                  CreateTransmitter();

        /// Returns the transmitter with the passed ID
        TransmitterPtr                  GetTransmitter( Unsigned32 _u32ID ) const;

        //////////////////////////////////////////////////////////////////////
        /// Presents
        //////////////////////////////////////////////////////////////////////

        /// Increases the counter for the passed present
        void                            IncreasePresentCounter( PresentPtr _spPresent );

        /// Returns the number of times the passed present was given to betty
        Unsigned32                      GetPresentCounter( PresentPtr _spPresent ) const;

        /// Computes the love increase when giving the passed present to betty
        Unsigned8                       ComputePresentLoveIncrease( PresentPtr _spPresent ) const;

        //////////////////////////////////////////////////////////////////////
        /// Sammy
        //////////////////////////////////////////////////////////////////////

        /// Sets the current sammy
        void                            SetSammy( SammyPtr _spSammy );

        /// Returns the current sammy
        SammyPtr                        GetSammy() const;

        /// Sets the next sammy
        void                            SetNextSammy( SammyPtr _spSammy );

        /// Returns the next sammy
        SammyPtr                        GetNextSammy() const;

        /// Serializes a sammy pointer
        void                            Serialize( CFoundation::Serializer& _Serializer, SammyPtr& _spSammy ) const;

    private:
        typedef std::vector< PlayerPtr > PlayerVec;
        typedef std::map< PresentPtr, Unsigned32 > PresentCounterMap;
        typedef std::vector< TransmitterPtr > TransmitterVec;

        //////////////////////////////////////////////////////////////////////
        /// Functions that are called at special times
        //////////////////////////////////////////////////////////////////////

        /// State
        State                           m_eState;

        /// Start date
        CFoundation::Date               m_StartDate;

        /// Current date and time
        CFoundation::Date               m_Date;
        CFoundation::Time               m_Time;

        /// Unused time (in sec)
        Float32                         m_f32Secs;

        /// Speed factor
        Float32                         m_f32Speed;

        /// Players
        PlayerVec                       m_aspPlayers;

        /// Broadcast dealer
        BroadcastDealerPtr              m_spBroadcastDealer;

        /// Advert dealer
        AdvertDealerPtr                 m_spAdvertDealer;

        /// Current mission
        MissionPtr                      m_spMission;

        /// Unused news
        std::vector< NewsPtr >          m_aspUnusedNews;

        /// Next news instance ID
        Unsigned32                      m_u32NextNewsInstanceID;

        /// Created news instance
        std::vector< NewsInstancePtr >  m_aspNewsInstances;

        /// Next transmitter ID
        Unsigned32                      m_u32NextTransmitterID;

        /// Created transmitters
        TransmitterVec                  m_aspTransmitters;

        /// Present counters
        PresentCounterMap               m_mapPresentCounters;

        /// Current sammy
        SammyPtr                        m_spSammy;

        /// Next sammy
        SammyPtr                        m_spNextSammy;
    };

    INCLUDE_SHARED_CLASS( Game );
}

#endif
