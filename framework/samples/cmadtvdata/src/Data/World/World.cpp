#include "cmadtv/Data/World/World.h"

#include <algorithm>

#include "cfoundation/Debug/Makros.h"
#include "cfoundation/Math/Random.h"
#include "cfoundation/Serialization/Serializer.h"

#include "csystem/Files/File.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Genre.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Game/GameSettings.h"
#include "cmadtv/Data/Game/Mission.h"
#include "cmadtv/Data/Game/MissionConditionFactory.h"

#include "cmadtv/Logic/Game/GameLogic.h"

using namespace CMadTV;

World::World()
{
    // Load database
    m_spDatabase = DatabasePtr( new Database() );
    m_spDatabase->LoadFromXML( CSystem::FilePtr( new CSystem::File( "data/database/madtv.xml" ) ) );
}

World::~World()
{
}
        
void World::Serialize( CFoundation::Serializer& _Serializer )
{
    // Serialize game
    bool bGame = m_spGame != NULL;
    _Serializer.Serialize( bGame );    
    if ( bGame )
    {
        if ( _Serializer.IsLoading() )
        {
            m_spGame = GamePtr( new Game() );
        }
        m_spGame->Serialize( _Serializer );
    }
}
        
void World::Update( Float32 _f32TimeDifference )
{
    if ( m_spGame != NULL )
    {
        GameLogic logic;
        logic.Setup( m_spDatabase, m_spGame );
        logic.UpdateGame( _f32TimeDifference );
    }
}
        
void World::StartGame( const GameSettings& _Settings )
{
    // Init game
    m_spGame = GamePtr( new Game() );

    GameLogic logic;
    logic.Setup( m_spDatabase, m_spGame );
    logic.StartGame( _Settings );
}
        
DatabasePtr World::GetDatabase() const
{
    return m_spDatabase;
}
        
GamePtr World::GetGame() const
{
    return m_spGame;
}