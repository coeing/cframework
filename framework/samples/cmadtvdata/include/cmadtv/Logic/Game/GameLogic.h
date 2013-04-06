#ifndef CMADTV_PLAYER_INPUT_H
#define CMADTV_PLAYER_INPUT_H

#include <vector>

#include "cfoundation/Base/Types.h"
#include "cfoundation/DateTime/DateTime.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );
    INCLUDE_SHARED_CLASS( Broadcast );
    INCLUDE_SHARED_CLASS( Database );
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( Player );

    typedef std::vector< BroadcastPtr > BroadcastVec;
    typedef std::vector< AdvertPtr > AdvertVec;

    class GameSettings;
    enum Hour;

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// GameLogic
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class GameLogic
    {
    public:
        /// Helper structure to perform image movements
        struct PlayerRating;

        /// Sets up the database and game to use
        void                    Setup( DatabasePtr _spDatabase, GamePtr _spGame );

        /// Starts the game
        void                    StartGame( const GameSettings& _Settings );

        /// Per frame update
        void                    UpdateGame( Float32 _f32TimeDifference );

        /// Moves image from the worst player to the best one
        void                    MoveImage( std::vector< PlayerRating >& _aPlayerRatings, Unsigned8 _u8ImagePoints ) const;

    private:

        /// Per minute update
        void                    UpdateMinute();

        /// Start day
        void                    StartDay();

        /// Start news
        void                    StartNews( Hour _eHour );

        /// Start broadcasts
        void                    StartBroadcasts( Hour _eHour );

        /// Start adverts
        void                    StartAdverts( Hour _eHour );

        /// End day
        void                    EndDay();

        /// Creates new news
        void                    CreateNews( Unsigned32 _u32NumNewNews );

        /// Computes the rating for the planned news of the passed player
        Float32                 ComputeNewsRating( PlayerPtr _spPlayer, Hour _eHour ) const;

        /// Computes the rating for the passed broadcast at the passed time
        Float32                 ComputeRating( PlayerPtr _spPlayer, BroadcastPtr _spBroadcast, const CFoundation::Date& _Date, Hour _eHour ) const;

        /// Computes the average rating of the broadcast that ended at the passed time
        Float32                 ComputeAvgRating( PlayerPtr _spPlayer, BroadcastPtr _spBroadcast, const CFoundation::Date& _Date, Hour _eHourEnd ) const;

        /// Finalizes the passed broadcast after it was sended completely
        void                    FinalizeBroadcast( PlayerPtr _spPlayer, BroadcastPtr _spBroadcast, Float32 _f32AvgRating ) const;

        /// Checks if the passed advert is successful
        bool                    CheckAdvert( AdvertPtr _spAdvert, BroadcastPtr _spBroadcast, Unsigned32 _u32NumSpectators ) const;

        /// Finalizes the passed advert for the passed player
        /// _bSuccess indicates if it succeeded
        void                    FinalizeAdvert( PlayerPtr _spPlayer, AdvertPtr _spAdvert, bool _bSuccess ) const;

        /// Collects all broadcasts of all players
        void                    CollectPlayerBroadcasts( BroadcastVec& _aspBroadcasts ) const;

        /// Collects all adverts of all players
        void                    CollectPlayerAdverts( AdvertVec& _aspAdverts ) const;

        /// Collects the broadcasts that are not owned by any player and
        /// not at the broadcast dealer or in an auction
        void                    CollectFreeBroadcasts( BroadcastVec& _aspBroadcasts ) const;

        /// Collects the adverts that are not owned by any player and
        /// not at the advert dealer
        void                    CollectFreeAdverts( AdvertVec& _aspAdverts ) const;

        /// Fills up the broadcast dealer
        /// _bClear indicates if he should be cleared before fill up
        void                    FillBroadcastDealer( bool _bClear = false );

        /// Fills up the advert dealer
        /// _bClear indicates if he should be cleared before fill up
        void                    FillAdvertDealer( bool _bClear = false );

        /// Ends the sammy
        /// Moves image points from worst to best player
        void                    EndSammy() const;

        /// Database
        DatabasePtr             m_spDatabase;

        /// Game
        GamePtr                 m_spGame;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// GameLogic::PlayerRating
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    struct GameLogic::PlayerRating
    {
        PlayerRating( PlayerPtr _spPlayer, Float32 _f32Rating )
        :   m_spPlayer( _spPlayer ),
            m_f32Rating( _f32Rating ),
            m_i8ImageChange( 0 )
        {
        }

        bool operator<( const PlayerRating& _rhs ) const
        {
            return m_f32Rating < _rhs.m_f32Rating;
        }

        PlayerPtr       m_spPlayer;
        Float32         m_f32Rating;

        Integer8        m_i8ImageChange;
    };
}

#endif
