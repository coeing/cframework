#include "cgui/Gui/ScrollPanel.h"

#include "cfoundation/XML/XMLElement.h"

#include "cgui/Gui/CtrlStyle.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/ScrollBar.h"
#include "cgui/Gui/SizeMode.h"

#include "cgui/Messages/ScrollMsg.h"

using namespace CGui;

ScrollPanel::ScrollPanel( const CFoundation::String &sName )
:	Decorator( sName ),
	m_eScrollingHoriz( SCROLLING_AUTO ),
	m_eScrollingVert( SCROLLING_AUTO )
{
    // Create delegates
    m_spDelegateViewPanelSizeChanged = SizeEventDelegate0Ptr( new SizeEventDelegate0( this, &ScrollPanel::OnViewPanelSizeChanged ) );
}

const CFoundation::String ScrollPanel::GetTypeID()
{
	return "scrollpanel";
}

const CFoundation::String ScrollPanel::GetTypeId() const
{
	return ScrollPanel::GetTypeID();
}
		
void ScrollPanel::Clear()
{
    m_spScrollBarHoriz.reset();
    m_spScrollBarVert.reset();
    m_spPnlClipping.reset();

    m_spViewPanel->RemoveDelegate( SIZE_EVENT_SIZE_CHANGED, m_spDelegateViewPanelSizeChanged );
    m_spViewPanel.reset();

    Decorator::Clear();
}

//void ScrollPanel::ProcessMessage( const CFoundation::Message &clMessage )
//{
//	//if( GetReference() != NULL &&
//	//	clMessage.IsOriginalReceiver( *GetReference() ) )
//	//{
//
//	//}
//
//	const ScrollMsg *pScrollMsg = clMessage.CastTo< ScrollMsg >();
//	if( pScrollMsg != NULL )
//	{
//		if ( pScrollMsg->GetObject() == m_spScrollBarHoriz )
//		{
//			if( GetReference() != NULL )
//			{
//				ScrollMsg clNewMsg = *pScrollMsg;
//				// clNewMsg.SetSender( this );
//				clNewMsg.SetReceiver( GetReference() );
//
//				Integer32 i32NewX = static_cast< Integer32 >( ( GetReference()->GetWidth() - GetVisibleWidth() ) * pScrollMsg->GetScrollPosition() );
//				m_clRefPosition.SetX( -i32NewX );
//                SetReferencePosition( m_clRefPosition );
//
//				clNewMsg.SetScrollOffset( m_clRefPosition.GetX() );
//				//gMessageSystem.Send( clNewMsg );
//			}
//
//			return;
//		}
//		else if ( pScrollMsg->GetObject() == m_spScrollBarVert )
//		{
//			if( GetReference() != NULL )
//			{
//				ScrollMsg clNewMsg = *pScrollMsg;
//				//clNewMsg.SetSender( this );
//				clNewMsg.SetReceiver( GetReference() );
//
//				Integer32 i32NewY = static_cast< Integer32 >( ( GetReference()->GetHeight() - GetVisibleHeight() ) * pScrollMsg->GetScrollPosition() );
//				m_clRefPosition.SetY( -i32NewY );
//                SetReferencePosition( m_clRefPosition );
//
//				clNewMsg.SetScrollOffset( m_clRefPosition.GetY() );
//				//gMessageSystem.Send( clNewMsg );
//			}
//
//			return;
//		}
//	}
//
//	Decorator::ProcessMessage( clMessage );
//}

void ScrollPanel::UpdateScrollBarSizes()
{
    Unsigned32 u32BarSize = 20;

    // Check which scroll bars are needed
    bool bScrollBarHoriz = GetWidth() < m_spViewPanel->GetWidth();
    bool bScrollBarVert = GetHeight() < m_spViewPanel->GetHeight();

    // If one needed check if other also needed then
    if ( bScrollBarHoriz &&
         !bScrollBarVert )
    {
        bScrollBarVert = GetHeight() - u32BarSize < m_spViewPanel->GetHeight();
    }
    if ( !bScrollBarHoriz &&
         bScrollBarVert )
    {
        bScrollBarHoriz = GetWidth() - u32BarSize < m_spViewPanel->GetWidth();
    }

    // Update scroll bar visibilities
    if ( m_eScrollingHoriz == SCROLLING_AUTO )
    {
        ShowScrollBarHoriz( bScrollBarHoriz );
    }
    if ( m_eScrollingHoriz == SCROLLING_AUTO )
    {
        ShowScrollBarVert( bScrollBarVert );
    }

    // Set sub object sizes
    Unsigned32 u32Width = bScrollBarVert ? GetWidth() - u32BarSize : GetWidth();
    if ( m_spScrollBarHoriz != NULL )
        m_spScrollBarHoriz->SetCustomSize( u32Width, u32BarSize );

    Unsigned32 u32Height = bScrollBarHoriz ? GetHeight() - u32BarSize : GetHeight();
    if ( m_spScrollBarVert != NULL )
        m_spScrollBarVert->SetCustomSize( u32BarSize, u32Height );

    u32Width = bScrollBarVert ? GetWidth() - u32BarSize : GetWidth();
    u32Height = bScrollBarHoriz ? GetHeight() - u32BarSize : GetHeight();
    m_spPnlClipping->SetCustomSize( u32Width, u32Height );

    // Update scroll bar settings
    UpdateScrollBarSettings();
}
		
void ScrollPanel::UpdateScrollBarSettings()
{
    if ( m_spScrollBarHoriz != NULL )
    {
        m_spScrollBarHoriz->SetTotalSize( m_spViewPanel->GetWidth() );
        m_spScrollBarHoriz->SetVisibleSize( m_spPnlClipping->GetWidth() );
    }

    if ( m_spScrollBarVert != NULL )
    {
        m_spScrollBarVert->SetTotalSize( m_spViewPanel->GetHeight() );
        m_spScrollBarVert->SetVisibleSize( m_spPnlClipping->GetHeight() );
    }
}

void ScrollPanel::ShowScrollBarHoriz( bool bEnable )
{
	if( m_spScrollBarHoriz != NULL )
	{
		m_spScrollBarHoriz->SetVisible( bEnable );
	}
	else if( bEnable )
	{
		// Create standard scrollbar
        m_spScrollBarHoriz = GuiMain::GetInstance().CreateDefault< ScrollBar >( "theScrollBarHoriz" );
		m_spScrollBarHoriz->SetOrientation( ScrollBar::SCROLLBAR_HORIZONTAL );
		m_spScrollBarHoriz->SetResizable( false );
		m_spScrollBarHoriz->SetDraggable( false );
		AddPieceLast( m_spScrollBarHoriz );
        m_spScrollBarHoriz->SetCustomSize( GetWidth() - 20, 20 );
        m_spScrollBarHoriz->SetLocalAttachPoint( ATTACH_POINT_BOTTOM_RIGHT );
        m_spScrollBarHoriz->SetReferenceAttachPoint( ATTACH_POINT_BOTTOM_RIGHT );

		// Set position
		m_spScrollBarHoriz->SetOffset( CFoundation::Vector2Di( -20, 0 ) );
	}
}

void ScrollPanel::ShowScrollBarVert( bool bEnable )
{
	if( m_spScrollBarVert != NULL )
	{
		m_spScrollBarVert->SetVisible( bEnable );
	}
	else if( bEnable )
	{
		// Create standard scrollbar
        m_spScrollBarVert = GuiMain::GetInstance().CreateDefault< ScrollBar >( "theScrollBarVert" );
        m_spScrollBarVert->SetOrientation( ScrollBar::SCROLLBAR_VERTICAL );
		m_spScrollBarVert->SetResizable( false );
		m_spScrollBarVert->SetDraggable( false );
		AddPieceLast( m_spScrollBarVert );
        m_spScrollBarVert->SetCustomSize( 20, GetHeight() - 20 );
        m_spScrollBarVert->SetLocalAttachPoint( ATTACH_POINT_TOP_RIGHT );
        m_spScrollBarVert->SetReferenceAttachPoint( ATTACH_POINT_TOP_RIGHT );

		// Set position
        m_spScrollBarVert->SetOffset( CFoundation::Vector2Di( 0, 0 ) );
	}
}

bool ScrollPanel::IsScrollBarHorizVisible() const
{
	return ( m_spScrollBarHoriz != NULL &&
			 m_spScrollBarHoriz->IsVisible() );
}

bool ScrollPanel::IsScrollBarVertVisible() const
{
	return ( m_spScrollBarVert != NULL &&
			 m_spScrollBarVert->IsVisible() );
}

Unsigned32 ScrollPanel::GetVisibleWidth() const
{
	if( IsScrollBarVertVisible() )
	{
		return GetWidth() - m_spScrollBarVert->GetWidth();
	}
	else
	{
		return GetWidth();
	}
}

Unsigned32 ScrollPanel::GetVisibleHeight() const
{
	if( IsScrollBarHorizVisible() )
	{
		return GetHeight() - m_spScrollBarHoriz->GetHeight();
	}
	else
	{
		return GetHeight();
	}
}

Unsigned32 ScrollPanel::GetScrollPositionX() const
{
	if( m_spScrollBarHoriz != NULL )
	{
		return m_spScrollBarHoriz->GetScrollPosition();
	}
	else
	{
		return 0;
	}
}

Unsigned32 ScrollPanel::GetScrollPositionY() const
{
	if( m_spScrollBarVert != NULL )
	{
		return m_spScrollBarVert->GetScrollPosition();
	}
	else
	{
		return 0;
	}
}

void ScrollPanel::SetReference( GuiObjectPtr _spReference )
{
    if ( _spReference == GetReference() )
    {
        return;
    }

    // Release old reference
    GuiObjectPtr spOldReference = GetReference();
    if ( spOldReference != NULL )
    {
        m_spViewPanel->RemoveChild( spOldReference );
    }

    Decorator::SetReference( _spReference );

    // Bind new reference
	if ( _spReference != NULL )
	{
        _spReference->RemoveFromParent();
		m_spViewPanel->AddChildLast( _spReference );
	}
}

void ScrollPanel::SetScrollingHoriz( ScrollingType eType )
{
	m_eScrollingHoriz = eType;
	if( m_eScrollingHoriz == SCROLLING_OFF )
	{
		ShowScrollBarHoriz( false );
	}
	else if( m_eScrollingHoriz == SCROLLING_ON )
	{
		ShowScrollBarHoriz( true );
	}
	else
	{
		UpdateScrollBarSizes();
	}
}

void ScrollPanel::SetScrollingVert( ScrollingType eType )
{
	m_eScrollingVert = eType;
	if( m_eScrollingVert == SCROLLING_OFF )
	{
		ShowScrollBarVert( false );
	}
	else if( m_eScrollingVert == SCROLLING_ON )
	{
		ShowScrollBarVert( true );
	}
	else
	{
		UpdateScrollBarSizes();
	}
}
        
void ScrollPanel::SetSize( const CFoundation::Vector2Du& _clSize )
{
    if ( _clSize == GetSize() )
    {
        return;
    }

    Decorator::SetSize( _clSize );
    UpdateScrollBarSizes();
}

void ScrollPanel::NewReferenceSize( Unsigned32 u32Width, Unsigned32 u32Height )
{
	Decorator::NewReferenceSize( u32Width, u32Height );

	GuiObjectPtr pRef = GetReference();
	if( pRef != NULL )
	{
		UpdateScrollBarSizes();
	}
}

void ScrollPanel::FitReference()
{
	GuiObjectPtr pRef = GetReference();
	if( pRef != NULL )
	{
		SetReferencePosition( m_clRefPosition );
	}
	UpdateScrollBarSizes();
}

Unsigned32 ScrollPanel::GetLeftMargin() const
{
	//if( m_spScrollBarVert != NULL &&
	//	m_spScrollBarVert->IsVisible() )
	//{
	//	return m_spScrollBarVert->GetWidth();
	//}
	//else
	{
		return 0;
	}
}

Unsigned32 ScrollPanel::GetRightMargin() const
{
	if( IsScrollBarVertVisible() )
	{
		return m_spScrollBarVert->GetWidth();
	}
	else
	{
		return 0;
	}
}

Unsigned32 ScrollPanel::GetTopMargin() const
{
	//if( m_spScrollBarHoriz != NULL &&
	//	m_spScrollBarHoriz->IsVisible() )
	//{
	//	return m_spScrollBarHoriz->GetWidth();
	//}
	//else
	{
		return 0;
	}
}

Unsigned32 ScrollPanel::GetBottomMargin() const
{
	if( IsScrollBarHorizVisible() )
	{
		return m_spScrollBarHoriz->GetHeight();
	}
	else
	{
		return 0;
	}
}
        
void ScrollPanel::OnViewPanelSizeChanged()
{
    UpdateScrollBarSizes();
}

const CFoundation::String ScrollPanelCreator::GetTypeId() const
{
	return ScrollPanel::GetTypeID();
}

GuiObjectPtr ScrollPanelCreator::CreateObject( const CFoundation::String &sName ) const
{
	ScrollPanelPtr spScrollPanel( new ScrollPanel( sName ) );

    InitDefault( spScrollPanel );

    return spScrollPanel;
}

void ScrollPanelCreator::InitDefault( GuiObjectPtr _spObject ) const
{
	DecoratorCreator::InitDefault( _spObject );

	ScrollPanelPtr spScrollPanel = static_pointer_cast< ScrollPanel >( _spObject );
    
    // Create view panel
    PanelPtr spViewPanel = GuiMain::GetInstance().CreateDefault< Panel >( "theViewPanel" );
    spScrollPanel->m_spViewPanel = spViewPanel;
    
    // Init view panel
    spViewPanel->SetSizeMode( SIZE_MODE_CONTENT );
    spViewPanel->RegisterDelegate( SIZE_EVENT_SIZE_CHANGED, spScrollPanel->m_spDelegateViewPanelSizeChanged );
    
    // Create clipping panel
    PanelPtr spPnlClipping = GuiMain::GetInstance().CreateDefault< Panel >( "theClippingPanel" );
    spScrollPanel->AddPieceLast( spPnlClipping );
    spScrollPanel->m_spPnlClipping = spPnlClipping;

    // Init clipping panel
    spPnlClipping->AddChildFirst( spViewPanel );
    spPnlClipping->SetBgColor( CFoundation::Color( 0.0f, 1.0f, 0.0f ) );
}

void ScrollPanelCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	DecoratorCreator::InitFromXML( _spObject, _spXMLElement );

	ScrollPanelPtr spScrollPanel = static_pointer_cast< ScrollPanel >( _spObject );

    spScrollPanel->ShowScrollBarHoriz( true );
    spScrollPanel->ShowScrollBarVert( true );

    // Add children to view panel
    CFoundation::XMLNodeCPtr spChildNode = _spXMLElement->GetFirstChild();
    while ( spChildNode != NULL )
    {
        if ( spChildNode->IsElement() )
        {
            CFoundation::XMLElementCPtr spChildElement = static_pointer_cast< const CFoundation::XMLElement >( spChildNode );

            const CFoundation::String& strName = spChildElement->GetName();

            if ( strName == "scrollbar_hori" )
            {
                spScrollPanel->m_spScrollBarHoriz->SetStyle( CtrlStylePtr( new CtrlStyle( spChildElement ) ) );
            }
            else if ( strName == "scrollbar_vert" )
            {
                spScrollPanel->m_spScrollBarVert->SetStyle( CtrlStylePtr( new CtrlStyle( spChildElement ) ) );
            }
            else
            {
                GuiObjectPtr spChild = GuiMain::GetInstance().CreateFromXML( spChildElement );
                if ( spChild != NULL )
                {
                    spScrollPanel->SetReference( spChild );
                }
            }
        }
        spChildNode = spChildNode->GetNextSibling();
    }
}
//
//void ScrollPanelCreator::SaveToXML( const GuiObject &clObject, CFoundation::XMLDocument &clDoc, CFoundation::XMLElement &clXMLElement ) const
//{
//	DecoratorCreator::SaveToXML( clObject, clDoc, clXMLElement );
//
//	//const ScrollPanel &clScrollPanel = dynamic_cast<const ScrollPanel&>( clObject );
//}
