#include "cmadtv/DragDrop/BroadcastDragController.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Layer.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Input/Input.h"

#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Windows/BroadcastBlock.h"

using namespace CMadTV;

BroadcastDragController::BroadcastDragController( BroadcastPtr _spBroadcast, const CFoundation::Date& _Date, Hour _eHour )
:   m_spDragData( new BroadcastDragData() )
{
    // Create broadcast block
    m_spBroadcastBlock = BroadcastBlock::Create( _spBroadcast );

    m_spDragData->m_spBroadcast = _spBroadcast;
    m_spDragData->m_Date = _Date;
    m_spDragData->m_eHour = _eHour;
}
        
BroadcastDragController::BroadcastDragController( BroadcastBlockPtr _spBroadcastBlock, const CFoundation::Date& _Date, Hour _eHour )
:   m_spBroadcastBlock( _spBroadcastBlock ),
    m_spDragData( new BroadcastDragData() )
{
    m_spDragData->m_spBroadcast = _spBroadcastBlock->GetBroadcast();
    m_spDragData->m_Date = _Date;
    m_spDragData->m_eHour = _eHour;
}

BroadcastDragController::~BroadcastDragController()
{
}
        
CGui::DragDataPtr BroadcastDragController::GetDragData() const
{
    return m_spDragData;
}
        
void BroadcastDragController::SetBroadcast( BroadcastPtr _spBroadcast )
{
    if ( _spBroadcast == m_spDragData->m_spBroadcast )
    {
        return;
    }

    m_spDragData->m_spBroadcast = _spBroadcast;

    // Create broadcast block
    m_spBroadcastBlock = BroadcastBlock::Create( _spBroadcast );
}
        
CGui::GuiObjectPtr BroadcastDragController::CreateProxyObject( CGui::GuiObjectPtr _spObject ) const
{
    return m_spBroadcastBlock->GetPanel();
}
