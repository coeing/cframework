#include "cgui/Gui/EditBox.h"

#include "cfoundation/XML/XMLElement.h"

#include "csystem/Input/InputState.h"

#include "cgui/Gui/CtrlStyle.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/TextBox.h"

#include "cgui/Input/Input.h"

#include "cgui/Messages/KeyboardMsg.h"

using namespace CGui;

EditBox::EditBox( const CFoundation::String &sName )
:	Button( sName ),
	m_f32CaretBlinkSpeed( 0.5f ),
	m_f32PassedTime( 0.0f ),
    m_u32CaretPosition( 0 )
{
    SetType( Button::TYPE_NONE );
}

EditBox::~EditBox()
{
}

const CFoundation::String EditBox::GetTypeID()
{
	return "editbox";
}

const CFoundation::String EditBox::GetTypeId() const
{
	return EditBox::GetTypeID();
}

bool EditBox::ProcessInputFocused( const CSystem::InputState& _State )
{
    // Check if typed characters
    const std::vector< wchar_t >& awcTypedCharacters = _State.m_awcTypedCharacters;
    Unsigned32 u32NumTypedCharacters = awcTypedCharacters.size();
    for ( Unsigned32 u32Idx = 0; u32Idx != u32NumTypedCharacters; ++u32Idx )
    {
	    wchar_t wcChar = awcTypedCharacters[ u32Idx ];
	    switch ( wcChar )
	    {
        case CSystem::KEY_BACKSPACE:
		    {
			    if ( GetText().GetLength() > 0 &&
                     m_u32CaretPosition > 0 )
			    {
				    const CFoundation::String& strOldValue = GetText();
                    CFoundation::String strNewValue = strOldValue.Head( m_u32CaretPosition - 1 ) + strOldValue.Tail( m_u32CaretPosition );
                    --m_u32CaretPosition;
				    SetText( strNewValue, false );
			    }
		    }
		    break;
	    //case CSystem::KEY_ENTER:
	    case CSystem::KEY_RETURN:
            {
                Input::GetInstance().SetFocusObject( GuiObjectPtr() );
            }
            break;
        case CSystem::KEY_ESC:
            {
                // Set back text
                SetText( m_strInitialText, false );

                // Loose focus
                Input::GetInstance().SetFocusObject( GuiObjectPtr() );
            }
            break;
	    case CSystem::KEY_SHIFT:
	    case CSystem::KEY_TAB:
		    break;
	    default:
		    {
			    const CFoundation::String& strOldValue = GetText();
                CFoundation::String strNewValue = strOldValue.Head( m_u32CaretPosition ) + wcChar + strOldValue.Tail( m_u32CaretPosition );
                ++m_u32CaretPosition;
			    SetText( strNewValue, false );
		    }
		    break;
	    }
    }

    if ( _State.m_eKeyStates[ CSystem::KEY_DEL ] == CSystem::KEY_STATE_FIRST_DOWN )
    {
        Unsigned32 u32TextLength = GetText().GetLength();
	    if ( u32TextLength > 0 &&
             m_u32CaretPosition <= u32TextLength )
	    {
		    const CFoundation::String& strOldValue = GetText();
            CFoundation::String strNewValue = strOldValue.Head( m_u32CaretPosition ) + strOldValue.Tail( m_u32CaretPosition + 1 );
		    SetText( strNewValue, false );
	    }
    }
    
    // Check for caret position change
    bool bChangeCaretPosition = false;
    Unsigned32 u32NewCaretPosition = m_u32CaretPosition;
    if ( _State.m_eKeyStates[ CSystem::KEY_LEFT ] == CSystem::KEY_STATE_FIRST_DOWN )
    {
        if ( m_u32CaretPosition > 0 )
        {
            bChangeCaretPosition = true;        
            u32NewCaretPosition = m_u32CaretPosition - 1;
        }
    }
    else if ( _State.m_eKeyStates[ CSystem::KEY_RIGHT ] == CSystem::KEY_STATE_FIRST_DOWN )
    {
        if ( m_u32CaretPosition < GetText().GetLength() )
        {
            bChangeCaretPosition = true;        
            u32NewCaretPosition = m_u32CaretPosition + 1;
        }
    }
    else if ( _State.m_eKeyStates[ CSystem::KEY_END ] == CSystem::KEY_STATE_FIRST_DOWN )
    {
        bChangeCaretPosition = true;     
        u32NewCaretPosition = GetText().GetLength();
    }
    else if ( _State.m_eKeyStates[ CSystem::KEY_HOME ] == CSystem::KEY_STATE_FIRST_DOWN )
    {
        bChangeCaretPosition = true;     
        u32NewCaretPosition = 0;
    }

    if ( bChangeCaretPosition )
    {
        SetCaretPosition( u32NewCaretPosition );
    }

	return Button::ProcessInputFocused( _State );
}

bool EditBox::ProcessInputPointed( const CSystem::InputState& _State )
{
    // Set caret position
    if ( _State.m_eMouseButtonStates[ CSystem::MOUSE_LEFT ] == CSystem::MBS_FIRST_DOWN )
    {
        // Get relative mouse position
        TextBoxPtr spTextBox = GetTextBox();
        CFoundation::Vector2Di vRelativeMousePosition = _State.m_vMousePosition;
        if ( spTextBox != NULL )
        {
            vRelativeMousePosition -= spTextBox->GetAbsolutePosition();
        }
        
        // Get current range
        CFoundation::Vector2Df vCutOffset;
        if ( spTextBox != NULL )
        {
            vCutOffset = spTextBox->GetCutOffset();
        }
        Float32 f32MinOffset = vCutOffset.GetX();

        // Get offset
        Float32 f32Offset = vRelativeMousePosition.GetX() + f32MinOffset;

        // Get text position
        FontCPtr spFont = GetFont();
        Float32 f32CharPosition = 0.0f;
        if ( spFont != NULL )
        {
            f32CharPosition = spFont->GetPosition( GetText(), f32Offset );
        }
        SetCaretPosition( static_cast< Unsigned32 >( f32CharPosition + 0.5f ) );
    }

	return Button::ProcessInputPointed( _State );
}

void EditBox::OnGainedFocus()
{
    SetActive( true );
    SAFE_CALL( m_spImgCaret )->SetVisible( true );

    // Update caret height and position
    UpdateCaretPosition();

	Button::OnGainedFocus();
}

void EditBox::OnLostFocus()
{
    SetActive( false );
    SAFE_CALL( m_spImgCaret )->SetVisible( false );
    
    // Set text alignment
    SAFE_CALL( GetTextBox() )->SetCutOffset( CFoundation::Vector2Df( 0, 0 ) );

    // Store initial text
    m_strInitialText = GetText();

	Button::OnLostFocus();
}

void EditBox::Update( Float32 _f32TimeDifference )
{
	Button::Update( _f32TimeDifference );

	if ( HasFocus() &&
		 m_f32CaretBlinkSpeed > 0.0f )
	{
		m_f32PassedTime += _f32TimeDifference;
		while ( m_f32PassedTime > m_f32CaretBlinkSpeed )
		{
            if ( m_spImgCaret != NULL )
            {
                m_spImgCaret->SetVisible( !m_spImgCaret->IsVisible() );
            }
			m_f32PassedTime -= m_f32CaretBlinkSpeed;
		}
	}
}

void EditBox::SetStateFlag( StateFlag _eStateFlag, bool _bSet )
{
    Button::SetStateFlag( _eStateFlag, _bSet );

    if ( _eStateFlag == STATE_FLAG_ACTIVE )
    {
    }
}

void EditBox::SetText( const CFoundation::String& _strText )
{
    SetText( _strText, true );
}

void EditBox::SetFont( FontCPtr _spFont )
{
    Button::SetFont( _spFont );
    //UpdateCaretHeight();
}

void EditBox::SetCaretBlinkSpeed( Float32 _f32CaretBlinkSpeedMs )
{
    m_f32CaretBlinkSpeed = _f32CaretBlinkSpeedMs;
}

Float32 EditBox::GetCaretBlinkSpeed() const
{
    return m_f32CaretBlinkSpeed;
}
        
void EditBox::SetCaretPosition( Unsigned32 _u32CaretPosition )
{
    if ( _u32CaretPosition == m_u32CaretPosition )
    {
        return;
    }

    m_u32CaretPosition = _u32CaretPosition;

    // Update caret position
    UpdateCaretPosition();
}

Unsigned32 EditBox::GetCaretPosition() const
{
    return m_u32CaretPosition;
}

void EditBox::UpdateCaretHeight()
{
    const CFoundation::RectF32& clTextBB = GetTextBB();
    // TODO CO: Only do this if the caret should be scaled (introduce a new flag)
    //SAFE_CALL( m_spImgCaret )->SetCustomHeight( static_cast<Unsigned32>( clTextBB.GetHeight() ) );
}

void EditBox::UpdateCaretPosition()
{
    // Compute text and caret position
    CFoundation::Vector2Di vCaretOffset;
    const CFoundation::Vector2Df& vCutOffset = ComputeTextPosition( vCaretOffset );

    // Set text offset
    TextBoxPtr spTextBox = GetTextBox();
    SAFE_CALL( spTextBox )->SetCutOffset( vCutOffset );

    //CFoundation::Vector2Di clNewOffset = m_spImgCaret->GetOffset();
    //clNewOffset.SetX( static_cast< Integer32 >( clTextBB.GetX() + clTextBB.GetWidth() + 1 ) );
    SAFE_CALL( m_spImgCaret )->SetOffset( vCaretOffset );
}
        
void EditBox::SetSize( const CFoundation::Vector2Du& _vSize )
{
    if ( _vSize == GetSize() )
    {
        return;
    }

    Button::SetSize( _vSize );
    UpdateCaretHeight();
    UpdateCaretPosition();
}
		
void EditBox::SetText( const CFoundation::String& _strText, bool _bResetCaret )
{
    if ( _strText == GetText() )
    {
        return;
    }

    CFoundation::String strOldText = GetText();

	Button::SetText( _strText );

    // Set caret position
    if ( _bResetCaret )
    {
        TextBoxPtr spTextBox = GetTextBox();
        SAFE_CALL( spTextBox )->SetCutOffset( CFoundation::Vector2Df( 0, 0 ) );
        m_u32CaretPosition = _strText.GetLength();
    }

    if ( !IsLayoutSuspended() )
    {
        UpdateCaretHeight();
	    UpdateCaretPosition();
        
        // Dispatch event
        DispatchEvent( EVENT_EDITBOX_TEXT_CHANGED, GetSharedPtr< EditBox >(), _strText, strOldText );
    }
}
        
ImagePtr EditBox::GetCaret() const
{
    return m_spImgCaret;
}
        
CFoundation::Vector2Di EditBox::ComputeTextPosition( CFoundation::Vector2Di& _vCaretPosition ) const
{
    // Get text till caret position
    const CFoundation::String& strText = GetText().Head( m_u32CaretPosition );

    // Get text size
    CFoundation::Vector2Df vTextSize;
    if ( GetFont() != NULL )
    {
        vTextSize = GetFont()->GetSize( strText );
    }

    // Get caret offset
    Float32 f32CaretOffset = vTextSize.GetX();
    Float32 f32CaretHeight = 0.0f;
    if ( m_spImgCaret != NULL )
    {
        f32CaretHeight = m_spImgCaret->GetHeight();
    }

    // Get current visible range
    TextBoxCPtr spTextBox = GetTextBox();
    CFoundation::RectF32 rectTextBB;
    CFoundation::Vector2Df vCutOffset;
    CFoundation::Vector2Di vTextPosition;
    Float32 f32MinOffset = 0;
    Float32 f32MaxOffset = 0;
    if ( spTextBox != NULL )
    {
        vCutOffset = spTextBox->GetCutOffset();
        vTextPosition = spTextBox->GetRelativePosition();
        rectTextBB = spTextBox->GetTextBB();
        vTextPosition.AddX( rectTextBB.GetLeft() );
        f32MinOffset = vCutOffset.GetX();
        f32MaxOffset = f32MinOffset + spTextBox->GetInnerWidth();
    }

    // Set cut offset
    if ( f32CaretOffset < f32MinOffset )
    {
        vCutOffset.SubX( f32MinOffset - f32CaretOffset );
    }
    else if ( f32CaretOffset > f32MaxOffset )
    {
        vCutOffset.AddX( f32CaretOffset - f32MaxOffset );
    }        

    _vCaretPosition.Set( static_cast< Integer32 >( f32CaretOffset - vCutOffset.GetX() + vTextPosition.GetX() ), static_cast< Integer32 >( ( rectTextBB.GetSize().GetY() - f32CaretHeight ) / 2.0f + vTextPosition.GetY() ) );

    return vCutOffset;
}

const CFoundation::String EditBoxCreator::GetTypeId() const
{
	return EditBox::GetTypeID();
}

GuiObjectPtr EditBoxCreator::CreateObject( const CFoundation::String &sName ) const
{
	EditBoxPtr spEditBox( new EditBox( sName ) );

    InitDefault( spEditBox );

	return spEditBox;
}

void EditBoxCreator::InitDefault( GuiObjectPtr _spObject ) const
{
	ButtonCreator::InitDefault( _spObject );

	EditBoxPtr spEditBox = static_pointer_cast< EditBox >( _spObject );

    /*
    // Init text box
    TextBoxPtr spTextBox = spEditBox->GetTextBox();
    spTextBox->SetPaddingRight( 5 );
    spTextBox->AddListener( spEditBox, KeyboardMsg::GetType() );*/

    /*
    // Create caret image
	ImagePtr spImgCaret = GuiMain::GetInstance().Create< Image >( "theCaret" );
	spEditBox->AddPieceLast( spImgCaret );
	spEditBox->m_spImgCaret = spImgCaret;

    // Init caret image
    spImgCaret->SetAttachObject( ATTACH_OBJECT_PARENT );
    spImgCaret->SetLocalAttachPoint( ATTACH_POINT_LEFT );
    spImgCaret->SetReferenceAttachPoint( ATTACH_POINT_LEFT );
	spImgCaret->SetCustomWidth( 1 );
    spImgCaret->SetBgVisible( true );
    spImgCaret->SetBgColor( CFoundation::Color( 0.0f, 0.0f, 0.0f, 1.0f ) );
    spImgCaret->SetVisible( false );*/
}

void EditBoxCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	ButtonCreator::InitFromXML( _spObject, _spXMLElement );

	EditBoxPtr spEditBox = static_pointer_cast< EditBox >( _spObject );

    // Get controls
    spEditBox->m_spImgCaret = static_pointer_cast< Image >( spEditBox->FindChild( "theCaret" ) );
    SAFE_CALL( spEditBox->m_spImgCaret )->SetVisible( false );

    // Get caret blink speed
    if ( _spXMLElement->HasAttribute( "caret_blink_speed" ) )
    {
        spEditBox->SetCaretBlinkSpeed( _spXMLElement->GetAttribute( "caret_blink_speed" ).ToUnsigned32() );
    }

	CFoundation::XMLNodePtr spChild = _spXMLElement->GetFirstChild();
	while ( spChild != NULL )
	{
		if ( spChild->IsElement() )
		{
            CFoundation::XMLElementPtr spElement = static_pointer_cast< CFoundation::XMLElement >( spChild );

			const CFoundation::String& sName = spElement->GetName();

			// Setup caret
            if ( sName == "caret" )
			{
                ImagePtr spCaret = spEditBox->GetCaret();
                SAFE_CALL( spCaret )->SetStyle( CtrlStylePtr( new CtrlStyle( spElement ) ) );
			}
		}
			
        spChild = spChild->GetNextSibling();
	}
}
		
void EditBoxCreator::InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const
{
	ButtonCreator::InitFromTemplate( _spObject, _spTemplate );

	EditBoxPtr spObject = static_pointer_cast< EditBox >( _spObject );
	EditBoxCPtr spTemplate = static_pointer_cast< const EditBox >( _spTemplate );

    // Init editbox
    spObject->SetCaretBlinkSpeed( spTemplate->GetCaretBlinkSpeed() );

    // Init caret
    GuiMain::GetInstance().InitFromTemplate( spObject->m_spImgCaret, spTemplate->m_spImgCaret );
}

/*
EditBox& EditBoxCreator::AssignStyle( GuiObject& _clObject, const CGui::CtrlStyle& _clStyle ) const
{
	EditBox& clEditBox = static_cast< EditBox& >( _clObject );

	CFoundation::XMLElementConstPtr pRoot = _clStyle.GetRoot();
    if ( pRoot == NULL )
    {
        return clEditBox;
    }

	return InitFromXML( _clObject, *pRoot );
}*/

//CFoundation::XMLElement& EditBoxCreator::SaveToXML( const GuiObject &clObject, CFoundation::XMLDocument &clDoc, CFoundation::XMLElement &clXMLElement ) const
//{
//	ButtonCreator::GetInstance().SaveToXML( clObject, clDoc, clXMLElement );
//
//	//const EditBox &clEditBox = static_cast<const EditBox&>( clObject );
//
//	return clXMLElement;
//}
