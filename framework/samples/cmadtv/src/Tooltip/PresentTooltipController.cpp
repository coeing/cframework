#include "cmadtv/Tooltip/PresentTooltipController.h"

#include "cgui/Gui/Panel.h"

#include "cmadtv/Data/Betty/Present.h"
#include "cmadtv/Tooltip/PresentTooltip.h"

using namespace CMadTV;

PresentTooltipPtr PresentTooltipController::s_spTooltip;  

void PresentTooltipController::DestroyTooltip()
{
    s_spTooltip.reset();
}

PresentTooltipController::PresentTooltipController( PresentPtr _spPresent )
:   m_spPresent( _spPresent )
{
}

PresentTooltipController::~PresentTooltipController()
{
}

void PresentTooltipController::Start( CGui::GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition )
{
    // Create tooltip if not already created
    if ( s_spTooltip == NULL )
    {
        s_spTooltip = PresentTooltip::Create();

        // Check if tooltip could be created
        if ( s_spTooltip == NULL )
        {
            return;
        }
    }

    // Setup tooltip
    s_spTooltip->SetGame( m_spGame );
    s_spTooltip->SetPresent( m_spPresent );

    if ( m_spPresent == NULL )
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
        
void PresentTooltipController::SetGame( GamePtr _spGame )
{
    m_spGame = _spGame;
}
        
GamePtr PresentTooltipController::GetGame() const
{
    return m_spGame;
}
              
void PresentTooltipController::SetPresent( PresentPtr _spPresent )
{
    m_spPresent = _spPresent;
}
       
PresentPtr PresentTooltipController::GetPresent() const
{
    return m_spPresent;
}
 