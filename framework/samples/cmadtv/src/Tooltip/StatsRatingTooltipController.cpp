#include "cmadtv/Tooltip/StatsRatingTooltipController.h"

#include "cgui/Gui/Panel.h"

#include "cmadtv/Data/Player/BlockStat.h"
#include "cmadtv/Tooltip/StatsRatingTooltip.h"

using namespace CMadTV;

StatsRatingTooltipPtr StatsRatingTooltipController::s_spTooltip;  

void StatsRatingTooltipController::DestroyTooltip()
{
    s_spTooltip.reset();
}

StatsRatingTooltipController::StatsRatingTooltipController( BlockStatPtr _spBlockStat )
:   m_spBlockStat( _spBlockStat )
{
}

StatsRatingTooltipController::~StatsRatingTooltipController()
{
}

void StatsRatingTooltipController::Start( CGui::GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vPointerPosition )
{
    // Create tooltip if not already created
    if ( s_spTooltip == NULL )
    {
        s_spTooltip = StatsRatingTooltip::Create();

        // Check if tooltip could be created
        if ( s_spTooltip == NULL )
        {
            return;
        }
    }

    // Set BlockStat
    s_spTooltip->SetBlockStat( m_spBlockStat );

    if ( m_spBlockStat == NULL ||
        m_spBlockStat->GetBroadcast() == NULL )
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
              
void StatsRatingTooltipController::SetBlockStat( BlockStatPtr _spBlockStat )
{
    m_spBlockStat = _spBlockStat;
}
    