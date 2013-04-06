#include "SurpriseWindow.h"

#include "Door.h"

using namespace CAdventCalendar;

SurpriseWindow::SurpriseWindow()
{
    m_spDelegateClickedLeft = CSystem::InputEventDelegatePtr( new CSystem::InputEventDelegate( this, &SurpriseWindow::OnClickedLeft ) );
}

const CFoundation::String SurpriseWindow::GetXMLFileName()
{
	return "./data/SurpriseWindow.xml";
};

void SurpriseWindow::InitWindow()
{
    m_pImgImage = GetImage( "ImgImage" );
    m_pTbCaption = GetTextBox( "TbCaption" );
    m_pTbText = GetTextBox( "TbText" );

    // Register delegate
    CSystem::Input::GetInstance().RegisterDelegate( CSystem::INPUT_EVENT_LMB_PRESSED, m_spDelegateClickedLeft );
}
		
void SurpriseWindow::DeinitWindow()
{
    // Remove delegate
    CSystem::Input::GetInstance().RemoveDelegate( CSystem::INPUT_EVENT_LMB_PRESSED, m_spDelegateClickedLeft );
}

void SurpriseWindow::Init( const Surprise& _clSurprise )
{
    m_pSurprise = &_clSurprise;

    m_pImgImage->SetTexture( _clSurprise.m_sImage );
    m_pTbCaption->SetText( _clSurprise.m_sCaption );
    m_pTbText->SetText( _clSurprise.m_sText );
}
		
void SurpriseWindow::OnClickedLeft()
{
    Close();
}
