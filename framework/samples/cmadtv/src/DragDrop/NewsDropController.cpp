#include "cmadtv/DragDrop/NewsDropController.h"

#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/News/NewsInstance.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/PlayerCompNews.h"

#include "cmadtv/Logic/PlayerInput.h"
#include "cmadtv/Logic/Commands/News/ScheduleNewsCmd.h"

#include "cmadtv/DragDrop/NewsDragController.h"

using namespace CMadTV;

NewsDropController::NewsDropController( PlayerContextPtr _spContext, Unsigned32 _u32Slot )
:   m_spContext( _spContext ),
    m_u32Slot( _u32Slot )
{
}

NewsDropController::~NewsDropController()
{
}

bool NewsDropController::Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& /*_vDragPosition*/, const CFoundation::Vector2Di& /*_vDropPosition*/ ) 
{
    // News drag data expected
    NewsDragDataPtr spDragData = dynamic_pointer_cast< NewsDragData >( _spDragData );
    if ( spDragData == NULL ||
         m_spContext == NULL )
    {
        return false;
    }
    
    // Send command
    ScheduleNewsCmdPtr spCmd( new ScheduleNewsCmd( m_spContext, spDragData->m_spNews, m_u32Slot ) );
    Unsigned32 u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );

    return u32ReturnValue == 0;
}
                
void NewsDropController::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;
}
