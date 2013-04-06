#include "cmadtv/Tooltip/BroadcastTooltipController.h"

#include "cgui/Gui/Panel.h"

#include "cmadtv/Data/World/Hour.h"
#include "cmadtv/Tooltip/BroadcastTooltip.h"

using namespace CMadTV;

BroadcastTooltipPtr BroadcastTooltipController::s_spTooltip;  

void BroadcastTooltipController::DestroyTooltip()
{
    s_spTooltip.reset();
}

BroadcastTooltipController::BroadcastTooltipController( PlayerContextPtr _spContext, BroadcastPtr _spBroadcast )
:   m_spContext( _spContext ),
    m_spBroadcast( _spBroadcast )
{
}

BroadcastTooltipController::~BroadcastTooltipController()
{
}

void BroadcastTooltipController::Start( CGui::GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition )
{
    // Create tooltip if not already created
    if ( s_spTooltip == NULL )
    {
        s_spTooltip = BroadcastTooltip::Create();

        // Check if tooltip could be created
        if ( s_spTooltip == NULL )
        {
            return;
        }
    }

    // Set broadcast
    s_spTooltip->SetContext( m_spContext );
    s_spTooltip->SetBroadcast( m_spBroadcast );
    s_spTooltip->SetOwner( m_spOwner );

    // Set tooltip
    SetTooltip( s_spTooltip->GetPanel() );

    PointerTooltipController::Start( _spObject, _vPointerPosition );
}
        
void BroadcastTooltipController::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;
}
        
void BroadcastTooltipController::SetBroadcast( BroadcastPtr _spBroadcast )
{
    m_spBroadcast = _spBroadcast;
}
        
void BroadcastTooltipController::SetOwner( PlayerPtr _spOwner )
{
    m_spOwner = _spOwner;
}
       
BroadcastPtr BroadcastTooltipController::GetBroadcast() const
{
    return m_spBroadcast;
}
