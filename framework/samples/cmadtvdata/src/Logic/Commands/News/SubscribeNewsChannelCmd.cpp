#include "cmadtv/Logic/Commands/News/SubscribeNewsChannelCmd.h"

#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/News/NewsChannel.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/PlayerCompNews.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// SubscribeNewsChannelCmd
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF1_INST( SubscribeNewsChannelCmd, "CMadTV::SubscribeNewsChannelCmd", PlayerCommand );
        
SubscribeNewsChannelCmd::SubscribeNewsChannelCmd( PlayerContextPtr _spContext, NewsChannelPtr _spNewsChannel, bool _bSubscribe )
:   PlayerCommand( _spContext ),
    m_spChannel( _spNewsChannel ),
    m_bSubscribe( _bSubscribe )
{
}

void SubscribeNewsChannelCmd::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_Context >( m_spChannel );
    _Serializer.SerializeEnum( m_bSubscribe );
}
        
Unsigned32 SubscribeNewsChannelCmd::Verify() const
{
    if ( m_spContext == NULL ||
         m_spChannel == NULL )
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

    // Check if subscribed
    bool bSubscribed = spCompNews->IsSubscribed( m_spChannel );
    if ( bSubscribed == m_bSubscribe )
    {
        return m_bSubscribe ? ERROR_ALREADY_SUBSCRIBED : ERROR_NOT_SUBSCRIBED;
    }

    if ( m_bSubscribe )
    {
        // Check if enough money available
        Budget& budget = spPlayer->GetBudget();
        if ( budget.GetCash() < m_spChannel->GetPrice() )
        {
            return ERROR_INSUFFICIENT_MONEY;
        }
    }

    return ERROR_NONE;
}

void SubscribeNewsChannelCmd::Execute() const
{
    if ( m_spContext == NULL ||
         m_spChannel == NULL )
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

    if ( m_bSubscribe )
    {
        // Pay
        Budget& budget = spPlayer->GetBudget();
        budget.Book( ACCOUNT_CASH, ACCOUNT_NEWS_COSTS, m_spChannel->GetPrice(), m_spContext->GetGame()->GetDateTime() );

        // Add to subscribed channels
        spCompNews->AddChannel( m_spChannel );
        spCompNews->AddRecentlySubscribedChannel( m_spChannel );
    }
    else
    {
        // Payback if recently subscribed
        if ( spCompNews->WasRecentlySubscribed( m_spChannel ) )
        {
            Budget& budget = spPlayer->GetBudget();
            budget.Book( ACCOUNT_NEWS_COSTS, ACCOUNT_CASH, m_spChannel->GetPrice(), m_spContext->GetGame()->GetDateTime() );
            spCompNews->RemoveRecentlySubscribedChannel( m_spChannel );
        }

        // Remove from subscribed channels
        spCompNews->RemoveChannel( m_spChannel );
    }
}
        