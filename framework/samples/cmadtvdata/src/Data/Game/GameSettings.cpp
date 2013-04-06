#include "cmadtv/Data/Game/GameSettings.h"

using namespace CMadTV;

void GameSettings::AddPlayer( const PlayerInfo& _Info )
{
    m_aPlayerInfos.push_back( _Info );
}

void GameSettings::RemovePlayer( Unsigned32 _u32Idx )
{
    if ( _u32Idx < m_aPlayerInfos.size() )
    {
        m_aPlayerInfos.erase( m_aPlayerInfos.begin() + _u32Idx );
    }
}

Unsigned32 GameSettings::GetNumPlayers() const
{
    return m_aPlayerInfos.size();
}
        
const GameSettings::PlayerInfo& GameSettings::GetPlayerInfo( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < m_aPlayerInfos.size() )
    {
        return m_aPlayerInfos[ _u32Idx ];
    }
    else
    {
        static PlayerInfo strTmp;
        return strTmp;
    }
}

const CFoundation::String& GameSettings::GetPlayerName( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < m_aPlayerInfos.size() )
    {
        return m_aPlayerInfos[ _u32Idx ].m_strName;
    }
    else
    {
        static CFoundation::String strEmpty;
        return strEmpty;
    }
}
       
const CFoundation::Color& GameSettings::GetPlayerColor( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < m_aPlayerInfos.size() )
    {
        return m_aPlayerInfos[ _u32Idx ].m_colColor;
    }
    else
    {
        static CFoundation::Color colDefault;
        return colDefault;
    }
}

bool GameSettings::IsHumanPlayer( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < m_aPlayerInfos.size() )
    {
        return m_aPlayerInfos[ _u32Idx ].m_bHuman;
    }
    else
    {
        return false;
    }
}
        
void GameSettings::SetMission( MissionPtr _spMission )
{
    m_spMission = _spMission;
}

MissionPtr GameSettings::GetMission() const
{
    return m_spMission;
}
