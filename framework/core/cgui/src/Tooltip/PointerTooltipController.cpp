#include "cgui/Tooltip/PointerTooltipController.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/GuiObject.h"
#include "cgui/Gui/Layer.h"

using namespace CGui;

PointerTooltipController::PointerTooltipController()
{
}

void PointerTooltipController::Start( GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition )
{
    if ( m_spTooltip != NULL )
    {
        // Make visible
        m_spTooltip->SetVisible( true );

        // Set tooltip offset
        m_vOffset = _vPointerPosition + CFoundation::Vector2Di( 1, 1 );
        UpdateOffset();

        // Add to foreground
        CGui::GuiMain::GetInstance().GetLayer( CGui::LAYER_HOVER ).AddObjectLast( m_spTooltip );
    }

    TooltipController::Start( _spObject, _vPointerPosition );
}
        
void PointerTooltipController::Continue( const CFoundation::Vector2Di& _vPointerMovement )
{
    // Move tooltip
    m_vOffset += _vPointerMovement;
    UpdateOffset();

    TooltipController::Continue( _vPointerMovement );
}
        
void PointerTooltipController::End()
{
    if ( m_spTooltip != NULL )
    {
        // Hide
        m_spTooltip->SetVisible( false );

        // Remove from foreground
        CGui::GuiMain::GetInstance().GetLayer( CGui::LAYER_HOVER ).RemoveObject( m_spTooltip );
    }

    TooltipController::End();
}
        
void PointerTooltipController::SetTooltip( GuiObjectPtr _spTooltip )
{
    m_spTooltip = _spTooltip;
}

GuiObjectPtr PointerTooltipController::GetTooltip() const
{
    return m_spTooltip;
}
        
void PointerTooltipController::UpdateOffset()
{
    if ( m_spTooltip == NULL )
    {
        return;
    }

    // Compute new offset
    CFoundation::Vector2Di vNewOffset = m_vOffset;
    const CFoundation::Vector2Du& vSize = m_spTooltip->GetSize();
    if ( vNewOffset.GetX() + vSize.GetX() > CGui::GuiMain::GetInstance().GetWidth() )
    {
        vNewOffset.SetX( vNewOffset.GetX() - vSize.GetX() );
    }
    if ( vNewOffset.GetY() + vSize.GetY() > CGui::GuiMain::GetInstance().GetHeight() )
    {
        vNewOffset.SetY( vNewOffset.GetY() - vSize.GetY() );
    }

    // Set new offset
    m_spTooltip->SetOffset( vNewOffset );
}
     