#include "cmadtv/Logic/Commands/Commands.h"

#include "cmadtv/Logic/Commands/BuyBroadcastCmd.h"
#include "cmadtv/Logic/Commands/MoveAdvertCmd.h"
#include "cmadtv/Logic/Commands/MoveBroadcastCmd.h"
#include "cmadtv/Logic/Commands/ScheduleAdvertCmd.h"
#include "cmadtv/Logic/Commands/ScheduleBroadcastCmd.h"
#include "cmadtv/Logic/Commands/SellBroadcastCmd.h"
#include "cmadtv/Logic/Commands/News/ClearRecentlySubscribedChannelsCmd.h"
#include "cmadtv/Logic/Commands/News/ScheduleNewsCmd.h"
#include "cmadtv/Logic/Commands/News/SubscribeNewsChannelCmd.h"
#include "cmadtv/Logic/Commands/Spectators/BuyAntennaCmd.h"
#include "cmadtv/Logic/Commands/Spectators/TradeTransmitterCmd.h"

using namespace CMadTV;

#define REFERENCE( Class )  Class();

void CMadTV::IncludeCommands()
{
    REFERENCE( BuyAntennaCmd )
    REFERENCE( BuyBroadcastCmd )
    REFERENCE( ClearRecentlySubscribedChannelsCmd )
    REFERENCE( MoveAdvertCmd )
    REFERENCE( MoveBroadcastCmd )
    REFERENCE( ScheduleAdvertCmd )
    REFERENCE( ScheduleBroadcastCmd )
    REFERENCE( ScheduleNewsCmd )
    REFERENCE( SellBroadcastCmd )
    REFERENCE( SubscribeNewsChannelCmd )
    REFERENCE( TradeTransmitterCmd )
}
