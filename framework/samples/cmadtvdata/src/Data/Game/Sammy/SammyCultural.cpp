#include "cmadtv/Data/Game/Sammy/SammyCultural.h"

#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Genre.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/DayStat.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerCompStats.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// SammyCultural
///
////////////////////////////////////////////////////////////////////////////////////////////

SammyCultural::SammyCultural()
:   Sammy( TYPE_CULTURAL )
{
}

void SammyCultural::OnBroadcastsStarted( const CFoundation::Date& _Date, Hour _eHour )
{
    // Check broadcasts of each player
    GameCPtr spGame = GetGame();
    Unsigned32 u32NumPlayers = spGame->GetNumPlayers();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
    {
        PlayerPtr spPlayer = spGame->GetPlayer( u32Idx );
        PlayerCompStatsPtr spCompStats = spPlayer->GetCompStats();

        // Get current block stat
        DayStatPtr spDayStat = spCompStats->GetDayStat( _Date );
        BlockStatPtr spBlockStat = spDayStat->GetBlockStat( _eHour );

        // Get current broadcast
        BroadcastPtr spBroadcast = spBlockStat->GetBroadcast();
        if ( spBroadcast == NULL )
        {
            continue;
        }

        // Check if cultural program
        if ( spBroadcast->GetGenre()->GetID() != 5 )   // TODO Some better way to check which genre?
        {
            continue;
        }

        // Add rating as points
        AddPoints( u32Idx, static_cast< Unsigned32 >( spBlockStat->GetBroadcastRating() * 100 ) );
    }
}
