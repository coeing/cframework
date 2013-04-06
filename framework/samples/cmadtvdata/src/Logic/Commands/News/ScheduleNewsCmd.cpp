#include "cmadtv/Logic/Commands/News/ScheduleNewsCmd.h"

#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/News/NewsInstance.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/PlayerCompNews.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// ScheduleNewsCmd
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF1_INST( ScheduleNewsCmd, "CMadTV::ScheduleNewsCmd", PlayerCommand );
        
ScheduleNewsCmd::ScheduleNewsCmd( PlayerContextPtr _spContext, NewsInstancePtr _spNews, Unsigned32 _u32Slot )
:   PlayerCommand( _spContext ),
    m_spNews( _spNews ),
    m_u32Slot( _u32Slot )
{
}

void ScheduleNewsCmd::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_Context >( m_spNews );
    _Serializer.SerializeEnum( m_u32Slot );
}
        
Unsigned32 ScheduleNewsCmd::Verify() const
{
    if ( m_spContext == NULL )
    {
        return ERROR_NONE;
    }

    PlayerPtr spPlayer = m_spContext->GetPlayer();
    if ( spPlayer == NULL )
    {
        return ERROR_NONE;
    }

    // Get news component
    PlayerCompNewsPtr spCompNews = spPlayer->GetCompNews();

    // Check if slot is valid
    if ( m_u32Slot >= PlayerCompNews::NUM_PLANNED_NEWS )
    {
        return ERROR_INVALID_SLOT;
    }

    // Check if player has news
    if ( m_spNews != NULL &&
        !spCompNews->HasNews( m_spNews ) )
    {
        return ERROR_NEWS_NOT_AVAILABLE;
    }

    // Check if enough money available
    Budget& budget = spPlayer->GetBudget();
    if ( m_spNews != NULL &&
         budget.GetCash() < m_spNews->GetCurrentPrice() )
    {
        return ERROR_INSUFFICIENT_MONEY;
    }

    return ERROR_NONE;
}

void ScheduleNewsCmd::Execute() const
{
    if ( m_spContext == NULL )
    {
        return;
    }

    PlayerPtr spPlayer = m_spContext->GetPlayer();
    if ( spPlayer == NULL )
    {
        return;
    }

    // Get news component
    PlayerCompNewsPtr spCompNews = spPlayer->GetCompNews();

    if ( m_spNews != NULL )
    {
        // Pay the news
        Budget& budget = spPlayer->GetBudget();
        budget.Book( CMadTV::ACCOUNT_CASH, CMadTV::ACCOUNT_NEWS_COSTS, m_spNews->GetCurrentPrice(), m_spContext->GetGame()->GetDateTime(), "Bought news" );
    }

    // Schedule
    spCompNews->SetPlannedNews( m_u32Slot, m_spNews );
}
        