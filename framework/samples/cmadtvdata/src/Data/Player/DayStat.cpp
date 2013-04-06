#include "cmadtv/Data/Player/DayStat.h"

#include "cfoundation/Serialization/Serializer.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Player/BlockStat.h"

using namespace CMadTV;

DayStat::DayStat()
{
    // Create block stats
    for ( Hour eHour = HOUR_NONE; eHour != HOUR_END; ++eHour )
    {
        m_aspBlockStats[ eHour ] = BlockStatPtr( new BlockStat() );
    }
}

DayStat::~DayStat()
{
}
        
void DayStat::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.SerializeArray< CFoundation::SerializeWrapper_SharedPtrSimple >( m_aspBlockStats, HOUR_END );
}
               
BlockStatPtr DayStat::GetBlockStat( Hour _eSlot )
{
    return m_aspBlockStats[ _eSlot ];
}
        
BlockStatCPtr DayStat::GetBlockStat( Hour _eSlot ) const
{
    return m_aspBlockStats[ _eSlot ];
}
     
Unsigned32 DayStat::GetAvgBroadcastSpectators() const
{
    Unsigned32 u32NumSpectators = 0;
    for ( Hour eHour = HOUR_BEGIN; eHour != HOUR_END; ++eHour )
    {
        u32NumSpectators += m_aspBlockStats[ eHour ]->GetBroadcastSpectators();
    }
    u32NumSpectators /= ( HOUR_END - HOUR_BEGIN );
    return u32NumSpectators;
}
    