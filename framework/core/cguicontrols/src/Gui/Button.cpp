#include "cgui/Gui/Button.h"

#include "cfoundation/Debug/Makros.h"
#include "cfoundation/Debug/Profiling.h"
#include "cfoundation/Enum/EnumConverter.h"
#include "cfoundation/XML/XMLElement.h"

#include "cgui/Font/FontLoader.h"

#include "cgui/Gui/CtrlStyle.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/ScaleImage.h"
#include "cgui/Gui/SizeMode.h"
#include "cgui/Gui/TextBox.h"

#include "cgui/Messages/FocusMsg.h"

using namespace CGui;

Button::ButtonType StringToButtonType( const CFoundation::String &sType )
{
	if( sType == "normal" )
	{
		return Button::TYPE_NORMAL;
	}
	else if( sType == "toggle" )
	{
		return Button::TYPE_TOGGLE;
	}
	else
	{
		return Button::TYPE_NONE;
	}
}

Button::Button( const CFoundation::String &rName )
:	CombinedCtrl( rName ),
	m_bColored( false ),
	m_eType( TYPE_NORMAL )
{
}

Button::~Button()
{
}
		
void Button::Clear()
{
    m_spBackground.reset();
    m_spTextBox.reset();

    CombinedCtrl::Clear();
}

const CFoundation::String Button::GetTypeID()
{
	return "button";
}

const CFoundation::String Button::GetTypeId() const
{
	return Button::GetTypeID();
}

void Button::SetType( ButtonType eType )
{
	m_eType = eType;
}

Button::ButtonType Button::GetType() const
{
	return m_eType;
}

void Button::SetOutline( Outline eOutline )
{
    if( GetOutline() != eOutline )
    {
        CombinedCtrl::SetOutline( eOutline );
        SAFE_CALL( m_spTextBox )->SetOutline( eOutline );
		if( m_spBackground != NULL )
		{
			m_spBackground->SetOutline( eOutline );
		}
    }
}

bool Button::HasFocus() const
{
	bool bFocus = GuiObject::HasFocus();

	if( m_spBackground != NULL )
	{
		bFocus = m_spBackground->HasFocus() || bFocus;
	}

    bFocus = m_spTextBox != NULL && m_spTextBox->HasFocus() || bFocus;

	return bFocus;
}

void Button::SetText( const CFoundation::String& _strText )
{
    SAFE_CALL( m_spTextBox )->SetText( _strText );
}
		
void Button::SetText( State _eState, const CFoundation::String& _strText )
{
    SAFE_CALL( m_spTextBox )->SetText( _eState, _strText );
}

const CFoundation::String& Button::GetText() const
{
    if ( m_spTextBox != NULL )
    {
        return m_spTextBox->GetText();
    }
    else
    {
        static CFoundation::String strTmp;
        return strTmp;
    }
}

void Button::SetFont( FontCPtr _spFont )
{
    SAFE_CALL( m_spTextBox )->SetFont( _spFont );
}
        
FontCPtr Button::GetFont() const
{
    if ( m_spTextBox != NULL )
    {
        return m_spTextBox->GetFont();
    }
    else
    {
        return FontCPtr();
    }
}

void Button::SetTextColor( State _eState, const CFoundation::Color& _clColor )
{
    SAFE_CALL( m_spTextBox )->SetTextColor( _eState, _clColor );
}

void Button::SetTextAlignment( AlignmentType eAlignType, Alignment eAlign )
{
    SAFE_CALL( m_spTextBox )->SetTextAlignment( eAlignType, eAlign );
}

bool Button::IsDown() const
{
	return IsActive();
}

void Button::SetBackground( GuiObjectPtr _pBackground )
{
	GuiObjectPtr pOldBackground = m_spBackground;
	if( pOldBackground != NULL )
	{
		RemovePiece( pOldBackground );
	}
	if( _pBackground != NULL )
	{
		AddPieceFirst( _pBackground );
		_pBackground->SetSizeMode( SIZE_MODE_STRETCH );
	}
	m_spBackground = _pBackground;
}

GuiObjectPtr Button::GetBackground()
{
	return m_spBackground;
}

GuiObjectCPtr Button::GetBackground() const
{
	return m_spBackground;
}

TextBoxPtr Button::GetTextBox()
{
	return m_spTextBox;
}

TextBoxCPtr Button::GetTextBox() const
{
	return m_spTextBox;
}

const CFoundation::RectF32& Button::GetTextBB() const
{
    if ( m_spTextBox != NULL )
    {
	    return m_spTextBox->GetTextBB();
    }
    else
    {
        static CFoundation::RectF32 rectTmp;
        return rectTmp;
    }
}

bool Button::IsColored() const
{
	return m_bColored;
}

void Button::SetColored( bool bColored )
{
	m_bColored = bColored;
}
        
void Button::OnPointerLeft()
{
    if ( m_eType == TYPE_NORMAL &&
         IsActive() )
    {
        SetActive( false );
    }

    CombinedCtrl::OnPointerLeft();
}

void Button::OnPressed()
{
    if ( m_eType == TYPE_NORMAL )
    {
        SetActive( true );
    }

    CombinedCtrl::OnPressed();
}

void Button::OnReleased()
{
    if ( m_eType == TYPE_NORMAL )
    {
        SetActive( false );
    }

    CombinedCtrl::OnReleased();
}

void Button::OnClicked()
{
    CombinedCtrl::OnClicked();

    if ( m_eType == TYPE_TOGGLE )
    {
        SetActive( !IsActive() );
    }
    else if ( !IsDisabled() )
    {
        DispatchEvent( INPUT_EVENT_PUSHED, GetSharedPtr< Button >() );
    }
}

const CFoundation::String ButtonCreator::GetTypeId() const
{
	return Button::GetTypeID();
}

GuiObjectPtr ButtonCreator::CreateObject( const CFoundation::String &sName ) const
{
	ButtonPtr spButton( new Button( sName ) );

    InitDefault( spButton );

	return spButton;
}

void ButtonCreator::InitDefault( GuiObjectPtr _spObject ) const
{
	CombinedCtrlCreator::InitDefault( _spObject );   

	ButtonPtr spButton = static_pointer_cast< Button >( _spObject );

    /*
    // Create button text box
    TextBoxPtr spTextBox = GuiMain::GetInstance().Create< TextBox >( "theTextBox" );
	spButton->AddPieceLast( spTextBox );
	spButton->m_spTextBox = spTextBox;

    // Init text box
    spTextBox->SetBgVisible( false );
    spTextBox->SetSizeMode( SIZE_MODE_STRETCH );*/
}

void ButtonCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
    PROFILING( "ButtonCreator::InitFromXML" );

	CombinedCtrlCreator::InitFromXML( _spObject, _spXMLElement );

	ButtonPtr spButton = static_pointer_cast< Button >( _spObject );

    // Get controls
    spButton->m_spTextBox = static_pointer_cast< TextBox >( spButton->FindChild( "theTextBox" ) );

	CFoundation::String sTmp = "";

	// Set colored
	if ( _spXMLElement->HasAttribute( "colored" ) )
	{
        CFoundation::String strColored = _spXMLElement->GetAttribute( "colored" );
        spButton->SetColored( strColored == "true" );
	}

    // Set text
	if ( _spXMLElement->HasAttribute( "text" ) )
	{
        spButton->SetText( _spXMLElement->GetAttribute( "text" ) );
	}

	// Set text alignment
    CFoundation::EnumConverter< Alignment >& clAlignmentConverter = CFoundation::EnumConverter< Alignment >::GetInstance();
	if ( _spXMLElement->HasAttribute( "text_align_horizontal" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "text_align_horizontal" );
        spButton->SetTextAlignment( ALIGN_HORIZONTAL, clAlignmentConverter.Convert( sTmp ) );
	}

	if ( _spXMLElement->HasAttribute( "text_align_vertical" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "text_align_vertical" );
        spButton->SetTextAlignment( ALIGN_VERTICAL, clAlignmentConverter.Convert( sTmp ) );
	}

    // set font
    FontCPtr spFont = spButton->GetFont();
    CFoundation::String sFontName = spFont != NULL ? spFont->GetName() : "common/fonts/arial.ttf";
    Unsigned8 u8FontSize = spFont != NULL ? spFont->GetSize() : 12;

    bool bSetFont = false;
	if ( _spXMLElement->HasAttribute( "font_name" ) )
	{
        sFontName = _spXMLElement->GetAttribute( "font_name" );
        bSetFont = true;
	}
	if ( _spXMLElement->HasAttribute( "font_size" ) )
	{
        u8FontSize = _spXMLElement->GetAttribute( "font_size" ).ToUnsigned8();
        bSetFont = true;
	}
    if ( bSetFont )
    {
	    spFont = FontLoader::GetInstance().LoadFontFromFile( sFontName, u8FontSize );
	    spButton->SetFont( spFont );
    }

	// set button type
	if ( _spXMLElement->HasAttribute( "button_type" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "button_type" );
        spButton->SetType( StringToButtonType( sTmp ) );
	}

    PROFILING( "ButtonCreator::InitFromXML::InitChildren" );

    // Read state and child infos    
    const CFoundation::EnumConverter< State >& clStateConverter = CFoundation::EnumConverter< State >::GetInstance();
	CFoundation::XMLNodePtr spChild = _spXMLElement->GetFirstChild();
	while ( spChild != NULL )
	{
		if ( spChild->IsElement() )
		{
            CFoundation::XMLElementPtr spElement = static_pointer_cast< CFoundation::XMLElement >( spChild );

			const CFoundation::String& sName = spElement->GetName();
            const CFoundation::String& strName = spElement->GetAttribute( "name" );
                        
            // Get state
            State eState = clStateConverter.Convert( sName );
            if ( eState != STATE_NONE )
            {
            }
            else
            {
			    // Setup background
                if ( strName == "Background" )
                {
                    GuiObjectPtr spBackground;
			        if ( sName == "image" )
			        { 
                        spBackground = spButton->GetBackground();
                        if ( spBackground == NULL ||
                             spBackground->GetTypeId() != "image" )
                        {
				            spBackground = GuiMain::GetInstance().Create< Image >( sName );
                        }
			        }
			        else if ( sName == "scaleimage" )
			        {
                        spBackground = spButton->GetBackground();
                        if ( spBackground == NULL ||
                             spBackground->GetTypeId() != "scaleimage" )
                        {
				            spBackground = GuiMain::GetInstance().Create< ScaleImage >( sName );
                        }
			        }

			        if ( spBackground != NULL )
			        {
                        spBackground->SetStyle( CtrlStylePtr( new CtrlStyle( spElement ) ) );
				        spButton->SetBackground( spBackground );
			        }
                }

                /*
			    // Setup textbox
			    if ( sName == "textbox" )
			    {
				    TextBoxPtr spTextBox = spButton->GetTextBox();
                    spTextBox->SetStyle( CtrlStylePtr( new CtrlStyle( spElement ) ) );
			    }*/
            }
		}
			
        spChild = spChild->GetNextSibling();
	}
}
		
void ButtonCreator::InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const
{
	CombinedCtrlCreator::InitFromTemplate( _spObject, _spTemplate );

	ButtonPtr spObject = static_pointer_cast< Button >( _spObject );
	ButtonCPtr spTemplate = static_pointer_cast< const Button >( _spTemplate );

    // Init button
    spObject->SetType( spTemplate->GetType() );
    spObject->SetColored( spTemplate->IsColored() );

    // Init textbox
    GuiMain::GetInstance().InitFromTemplate( spObject->m_spTextBox, spTemplate->m_spTextBox );
    if ( spTemplate->m_spBackground != NULL )
    {
        spObject->SetBackground( GuiMain::GetInstance().CreateFromTemplate( "ImgBackground", spTemplate->m_spBackground ) );
    }
    else
    {
        spObject->SetBackground( CGui::GuiObjectPtr() );
    }
}

    /*
Button& ButtonCreator::AssignStyle( GuiObject& clObject, const CGui::CtrlStyle& clStyle ) const
{
	CombinedCtrlCreator::AssignStyle( clObject, clStyle );
	Button &clButton = static_cast< Button& >( clObject );
    return clButton;

	const CFoundation::XMLElement* pRoot = clStyle.GetRoot();
    if ( pRoot == NULL )
    {
        return clButton;
    }

    const CFoundation::XMLElement& clXMLElement = *pRoot;
    return InitFromXML( clObject, clXMLElement );


	CFoundation::String sTmp = "";

	// Set text alignment
    CFoundation::EnumConverter< Alignment, ALIGNMENT_END >& clAlignmentConverter = CFoundation::EnumConverter< Alignment, ALIGNMENT_END >::GetInstance();
	if ( _spXMLElement->HasAttribute( "text_align_horizontal" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "text_align_horizontal" );
        spButton->SetTextAlignment( ALIGN_HORIZONTAL, clAlignmentConverter.ConvertFromString( sTmp ) );
	}

	if ( _spXMLElement->HasAttribute( "text_align_vertical" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "text_align_vertical" );
        spButton->SetTextAlignment( ALIGN_VERTICAL, clAlignmentConverter.ConvertFromString( sTmp ) );
	}

	// Set colored
	if ( pRoot->HasAttribute( "colored" ) )
	{
        CFoundation::String sColored = pRoot->GetAttribute( "colored" );
        spButton->SetColored( sColored == "true" );
	}

	// Set button type
	if ( pRoot->HasAttribute( "button_type" ) )
	{
        CFoundation::String sType = pRoot->GetAttribute( "button_type" );
        spButton->SetType( StringToButtonType( sType ) );
	}

	CFoundation::XMLNode* pChild = pRoot->GetFirstChild();
	while ( pChild != NULL )
	{
		if ( pChild->IsElement() )
		{
            CFoundation::XMLElement* pElement = static_cast< CFoundation::XMLElement* >( pChild );

			const CFoundation::String& sName = pElement->GetName();

			// setup background
			GuiObject* pBackground = NULL;
			if ( sName == "img" )
			{
				pBackground = &clGuiMain.Create< Image >( sName );
			}
			else if ( sName == "scaleimg" )
			{
				pBackground = &clGuiMain.Create< ScaleImage >( sName );
			}

			if ( pBackground != NULL )
			{
                pBackground->SetStyle( CtrlStyle( *pElement ) );
				spButton->SetBackground( pBackground );
			}

			// setup textbox
			if ( sName == "textbox" )
			{
				TextBox& clTextBox = spButton->GetTextBox();
                clTextBox.SetStyle( CtrlStyle( *pElement ) );
			}
		}
			
        pChild = pChild->GetNextSibling();
	}

	return clButton;
}*/

bool ButtonCreator::SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const
{
	CombinedCtrlCreator::SaveToXML( _spObject, _spDoc, _spXMLElement );

	ButtonCPtr spButton = static_pointer_cast< const Button >( _spObject );

	// set text settings
    const CFoundation::String& strText = spButton->GetText();
    if ( strText.GetLength() > 0 )
    {
	    _spXMLElement->SetAttribute( "text", strText );
    }
	//_spXMLElement->SetAttribute( "font", clButton.GetFontName() );
	//_spXMLElement->SetAttribute( "font_size", CFoundation::String( clButton.GetFontSize() ) );

	// set button type
	CFoundation::String strType = "";
	switch ( spButton->GetType() )
	{
	case Button::TYPE_TOGGLE:
		strType = "toggle";
		break;
    default:
        break;
	}

    if ( strType != "" )
    {
	    _spXMLElement->SetAttribute( "button_type", strType );
    }

	return true;
}
