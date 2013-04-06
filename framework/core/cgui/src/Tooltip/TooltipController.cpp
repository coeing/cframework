#include "cgui/Tooltip/TooltipController.h"

using namespace CGui;

TooltipController::TooltipController()
{
}

void TooltipController::Start( GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition )
{
    m_spObject = _spObject;

    // Dispatch event
    DispatchEvent( EVENT_TOOLTIP_CONTROLLER_STARTED, GetSharedPtr< TooltipController >(), m_spObject, _vPointerPosition );
}
        
void TooltipController::Continue( const CFoundation::Vector2Di& /*_vPointerMovement*/ )
{
}
        
void TooltipController::End()
{
    GuiObjectPtr spObject = m_spObject;

    // Reset object
    m_spObject.reset();

    // Dispatch event
    CFoundation::Vector2Di vTmp;
    DispatchEvent( EVENT_TOOLTIP_CONTROLLER_ENDED, GetSharedPtr< TooltipController >(), spObject, vTmp );
}
     