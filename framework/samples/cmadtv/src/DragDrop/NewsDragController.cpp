#include "cmadtv/DragDrop/NewsDragController.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Layer.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Input/Input.h"

#include "cmadtv/Data/News/NewsInstance.h"
#include "cmadtv/Windows/NewsBlock.h"

using namespace CMadTV;

NewsDragController::NewsDragController( NewsInstancePtr _spNews, PlayerPtr _spPlayer, Unsigned32 _u32Slot )
:   m_spDragData( new NewsDragData() )
{
    // Create News block
    m_spNewsBlock = NewsBlock::Create( _spNews );
    
    m_spDragData->m_spNews = _spNews;
    m_spDragData->m_spPlayer = _spPlayer;
    m_spDragData->m_u32Slot = _u32Slot;
}
        
/*
NewsDragController::NewsDragController( NewsBlockPtr _spNewsBlock, SchedulePtr _spSchedule, Hour _eHour )
:   m_spNewsBlock( _spNewsBlock ),
    m_spDragData( new NewsDragData() )
{
    m_spDragData->m_spNews = _spNewsBlock->GetNews();
    m_spDragData->m_spSchedule = _spSchedule;
    m_spDragData->m_eHour = _eHour;
}*/

NewsDragController::~NewsDragController()
{
}
        
CGui::DragDataPtr NewsDragController::GetDragData() const
{
    return m_spDragData;
}
        
void NewsDragController::SetNews( NewsInstancePtr _spNews )
{
    m_spDragData->m_spNews = _spNews;
    m_spNewsBlock->SetNewsInstance( _spNews );
}
        
void NewsDragController::SetPlayer( PlayerPtr _spPlayer )
{
    m_spDragData->m_spPlayer = _spPlayer;
}
        
CGui::GuiObjectPtr NewsDragController::CreateProxyObject( CGui::GuiObjectPtr _spObject ) const
{
    return m_spNewsBlock->GetPanel();
}
