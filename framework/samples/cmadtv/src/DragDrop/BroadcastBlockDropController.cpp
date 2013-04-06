#include "cmadtv/DragDrop/BroadcastBlockDropController.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Input/Input.h"

#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Player/PlayerContext.h"

#include "cmadtv/Logic/PlayerInput.h"
#include "cmadtv/Logic/Commands/ScheduleBroadcastCmd.h"

#include "cmadtv/DragDrop/BroadcastDragController.h"
#include "cmadtv/Windows/BroadcastBlock.h"

using namespace CMadTV;

BroadcastBlockDropController::BroadcastBlockDropController( PlayerContextPtr _spContext, const CFoundation::Date& _Date, BroadcastBlockPtr _spBroadcastBlock, Hour _eStartSlot )
:   m_spContext( _spContext ),
    m_Date( _Date ),
    m_spBroadcastBlock( _spBroadcastBlock ),
    m_eStartSlot( _eStartSlot )
{
}

BroadcastBlockDropController::~BroadcastBlockDropController()
{
}

bool BroadcastBlockDropController::Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& /*_vDragPosition*/, const CFoundation::Vector2Di& _vDropPosition ) 
{
    // Broadcast drag data expected
    BroadcastDragDataPtr spDragData = dynamic_pointer_cast< BroadcastDragData >( _spDragData );
    if ( spDragData == NULL )
    {
        return false;
    }

    BroadcastPtr spBroadcast = spDragData->m_spBroadcast;

    // Compute block number at drop position
    Unsigned32 u32Block = m_spBroadcastBlock->ComputeBlock( _vDropPosition );

    // Compute start slot
    Hour eStartSlot = static_cast< Hour >( m_eStartSlot + u32Block );

    // Try to schedule broadcast at this slot
    ScheduleBroadcastCmdPtr spCmd( new ScheduleBroadcastCmd( m_spContext, spBroadcast, m_Date, eStartSlot ) );
    Unsigned32 u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
    switch ( u32ReturnValue )
    {
    case ScheduleBroadcastCmd::ERROR_INVALID_DATE:
        {
        }
        break;
    case ScheduleBroadcastCmd::ERROR_BROADCAST_TOO_LONG:
        {
        }
        break;
    }

    return u32ReturnValue == 0;
}
       
void BroadcastBlockDropController::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;
}



