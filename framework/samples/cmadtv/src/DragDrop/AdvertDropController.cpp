#include "cmadtv/DragDrop/AdvertDropController.h"

#include "cfoundation/DateTime/Date.h"

#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/Scheduling.h"

#include "cmadtv/Logic/PlayerInput.h"
#include "cmadtv/Logic/Commands/ScheduleAdvertCmd.h"

#include "cmadtv/DragDrop/AdvertDragController.h"

using namespace CMadTV;

AdvertDropController::AdvertDropController( PlayerContextPtr _spContext, const CFoundation::Date& _Date, Hour _eSlot )
:   m_spContext( _spContext ),
    m_Date( _Date ),
    m_eSlot( _eSlot )
{
}

AdvertDropController::~AdvertDropController()
{
}

bool AdvertDropController::Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& /*_vDragPosition*/, const CFoundation::Vector2Di& /*_vDropPosition*/ ) 
{
    // Advert drag data expected
    AdvertDragDataPtr spDragData = dynamic_pointer_cast< AdvertDragData >( _spDragData );
    if ( spDragData == NULL )
    {
        return false;
    }

    AdvertPtr spAdvert = spDragData->m_spAdvert;

    // Try to schedule advert at this slot
    ScheduleAdvertCmdPtr spCmd( new ScheduleAdvertCmd( m_spContext, spAdvert, m_Date, m_eSlot ) );
    Unsigned32 u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
    switch ( u32ReturnValue )
    {
    case ScheduleAdvertCmd::ERROR_INVALID_DATE:
        {
        }
        break;
    }

    return u32ReturnValue == 0;
}


