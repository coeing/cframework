#include "cmadtv/Windows/AdvertBlock.h"

#include "cfoundation/Debug/Makros.h"

#include "cgui/Gui/GuiMain.h"

#include "cmadtv/Data/Advert.h"

using namespace CMadTV;

AdvertBlockPtr AdvertBlock::Create( AdvertPtr _spAdvert )
{
    // Check which xml to use
    CFoundation::String strXML = "data/windows/AdvertBlock.xml";

    // Create panel
    CGui::PanelPtr spPnlBlock = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( strXML );

    // Create Advert block
    AdvertBlockPtr spAdvertBlock( new AdvertBlock( spPnlBlock, _spAdvert ) );

    return spAdvertBlock;
}

AdvertBlock::AdvertBlock( CGui::PanelPtr _spPnlRoot, AdvertPtr _spAdvert )
{
    // Store pointer to root panel
    m_spPnlRoot = _spPnlRoot;

    // Get background
    m_spImgBackground = static_pointer_cast< CGui::Image >( m_spPnlRoot->FindChild( "ImgBackground" ) );

    // Get name textbox
    m_spTbName = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbName" ) );

    // Get sub text textbox
    m_spTbSubText = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbSubText" ) );

    // Set advert
    SetAdvert( _spAdvert );
}

AdvertBlock::~AdvertBlock()
{
    CGui::GuiMain::GetInstance().Delete( m_spPnlRoot );
}
        
void AdvertBlock::SetAdvert( AdvertPtr _spAdvert )
{
    if ( _spAdvert == m_spAdvert )
    {
        return;
    }

    m_spAdvert = _spAdvert;

    // Set name
    m_spTbName->SetText( m_spAdvert->GetName() );
}

AdvertPtr AdvertBlock::GetAdvert() const
{
    return m_spAdvert;
}

CGui::PanelPtr AdvertBlock::GetPanel() const
{
    return m_spPnlRoot;
}
        
void AdvertBlock::SetActive( bool _bActive )
{
    m_spImgBackground->SetActive( _bActive );
}
        
void AdvertBlock::SetSubText( const CFoundation::String& _strText )
{
    // Set text
    m_spTbSubText->SetText( _strText );
}
