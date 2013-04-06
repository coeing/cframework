#include "cmadtv/Data/Game/MissionCondition.h"
        
using namespace CMadTV;

MissionCondition::MissionCondition()
{
}
       
void MissionCondition::Init()
{
}
       
void MissionCondition::Deinit()
{
    m_spWorld.reset();
    m_spGame.reset();
    m_spPlayer.reset();
}

void MissionCondition::SetWorld( WorldPtr _spWorld )
{
    m_spWorld = _spWorld;
}

WorldPtr MissionCondition::GetWorld() const
{
    return m_spWorld;
}

void MissionCondition::SetGame( GamePtr _spGame )
{
    m_spGame = _spGame;
}

GamePtr MissionCondition::GetGame() const
{
    return m_spGame;
}

void MissionCondition::SetPlayer( PlayerPtr _spPlayer )
{
    m_spPlayer = _spPlayer;
}

PlayerPtr MissionCondition::GetPlayer() const
{
    return m_spPlayer;
}

void MissionCondition::Serialize( CFoundation::Serializer& _Serializer )
{
}
        
void MissionCondition::Update( Float32 _f32TimeDifference )
{
}
