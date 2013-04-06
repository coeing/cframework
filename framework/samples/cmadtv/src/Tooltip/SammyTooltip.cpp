#include "cmadtv/Tooltip/SammyTooltip.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/TextBox.h"

#include "cmadtv/Data/Game/Sammy/Sammy.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// SammyTooltip
///
////////////////////////////////////////////////////////////////////////////////////////////

SammyTooltipPtr SammyTooltip::Create()
{
    // Create panels
    CGui::PanelPtr spPnlRoot = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( "data/windows/SammyTooltip.xml" );

    // Create tooltip
    SammyTooltipPtr spTooltip( new SammyTooltip( spPnlRoot ) );
    return spTooltip;
}
        
CFoundation::String SammyTooltip::GetXMLFileName()
{
    return "data/windows/SammyTooltip.xml";
}

SammyTooltip::SammyTooltip( CGui::PanelPtr _spPnlRoot )
:   m_spPnlRoot( _spPnlRoot )
{
    // Get controls
    if ( m_spPnlRoot != NULL )
    {
        m_spTbName = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindDescendant( "TbName" ) );
    }
}

SammyTooltip::~SammyTooltip()
{
}
       
void SammyTooltip::SetSammy( SammyPtr _spSammy )
{
    m_spSammy = _spSammy;

    UpdateTooltip();
}
      
CGui::PanelPtr SammyTooltip::GetPanel() const
{
    return m_spPnlRoot;
}

void SammyTooltip::UpdateTooltip()
{
    Sammy::Type eType = m_spSammy != NULL ? m_spSammy->GetType() : Sammy::TYPE_NONE;
    CFoundation::String strName;
    switch ( eType )
    {
    case Sammy::TYPE_CULTURAL:
        {
            strName = "Sammy for the best cultural program";
        }
        break;
    case Sammy::TYPE_NEWS:
        {
            strName = "Sammy for the best news";
        }
        break;
    }
    m_spTbName->SetText( strName );
}
