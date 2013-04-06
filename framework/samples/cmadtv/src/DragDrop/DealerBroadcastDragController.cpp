#include "cmadtv/DragDrop/DealerBroadcastDragController.h"

#include "cgui/Gui/GuiMain.h"

#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Windows/BroadcastBlock.h"

using namespace CMadTV;

DealerBroadcastDragController::DealerBroadcastDragController( BroadcastPtr _spBroadcast )
:   m_spDragData( new DealerBroadcastDragData() )
{
    m_spDragData->m_spBroadcast = _spBroadcast;
}

DealerBroadcastDragController::~DealerBroadcastDragController()
{
}
        
CGui::DragDataPtr DealerBroadcastDragController::GetDragData() const
{
    return m_spDragData;
}
        
void DealerBroadcastDragController::SetBroadcast( BroadcastPtr _spBroadcast )
{
    m_spDragData->m_spBroadcast = _spBroadcast;
}
        
CGui::GuiObjectPtr DealerBroadcastDragController::CreateProxyObject( CGui::GuiObjectPtr _spObject ) const
{
    return CGui::GuiMain::GetInstance().CreateFromXML( "data/Windows/BroadcastDragObject.xml" );
}
