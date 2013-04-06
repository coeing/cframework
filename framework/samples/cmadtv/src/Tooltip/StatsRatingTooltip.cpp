#include "cmadtv/Tooltip/StatsRatingTooltip.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/TextBox.h"

#include "cmadtv/Data/Broadcast.h"
#include "cmadtv/Data/Genre.h"
#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Player/BlockStat.h"

using namespace CMadTV;

StatsRatingTooltipPtr StatsRatingTooltip::Create()
{
    // Create panels
    CGui::PanelPtr spPnlBroadcast = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( "data/windows/StatsRatingTooltip.xml" );
    CGui::PanelPtr spPnlSeries;// = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( "data/windows/StatsRatingTooltipSeries.xml" );

    // Create tooltip
    StatsRatingTooltipPtr spTooltip( new StatsRatingTooltip( spPnlBroadcast, spPnlSeries ) );
    return spTooltip;
}
        
CFoundation::String StatsRatingTooltip::GetXMLFileName()
{
    return "data/windows/StatsRatingTooltip.xml";
}

StatsRatingTooltip::StatsRatingTooltip( CGui::PanelPtr _spPnlRootBroadcast, CGui::PanelPtr _spPnlRootSeries )
:   m_spPnlRootBroadcast( _spPnlRootBroadcast ),
    m_spPnlRootSeries( _spPnlRootSeries )
{
    // Get controls
    if ( m_spPnlRootBroadcast != NULL )
    {
        m_spTbBroadcastTitle = static_pointer_cast< CGui::TextBox >( m_spPnlRootBroadcast->FindDescendant( "TbTitle" ) );
        m_spTbBroadcastGenre = static_pointer_cast< CGui::TextBox >( m_spPnlRootBroadcast->FindDescendant( "TbGenre" ) );
        m_spTbBroadcastBlock = static_pointer_cast< CGui::TextBox >( m_spPnlRootBroadcast->FindDescendant( "TbBlock" ) );
        m_spTbBroadcastSpectators = static_pointer_cast< CGui::TextBox >( m_spPnlRootBroadcast->FindDescendant( "TbSpectators" ) );
    }

    if ( m_spPnlRootSeries != NULL )
    {
        m_spTbSeriesTitle = static_pointer_cast< CGui::TextBox >( m_spPnlRootSeries->FindDescendant( "TbTitle" ) );
        m_spTbSeriesGenre = static_pointer_cast< CGui::TextBox >( m_spPnlRootSeries->FindDescendant( "TbGenre" ) );
        m_spTbSeriesBlock = static_pointer_cast< CGui::TextBox >( m_spPnlRootSeries->FindDescendant( "TbBlock" ) );
        m_spTbSeriesSpectators = static_pointer_cast< CGui::TextBox >( m_spPnlRootSeries->FindDescendant( "TbSpectators" ) );
    }
}

StatsRatingTooltip::~StatsRatingTooltip()
{
}
       
void StatsRatingTooltip::SetContext( PlayerContextPtr _spContext )
{
    m_spContext = _spContext;

    UpdateTooltip();
}

void StatsRatingTooltip::SetBlockStat( BlockStatPtr _spStat )
{
    m_spStat = _spStat;

    UpdateTooltip();
}
      
CGui::PanelPtr StatsRatingTooltip::GetPanel() const
{
    // TODO Return series tooltip or broadcast tooltip
    return m_spPnlRootBroadcast;
}

void StatsRatingTooltip::UpdateTooltip()
{
    BroadcastPtr spBroadcast = m_spStat->GetBroadcast();
    if ( spBroadcast == NULL )
    {
        return;
    }

    // TODO Fill broadcast or series tooltip
    m_spTbBroadcastTitle->SetText( spBroadcast->GetTitle() );
    m_spTbBroadcastGenre->SetText( spBroadcast->GetGenre()->GetName() );
    m_spTbBroadcastBlock->SetText( CFoundation::String::Build( "%d. block of %d", m_spStat->GetBroadcastOrder() + 1, spBroadcast->GetNumBlocks() ) );
    m_spTbBroadcastSpectators->SetText( CFoundation::String::Build( "%s (%.0f%%)", CFoundation::String::MakeMillionString( m_spStat->GetBroadcastSpectators() ).c_str(), m_spStat->GetBroadcastRating() * 100 ) );
}
