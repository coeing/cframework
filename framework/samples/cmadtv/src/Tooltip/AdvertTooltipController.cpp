#include "cmadtv/Tooltip/AdvertTooltipController.h"

#include "cmadtv/Tooltip/AdvertTooltip.h"

using namespace CMadTV;

AdvertTooltipPtr AdvertTooltipController::s_spTooltip;
        
void AdvertTooltipController::DestroyTooltip()
{
    s_spTooltip.reset();
}

AdvertTooltipController::AdvertTooltipController( PlayerContextPtr _spContext, AdvertPtr _spAdvert )
:   m_spContext( _spContext ),
    m_spAdvert( _spAdvert )
{
}

AdvertTooltipController::~AdvertTooltipController()
{
}

void AdvertTooltipController::Start( CGui::GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition )
{
    // Create tooltip if not already created
    if ( s_spTooltip == NULL )
    {
        s_spTooltip = AdvertTooltip::Create();

        // Check if tooltip could be created
        if ( s_spTooltip == NULL )
        {
            return;
        }
    }

    // Set Advert
    s_spTooltip->Set( m_spContext, m_spAdvert );

    // Set tooltip
    SetTooltip( s_spTooltip->GetPanel() );

    PointerTooltipController::Start( _spObject, _vPointerPosition );
}
        
void AdvertTooltipController::Set( PlayerContextPtr _spContext, AdvertPtr _spAdvert )
{
    m_spContext = _spContext;
    m_spAdvert = _spAdvert;
}





