#include "cmadtv/Tooltip/SammyTooltipController.h"

#include "cgui/Gui/Panel.h"

#include "cmadtv/Data/Game/Sammy/Sammy.h"
#include "cmadtv/Tooltip/SammyTooltip.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// SammyTooltipController
///
////////////////////////////////////////////////////////////////////////////////////////////

SammyTooltipPtr SammyTooltipController::s_spTooltip;  

void SammyTooltipController::DestroyTooltip()
{
    s_spTooltip.reset();
}

SammyTooltipController::SammyTooltipController( SammyPtr _spSammy )
:   m_spSammy( _spSammy )
{
}

SammyTooltipController::~SammyTooltipController()
{
}

void SammyTooltipController::Start( CGui::GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition )
{
    // Create tooltip if not already created
    if ( s_spTooltip == NULL )
    {
        s_spTooltip = SammyTooltip::Create();

        // Check if tooltip could be created
        if ( s_spTooltip == NULL )
        {
            return;
        }
    }

    // Setup tooltip
    s_spTooltip->SetSammy( m_spSammy );

    if ( m_spSammy == NULL )
    {
        SetTooltip( CGui::GuiObjectPtr() );
    }
    else
    {
        // Set tooltip
        SetTooltip( s_spTooltip->GetPanel() );
    }

    PointerTooltipController::Start( _spObject, _vPointerPosition );
}
              
void SammyTooltipController::SetSammy( SammyPtr _spSammy )
{
    m_spSammy = _spSammy;
}
       
SammyPtr SammyTooltipController::GetSammy() const
{
    return m_spSammy;
}
 