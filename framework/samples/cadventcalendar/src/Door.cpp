#include "Door.h"

#include "AdventCalendarApp.h"

#include "cgui/Gui/GuiObject.h"

using namespace CAdventCalendar;

Door::Door( CGui::TextBoxPtr _spTbDoor, CGui::ImagePtr _spImgSuprise )
{
    m_spTbDoor = _spTbDoor;
    m_spImgSurprise = _spImgSuprise;

    // Create delegates
    m_spDelegateClickedDoor = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &Door::OnClickedDoor ) );
    m_spDelegateClickedSurprise = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &Door::OnClickedSurprise ) );

    // Register delegates
    m_spTbDoor->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedDoor );
    m_spImgSurprise->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedSurprise );
}
        
void Door::InitFromXML( const CFoundation::XMLElementPtr _spXMLElement )
{
    m_u8Day = _spXMLElement->GetAttribute( "day" ).ToUnsigned8();

    CFoundation::XMLElementPtr spSurpriseElement = static_pointer_cast< CFoundation::XMLElement >( _spXMLElement->GetFirstChild() );
    if ( spSurpriseElement != NULL )
    {
        m_clSurprise.m_sImage = spSurpriseElement->GetAttribute( "image" );
        m_clSurprise.m_sCaption = spSurpriseElement->GetAttribute( "caption" );
        m_clSurprise.m_sText = spSurpriseElement->GetAttribute( "text" );
    }

    m_spImgSurprise->SetTexture( m_clSurprise.m_sImage );
}
      
Unsigned8 Door::GetDay() const
{
    return m_u8Day;
}

void Door::Open()
{
    m_spTbDoor->SetVisible( false );
}

void Door::OnClickedDoor()
{
    AdventCalendarApp::GetInstance().OnDoorOpened( *this );
}

void Door::OnClickedSurprise()
{
    AdventCalendarApp::GetInstance().OnSurpriseClicked( m_clSurprise );
}

