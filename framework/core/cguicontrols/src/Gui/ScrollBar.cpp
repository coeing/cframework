#include "cgui/Gui/ScrollBar.h"

#include "cfoundation/XML/XMLElement.h"

#include "cgui/DragDrop/DragController.h"
#include "cgui/DragDrop/DragDropSystem.h"

#include "cgui/Gui/CtrlStyle.h"
#include "cgui/Gui/Button.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Image.h"

#include "cgui/Messages/ScrollMsg.h"

using namespace CGui;

class ScrollBarDragController : public DragController
{
public:
    /// Constructor
    ScrollBarDragController( ScrollBarPtr _spScrollBar )
        : m_spScrollBar( _spScrollBar )
    {
    }

    /// Continues dragging for the current drag object
    virtual void        Continue( const CFoundation::Vector2Di& _vMovement )
    {
        Integer32 i32Movement = 0;
        if ( m_spScrollBar->GetOrientation() == ScrollBar::SCROLLBAR_VERTICAL )
        {
            i32Movement = _vMovement.GetY();
        }
        else
        {
            i32Movement = _vMovement.GetX();
        }

        Integer32 i32NewBarOffset = m_spScrollBar->GetBarOffset() + i32Movement;
        if ( i32NewBarOffset < 0 )
        {
            i32NewBarOffset = 0;
        }
        m_spScrollBar->SetBarOffset( i32NewBarOffset );
    }

    /// Indicates if dropping is valid on the passed object
    virtual bool        IsDropValid( GuiObjectPtr _spObject )
    {
        // Never valid
        return false;
    }

private:
    /// Scroll Bar
    ScrollBarPtr        m_spScrollBar;
};

ScrollBar::ScrollBar( const CFoundation::String &sName )
:	CombinedCtrl( sName ),
	m_eOrientation( SCROLLBAR_VERTICAL ),
    m_u32ScrollStep( 10 ),
	m_u32Size( 100 ),
	m_u32VisibleSize( 10 ),
	m_u32BarOffset( 0 )
{
}

ScrollBar::~ScrollBar()
{
}

const CFoundation::String ScrollBar::GetTypeID()
{
	return "scrollbar";
}

const CFoundation::String ScrollBar::GetTypeId() const
{
	return ScrollBar::GetTypeID();
}

void ScrollBar::Clear()
{
    m_spBtUp.reset();
    m_spBtDown.reset();
    m_spImgBg.reset();
    m_spImgBar.reset();

    CombinedCtrl::Clear();
}

//void ScrollBar::ProcessMessage( const CFoundation::Message &clMessage )
//{
	///*const MouseEventMsg *pMouseMsg = clMessage.CastTo<MouseEventMsg>();
	//if( pMouseMsg != NULL )
	//{
	//	if ( pMouseMsg->GetEvent() == ME_PRESSED &&
	//	     pMouseMsg->GetTrigger() == MOUSE_LEFT )
	//	{
	//		if ( pMouseMsg->GetObject() == m_spImgBg )
	//		{
	//			const CFoundation::Vector2Di &clRelPos = pMouseMsg->GetRelativeMousePosition();

	//			Integer32 i32Position = -1;
	//			Integer32 i32BarSize = -1;
	//			Integer32 i32BarStart = 0;
	//			Integer32 i32BarEnd = 0;
	//			switch( m_eOrientation )
	//			{
	//			case SCROLLBAR_HORIZONTAL:
	//				{
	//					i32Position = clRelPos.GetX();
	//					i32BarSize = m_spImgBar->GetWidth();
	//					i32BarStart = m_spImgBar->GetRelativePosition().GetX() - m_spImgBg->GetRelativePosition().GetX();
	//				}
	//				break;
	//			case SCROLLBAR_VERTICAL:
	//				{
	//					i32Position = clRelPos.GetY();
	//					i32BarSize = m_spImgBar->GetHeight();
	//					i32BarStart = m_spImgBar->GetRelativePosition().GetY() - m_spImgBg->GetRelativePosition().GetY();
	//				}
	//				break;
	//			}
	//			i32BarEnd = i32BarStart + i32BarSize;

	//			if( i32Position >= 0 &&
	//				i32Position < i32BarStart )
	//			{
	//				Integer32 i32NewOffset = static_cast< Integer32 >( m_u32BarOffset - m_u32VisibleSize );
	//				SetBarOffset( std::max( 0, i32NewOffset ) );
	//			}
	//			else if( i32Position > i32BarEnd )
	//			{
	//				Integer32 i32NewOffset = static_cast<Integer32>( GetBarOffset() + m_u32VisibleSize );
	//				SetBarOffset( std::min( static_cast<Integer32>( GetMaxBarOffset() ), i32NewOffset ) );
	//			}
	//		}
	//	}
	//}*/
//	CombinedCtrl::ProcessMessage( clMessage );
//}

void ScrollBar::SetOrientation( Orientation eOrientation )
{
	m_eOrientation = eOrientation;
}

ScrollBar::Orientation ScrollBar::GetOrientation() const
{
	return m_eOrientation;
}
        
void ScrollBar::SetScrollStep( Unsigned32 _u32Step )
{
    m_u32ScrollStep = _u32Step;
}

Unsigned32 ScrollBar::GetScrollStep() const
{
    return m_u32ScrollStep;
}

void ScrollBar::SetTotalSize( Unsigned32 u32Size )
{
	Unsigned32 u32MaxBarOffset = GetMaxBarOffset();

	m_u32Size = u32Size;
	UpdateBar();

	if( GetMaxBarOffset() != u32MaxBarOffset )
	{
		FireScrollMsg();
	}
}

void ScrollBar::SetVisibleSize( Unsigned32 u32Size )
{
	Unsigned32 u32MaxBarOffset = GetMaxBarOffset();

	m_u32VisibleSize = u32Size;
	UpdateBar();

	if( GetMaxBarOffset() != u32MaxBarOffset )
	{
		FireScrollMsg();
	}
}

void ScrollBar::SetBarOffset( Unsigned32 u32Offset )
{
	if( u32Offset != m_u32BarOffset )
	{
		m_u32BarOffset = u32Offset;
		UpdateBar();

		FireScrollMsg();
	}
}

Unsigned32 ScrollBar::GetBarOffset() const
{
	return m_u32BarOffset;
}

void ScrollBar::FireScrollMsg()
{
	// send message
	Unsigned32 u32MaxBarOffset = GetMaxBarOffset();
	Float32 f32ScrollPosition = u32MaxBarOffset == 0 ? 0 : m_u32BarOffset * 1.0 / u32MaxBarOffset;

	//ScrollMsg clMsg( GetSharedPtr< GuiObject >(), f32ScrollPosition, m_u32BarOffset, m_eOrientation );
	//clMsg.SetSender( this );
	//ProcessMessage( clMsg );
}

void ScrollBar::OnPressedUp( GuiObjectPtr /*_spObject*/ )
{
	if ( m_u32BarOffset == 0 )
	{
        return;
    }

    Integer32 i32NewOffset = static_cast< Integer32 >( m_u32BarOffset ) - m_u32ScrollStep;
	SetBarOffset( std::max( 0, i32NewOffset ) );
}

void ScrollBar::OnPressedDown( GuiObjectPtr /*_spObject*/ )
{
	if ( m_u32Size < m_u32VisibleSize ||
		 m_u32BarOffset >= GetMaxBarOffset() )
	{
        return;
    }

    Integer32 i32NewOffset = static_cast< Integer32 >( m_u32BarOffset ) + m_u32ScrollStep;
	SetBarOffset( std::min( static_cast< Integer32 >( GetMaxBarOffset() ), i32NewOffset ) );
}

Unsigned32 ScrollBar::GetScrollPosition() const
{
	return m_u32BarOffset;
}
        
void ScrollBar::SetSize( const CFoundation::Vector2Du& _clSize )
{
    if ( _clSize == GetSize() )
    {
        return;
    }

    CombinedCtrl::SetSize( _clSize );

    UpdateAllSizes();
}

void ScrollBar::UpdateAllSizes()
{
	switch( m_eOrientation )
	{
	case SCROLLBAR_VERTICAL:
		{
			// set button sizes and positions
			Unsigned32 u32ButtonWidth = GetWidth();
			Unsigned32 u32ButtonHeight = u32ButtonWidth;
			if( u32ButtonHeight > GetHeight() / 2 )
			{
				u32ButtonHeight = GetHeight() / 2;
			}

			if( m_spBtUp != NULL )
			{
				m_spBtUp->SetCustomWidth( u32ButtonWidth );
				m_spBtUp->SetCustomHeight( u32ButtonHeight );
				CFoundation::Vector2Di clButtonUpPosition( 0, 0 );
				m_spBtUp->SetOffset( clButtonUpPosition );
			}
			if( m_spBtDown != NULL )
			{
				m_spBtDown->SetCustomWidth( u32ButtonWidth );
				m_spBtDown->SetCustomHeight( u32ButtonHeight );
				CFoundation::Vector2Di clButtonDownPosition( 0, GetHeight() - u32ButtonHeight );
				m_spBtDown->SetOffset( clButtonDownPosition );
			}

			// set bar bg size and position
			if( m_spImgBg != NULL )
			{
				Unsigned32 u32BarBgWidth = GetWidth();
				Unsigned32 u32BarBgHeight = GetHeight() - 2 * u32ButtonHeight;
				m_spImgBg->SetCustomWidth( u32BarBgWidth );
				m_spImgBg->SetCustomHeight( u32BarBgHeight );

				CFoundation::Vector2Di clBarBgPosition( 0, u32ButtonHeight );
				m_spImgBg->SetOffset( clBarBgPosition );
			}

			// set the bar rect
			if( m_spImgBar != NULL )
			{
				Unsigned32 u32BarWidth = GetWidth() - 2;
				Unsigned32 u32BarHeight = GetHeight() - 2 * u32ButtonHeight;
				m_clBarRect.SetSize( u32BarWidth, u32BarHeight );
				m_clBarRect.SetPosition( 1, u32ButtonHeight );
				UpdateBar();
			}
		}
		break;
	case SCROLLBAR_HORIZONTAL:
		{
			// set button sizes and positions
			Unsigned32 u32ButtonHeight = GetHeight();
			Unsigned32 u32ButtonWidth = u32ButtonHeight;
			if( u32ButtonWidth > GetWidth() / 2 )
			{
				u32ButtonWidth = GetWidth() / 2;
			}

			if( m_spBtUp != NULL )
			{
				m_spBtUp->SetCustomWidth( u32ButtonWidth );
				m_spBtUp->SetCustomHeight( u32ButtonHeight );
				CFoundation::Vector2Di clButtonUpPosition( 0, 0 );
				m_spBtUp->SetOffset( clButtonUpPosition );
			}
			if( m_spBtDown != NULL )
			{
				m_spBtDown->SetCustomWidth( u32ButtonWidth );
				m_spBtDown->SetCustomHeight( u32ButtonHeight );
				CFoundation::Vector2Di clButtonDownPosition( GetWidth() - u32ButtonHeight, 0 );
				m_spBtDown->SetOffset( clButtonDownPosition );
			}

			// set bar bg size and position
			if( m_spImgBg != NULL )
			{
				Unsigned32 u32BarBgWidth = GetWidth() - 2 * u32ButtonWidth;
				Unsigned32 u32BarBgHeight = GetHeight();
				m_spImgBg->SetCustomWidth( u32BarBgWidth );
				m_spImgBg->SetCustomHeight( u32BarBgHeight );

				CFoundation::Vector2Di clBarBgPosition( u32ButtonWidth, 0 );
				m_spImgBg->SetOffset( clBarBgPosition );
			}

			// set the bar rect
			if( m_spImgBar != NULL )
			{
				Unsigned32 u32BarWidth = GetWidth() - 2 * u32ButtonWidth;
				Unsigned32 u32BarHeight = GetHeight() - 2;
                m_clBarRect.SetSize( u32BarWidth, u32BarHeight );
				m_clBarRect.SetPosition( u32ButtonWidth, 1 );
				UpdateBar();
			}
		}
		break;
	}
}

void ScrollBar::UpdateBarPosition()
{
	if ( m_spImgBar == NULL )
	{
        return;
    }

    // set new position
    Unsigned32 u32Pos = GetBarRectPosition();
    u32Pos += m_u32BarOffset;
    SetBarPosition( u32Pos );
}

void ScrollBar::UpdateBar()
{
	if( m_spImgBar != NULL )
	{
		UpdateBarPosition();

		// set new size
		Unsigned32 u32BarSize = 0;
		if( m_u32Size > 0 )
		{
			u32BarSize = static_cast<Unsigned32>( m_u32VisibleSize * GetBarRectSize() * 1.0f / m_u32Size );
		}
		SetBarSize( u32BarSize );

		// set new offset
		if( m_u32BarOffset > GetMaxBarOffset() &&
			m_u32BarOffset > 0 )
		{
			if( m_u32Size >= m_u32VisibleSize )
			{
				SetBarOffset( GetMaxBarOffset() );
			}
			else
			{
				SetBarOffset( 0 );
			}
		}
	}
}

Unsigned32	ScrollBar::GetMaxBarOffset() const
{
	if ( m_u32Size > m_u32VisibleSize )
	{
		return ( m_u32Size - m_u32VisibleSize ) * GetBarRectSize() / m_u32Size + 1;
	}
	else
	{
		return 0;
	}
}

void		ScrollBar::SetBarSize( Unsigned32 u32Size )
{
	switch( m_eOrientation )
	{
	case SCROLLBAR_HORIZONTAL:
		{
			m_spImgBar->SetCustomHeight( GetHeight() - 2 );
			m_spImgBar->SetCustomWidth( std::min( GetBarRectSize(), u32Size ) );
		}
		break;
	case SCROLLBAR_VERTICAL:
		{
			m_spImgBar->SetCustomWidth( GetWidth() - 2 );
			m_spImgBar->SetCustomHeight( std::min( GetBarRectSize(), u32Size ) );
		}
		break;
	}
}

void ScrollBar::SetBarPosition( Unsigned32 _u32Position )
{
	CFoundation::Vector2Di vPosition = m_clBarRect.GetPosition();
	switch( m_eOrientation )
	{
	case SCROLLBAR_HORIZONTAL:
		{
			vPosition.SetX( _u32Position );
		}
		break;
	case SCROLLBAR_VERTICAL:
		{
			vPosition.SetY( _u32Position );
		}
		break;
	}
	m_spImgBar->SetOffset( vPosition );
}

Unsigned32	ScrollBar::GetBarRectPosition() const
{
	switch( m_eOrientation )
	{
	case SCROLLBAR_HORIZONTAL:
		return m_clBarRect.GetX();
	case SCROLLBAR_VERTICAL:
		return m_clBarRect.GetY();
	}

	return 0;
}

Unsigned32  ScrollBar::GetBarRectSize() const
{
	switch( m_eOrientation )
	{
	case SCROLLBAR_HORIZONTAL:
		return m_clBarRect.GetWidth();
	case SCROLLBAR_VERTICAL:
		return m_clBarRect.GetHeight();
	}

	return 0;
}

const CFoundation::String ScrollBarCreator::GetTypeId() const
{
	return ScrollBar::GetTypeID();
}

GuiObjectPtr ScrollBarCreator::CreateObject( const CFoundation::String &sName ) const
{
	ScrollBarPtr spScrollBar( new ScrollBar( sName ) );
    InitDefault( spScrollBar );
	return spScrollBar;
}
		
void ScrollBarCreator::InitDefault( GuiObjectPtr _spObject ) const
{
    CombinedCtrlCreator::InitDefault( _spObject );

	ScrollBarPtr spScrollBar = static_pointer_cast< ScrollBar >( _spObject );

    // Create button up
    ButtonPtr spBtUp = GuiMain::GetInstance().CreateDefault< Button >( "theButtonUp" );
    spScrollBar->AddPieceLast( spBtUp );
    spScrollBar->m_spBtUp = spBtUp;
    spScrollBar->m_spBtUp->RegisterDelegate( INPUT_EVENT_PRESSED, ObjectInputEventDelegate1Ptr( new ObjectInputEventDelegate1( spScrollBar.get(), &ScrollBar::OnPressedUp ) ) );
    spScrollBar->m_spBtUp->SetBgColor( CFoundation::Color( 0.0f, 0.0f, 1.0f ) );

    // Create button down
    ButtonPtr spBtDown = GuiMain::GetInstance().CreateDefault< Button >( "theButtonDown" );
    spScrollBar->AddPieceLast( spBtDown );
    spScrollBar->m_spBtDown = spBtDown;
    spScrollBar->m_spBtDown->RegisterDelegate( INPUT_EVENT_PRESSED, ObjectInputEventDelegate1Ptr( new ObjectInputEventDelegate1( spScrollBar.get(), &ScrollBar::OnPressedDown ) ) );
    spScrollBar->m_spBtDown->SetBgColor( CFoundation::Color( 0.0f, 0.0f, 1.0f ) );

    // Create bar background
    ImagePtr spImgBg = GuiMain::GetInstance().CreateDefault< Image >( "theBarBg" );
    spScrollBar->AddPieceLast( spImgBg );
    spScrollBar->m_spImgBg = spImgBg;
    //spImgBg->AddListener( spScrollBar, MouseEventMsg::GetType() );
    spImgBg->SetBgColor( CFoundation::Color( 1.0f, 0.0f, 0.0f ) );

    // Create bar image
    ImagePtr spImgBar = GuiMain::GetInstance().CreateDefault< Image >( "theBar" );
    spScrollBar->AddPieceLast( spImgBar );
    spScrollBar->m_spImgBar = spImgBar;
    //spImgBar->AddListener( spScrollBar, MouseEventMsg::GetType() );
    spImgBar->SetBgColor( CFoundation::Color( 0.0f, 1.0f, 0.0f ) );
    spImgBar->SetDraggable( true );
    DragDropSystem::GetInstance().SetDragController( spImgBar, DragControllerPtr( new ScrollBarDragController( spScrollBar ) ) );

	spScrollBar->SetTotalSize( 100 );
	spScrollBar->SetVisibleSize( 10 );
}

void ScrollBarCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	CombinedCtrlCreator::InitFromXML( _spObject, _spXMLElement );

	ScrollBarPtr spScrollBar = static_pointer_cast< ScrollBar >( _spObject );

    if ( _spXMLElement->HasAttribute( "scroll_step" ) )
    {
        spScrollBar->SetScrollStep( _spXMLElement->GetAttributeAsUnsigned32( "scroll_step" ) );
    }

    // Init children
    CFoundation::XMLNodeCPtr spChildNode = _spXMLElement->GetFirstChild();
    while ( spChildNode != NULL )
    {
        if ( spChildNode->IsElement() )
        {
            CFoundation::XMLElementCPtr spChildElement = static_pointer_cast< const CFoundation::XMLElement >( spChildNode );

            const CFoundation::String& strName = spChildElement->GetName();

            if ( strName == "button_up" )
            {
                spScrollBar->m_spBtUp->SetStyle( CtrlStylePtr( new CtrlStyle( spChildElement ) ) );
            }
            else if ( strName == "button_down" )
            {
                spScrollBar->m_spBtDown->SetStyle( CtrlStylePtr( new CtrlStyle( spChildElement ) ) );
            }
            else if ( strName == "bar_bg" )
            {
                spScrollBar->m_spImgBg->SetStyle( CtrlStylePtr( new CtrlStyle( spChildElement ) ) );
            }
            else if ( strName == "bar" )
            {
                spScrollBar->m_spImgBar->SetStyle( CtrlStylePtr( new CtrlStyle( spChildElement ) ) );
            }
        }
        spChildNode = spChildNode->GetNextSibling();
    }
}
