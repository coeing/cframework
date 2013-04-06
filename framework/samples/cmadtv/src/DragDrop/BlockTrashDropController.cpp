#include "cmadtv/DragDrop/BlockTrashDropController.h"

#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/PlayerCompNews.h"

#include "cmadtv/Logic/PlayerInput.h"
#include "cmadtv/Logic/Commands/ScheduleAdvertCmd.h"
#include "cmadtv/Logic/Commands/ScheduleBroadcastCmd.h"

#include "cmadtv/DragDrop/AdvertDragController.h"
#include "cmadtv/DragDrop/BroadcastDragController.h"
#include "cmadtv/DragDrop/NewsDragController.h"

using namespace CMadTV;

BlockTrashDropController::BlockTrashDropController( PlayerContextPtr _spContext )
:   m_spContext( _spContext )
{
}

BlockTrashDropController::~BlockTrashDropController()
{
}

bool BlockTrashDropController::Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& /*_vDragPosition*/, const CFoundation::Vector2Di& /*_vDropPosition*/ )
{
    Unsigned32 u32ReturnValue = 0;

    // BroadcastDragData expected
    BroadcastDragDataPtr spBroadcastDragData = dynamic_pointer_cast< BroadcastDragData >( _spDragData );
    if ( spBroadcastDragData != NULL )
    {
        // Schedule no broadcast at this slot
        ScheduleBroadcastCmdPtr spCmd( new ScheduleBroadcastCmd( m_spContext, BroadcastPtr(), spBroadcastDragData->m_Date, spBroadcastDragData->m_eHour ) );
        u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
        switch ( u32ReturnValue )
        {
        case ScheduleBroadcastCmd::ERROR_INVALID_DATE:
        case ScheduleBroadcastCmd::ERROR_BROADCAST_TOO_LONG:
            {
            }
            break;
        }

        return true;
    }

    AdvertDragDataPtr spAdvertDragData = dynamic_pointer_cast< AdvertDragData >( _spDragData );
    if ( spAdvertDragData != NULL )
    {
        // Schedule no advert at this slot
        ScheduleAdvertCmdPtr spCmd( new ScheduleAdvertCmd( m_spContext, AdvertPtr(), spAdvertDragData->m_Date, spAdvertDragData->m_eHour ) );
        u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
        switch ( u32ReturnValue )
        {
        case ScheduleAdvertCmd::ERROR_INVALID_DATE:
            {
            }
            break;
        }
    }

    NewsDragDataPtr spNewsDragData = dynamic_pointer_cast< NewsDragData >( _spDragData );
    if ( spNewsDragData != NULL )
    {
        PlayerCompNewsPtr spCompNews = spNewsDragData->m_spPlayer != NULL ? spNewsDragData->m_spPlayer->GetCompNews() : PlayerCompNewsPtr();
        if ( spCompNews != NULL )
        {
            if ( spNewsDragData->m_u32Slot != MAX_U32 )
            {
                spCompNews->SetPlannedNews( spNewsDragData->m_u32Slot, NewsInstancePtr() );
            }
            else
            {
                spCompNews->RemoveNews( spNewsDragData->m_spNews );
            }
        }
        return true;
    }

    return u32ReturnValue == 0;
}
        
void BlockTrashDropController::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;
}
