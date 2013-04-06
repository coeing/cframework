#include "cmadtv/Logic/Commands/News/ClearRecentlySubscribedChannelsCmd.h"

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
/// ClearRecentlySubscribedChannelsCmd
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF1_INST( ClearRecentlySubscribedChannelsCmd, "CMadTV::ClearRecentlySubscribedChannelsCmd", PlayerCommand );
        
ClearRecentlySubscribedChannelsCmd::ClearRecentlySubscribedChannelsCmd( PlayerContextPtr _spContext )
:   PlayerCommand( _spContext )
{
}

void ClearRecentlySubscribedChannelsCmd::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );
}
        
Unsigned32 ClearRecentlySubscribedChannelsCmd::Verify() const
{
    return ERROR_NONE;
}

void ClearRecentlySubscribedChannelsCmd::Execute() const
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

    // Clear recently subscribed channels
    spCompNews->RemoveRecentlySubscribedChannels();
}
        