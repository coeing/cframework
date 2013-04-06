#include "cmadtv/Tooltip/PresentTooltip.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/TextBox.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Betty/Present.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Game/Game.h"

using namespace CMadTV;

PresentTooltipPtr PresentTooltip::Create()
{
    // Create panels
    CGui::PanelPtr spPnlRoot = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( "data/windows/PresentTooltip.xml" );

    // Create tooltip
    PresentTooltipPtr spTooltip( new PresentTooltip( spPnlRoot ) );
    return spTooltip;
}
        
CFoundation::String PresentTooltip::GetXMLFileName()
{
    return "data/windows/PresentTooltip.xml";
}

PresentTooltip::PresentTooltip( CGui::PanelPtr _spPnlRoot )
:   m_spPnlRoot( _spPnlRoot )
{
    // Get controls
    if ( m_spPnlRoot != NULL )
    {
        m_spTbPresentName = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindDescendant( "TbName" ) );
        m_spTbPresentPrice = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindDescendant( "TbPrice" ) );
        m_spTbPresentGiven = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindDescendant( "TbGiven" ) );
    }
}

PresentTooltip::~PresentTooltip()
{
}
        
void PresentTooltip::SetGame( GamePtr _spGame )
{
    if ( _spGame == m_spGame )
    {
        return;
    }

    m_spGame = _spGame;

    UpdateTooltip();
}
        
GamePtr PresentTooltip::GetGame() const
{
    return m_spGame;
}

void PresentTooltip::SetPresent( PresentPtr _spPresent )
{
    m_spPresent = _spPresent;

    UpdateTooltip();
}
      
CGui::PanelPtr PresentTooltip::GetPanel() const
{
    return m_spPnlRoot;
}

void PresentTooltip::UpdateTooltip()
{
    m_spTbPresentName->SetText( m_spPresent != NULL ? m_spPresent->GetName() : "" );
    m_spTbPresentPrice->SetText( m_spPresent != NULL ? m_spPresent->GetPrice().ToString( CFoundation::CURRENCY_DOLLAR ) : "$0" );

    Unsigned32 u32NumPresents = m_spGame != NULL ? m_spGame->GetPresentCounter( m_spPresent ) : 0;
    m_spTbPresentGiven->SetText( CFoundation::String::Build( "%d. %s", u32NumPresents + 1, u32NumPresents == 0 ? "time" : "times" ) );
}
