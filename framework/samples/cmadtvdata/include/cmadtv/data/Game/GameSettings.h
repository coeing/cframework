#ifndef CMADTV_GAME_SETTINGS_H
#define CMADTV_GAME_SETTINGS_H

#include <vector>

#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/Color.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Mission );

    class GameSettings
    {
    public:
        /// Player info
        class PlayerInfo
        {
        public:
            /// Name
            CFoundation::String m_strName;

            /// Color
            CFoundation::Color  m_colColor;

            /// Human
            bool                m_bHuman;
        };

        /// Adds a player
        void                        AddPlayer( const PlayerInfo& _Info );

        /// Removes the player with the passed index
        void                        RemovePlayer( Unsigned32 _u32Idx );

        /// Returns the number of players
        Unsigned32                  GetNumPlayers() const;

        /// Returns the player info with the passed index
        const PlayerInfo&           GetPlayerInfo( Unsigned32 _u32Idx ) const;

        /// Returns the name of the player with the passed index
        const CFoundation::String&  GetPlayerName( Unsigned32 _u32Idx ) const;

        /// Returns the color of the player with the passed index
        const CFoundation::Color&   GetPlayerColor( Unsigned32 _u32Idx ) const;

        /// Returns if the player with the passed index is a human
        bool                        IsHumanPlayer( Unsigned32 _u32Idx ) const;

        /// Sets the mission
        void                        SetMission( MissionPtr _spMission );

        /// Returns the mission
        MissionPtr                  GetMission() const;

    private:
        /// Players
        std::vector< PlayerInfo >   m_aPlayerInfos;

        /// Mission
        MissionPtr                  m_spMission;
    };
}

#endif
