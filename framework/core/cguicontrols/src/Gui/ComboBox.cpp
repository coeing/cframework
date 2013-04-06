#include "cgui/Gui/ComboBox.h"

#include "cfoundation/XML/XMLElement.h"

#include "cgui/Font/FontLoader.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/CtrlStyle.h"
#include "cgui/Gui/EditBox.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Layer.h"
#include "cgui/Gui/ListBox.h"
#include "cgui/Gui/ListBoxColumn.h"
#include "cgui/Gui/SizeMode.h"

#include "cgui/Messages/FocusMsg.h"

using namespace CGui;

ComboBox::ComboBox( const CFoundation::String &rName )
:	CombinedCtrl( rName ),
	m_u32BarHeight( 0 )
{
    // Create delegates
    m_spDelegateSelectedCell = ListBoxEventDelegate2Ptr( new ListBoxEventDelegate2( this, &ComboBox::OnSelectedCell ) );
    m_spDelegateButtonActivated = StateEventDelegate0Ptr( new StateEventDelegate0( this, &ComboBox::OnButtonActivated ) );
    m_spDelegateButtonDeactivated = StateEventDelegate0Ptr( new StateEventDelegate0( this, &ComboBox::OnButtonDeactivated ) );
}

ComboBox::~ComboBox()
{
}

const CFoundation::String ComboBox::GetTypeID()
{
	return CFoundation::String( "combobox" );
}

const CFoundation::String ComboBox::GetTypeId() const
{
	return ComboBox::GetTypeID();
}

//void ComboBox::ProcessMessage( const CFoundation::Message &clMessage )
//{
//    const FocusMsg* pFocusMsg = clMessage.CastTo<FocusMsg>();
//    if ( pFocusMsg != NULL )
//    {
//        if( pFocusMsg->GetObject() == m_spEditBox &&
//            pFocusMsg->GetFocusType() == FocusMsg::TYPE_GAINED )
//        {
//            // Hide drop down list by switching the button to normal state
//            m_spListButton->SetActive( false );
//        }
//    }
//}

void ComboBox::UpdateButtonSize()
{
    Unsigned32 u32ButtonDimension = std::min( GetWidth(), GetBarHeight() );
    SAFE_CALL( m_spListButton )->SetCustomHeight( u32ButtonDimension );
}

void ComboBox::UpdateEditBoxSize()
{
    Unsigned32 u32ButtonWidth = 0;
    if ( m_spListButton != NULL )
    {
        u32ButtonWidth = m_spListButton->GetWidth();
    }
    SAFE_CALL( m_spEditBox )->SetCustomSize( CFoundation::Vector2Du( GetWidth() - u32ButtonWidth, GetBarHeight() ) );
}

void ComboBox::UpdateListBoxSize()
{
    if ( m_spDropDownList == NULL )
    {
        return;
    }

    Unsigned32 u32DropDownListHeight = GetHeight() > GetBarHeight() ? GetHeight() - GetBarHeight() : 0;
    m_spDropDownList->SetCustomSize( CFoundation::Vector2Du( GetWidth(), u32DropDownListHeight ) );
}

void ComboBox::UpdateListBoxPosition()
{
    if ( m_spDropDownList == NULL )
    {
        return;
    }

    m_spDropDownList->SetOffset( CFoundation::Vector2Di( GetAbsolutePosition().GetX(), GetAbsolutePosition().GetY() + GetBarHeight() ) );
}

void ComboBox::ShowDropDownList( bool bShow )
{
    if ( m_spDropDownList == NULL )
    {
        return;
    }

	//m_spDropDownList->SetVisible( bShow );

    GuiMain& clGuiMain = GuiMain::GetInstance();
    if ( bShow )
    {
        UpdateListBoxPosition();

        // Add to foreground layer to make sure it is always visible
        clGuiMain.GetLayer( LAYER_FG ).AddObjectLast( m_spDropDownList );
    }
    else
    {
        clGuiMain.GetLayer( LAYER_FG ).RemoveObject( m_spDropDownList );
    }
}

void ComboBox::SetBarHeight( Unsigned32 u32BarHeight )
{
	if( u32BarHeight != GetBarHeight() )
	{
		m_u32BarHeight = u32BarHeight;
		UpdateListBoxPosition();
	}
}

Unsigned32 ComboBox::GetBarHeight() const
{
	return m_u32BarHeight;
}

void ComboBox::SetDropDownListHeight( Unsigned32 /*u32ListHeight*/ )
{
/*	if( u32ListHeight != GetDropDownListHeight() )
	{
		SetHeight( GetBarHeight() + u32ListHeight );
	}*/
}

Unsigned32 ComboBox::GetDropDownListHeight() const
{
	return GetHeight() - GetBarHeight();
}
		
void ComboBox::SetParent( PanelPtr _spParent )
{
    CombinedCtrl::SetParent( _spParent );
    UpdateListBoxPosition();
}
        
bool ComboBox::SetPosition( const CFoundation::Vector2Di& _vPosition )
{
    bool bPositionChanged = CombinedCtrl::SetPosition( _vPosition );
    if ( bPositionChanged )
    {
        UpdateListBoxPosition();
    }
    return bPositionChanged;
}

void ComboBox::SetSize( const CFoundation::Vector2Du& _vSize )
{
    if ( _vSize.GetY() < GetBarHeight() )
	{
		//_vSize.SetY( GetBarHeight() );
	}

	bool bWidthChanged = _vSize.GetX() != GetWidth();
	bool bHeightChanged = _vSize.GetY() != GetHeight();

    CombinedCtrl::SetSize( _vSize );

	if ( bWidthChanged || bHeightChanged )
	{
		UpdateButtonSize();
		UpdateEditBoxSize();
		UpdateListBoxSize();
		if( bHeightChanged )
		{
			UpdateListBoxPosition();
		}
	}
}

void ComboBox::OnAttachObjectPositionChanged()
{
    CombinedCtrl::OnAttachObjectPositionChanged();
    UpdateListBoxPosition();
}

void ComboBox::SetText( const CFoundation::String &sText )
{
    SAFE_CALL( m_spEditBox )->SetText( sText );
}

const CFoundation::String& ComboBox::GetText() const
{
    if ( m_spEditBox != NULL )
    {
        return m_spEditBox->GetText();
    }
    else
    {
        static CFoundation::String strTmp;
        return strTmp;
    }
}

void ComboBox::SetFont( FontCPtr _spFont )
{
	SAFE_CALL( m_spEditBox )->SetFont( _spFont );
}
        
FontCPtr ComboBox::GetFont() const
{
    return m_spEditBox != NULL ? m_spEditBox->GetFont() : FontCPtr();
}

void ComboBox::SetTextColor( State _eState, const CFoundation::Color& _clColor )
{
	SAFE_CALL( m_spEditBox )->SetTextColor( _eState, _clColor );
}

void ComboBox::SetTextAlignment( AlignmentType eAlignType, Alignment eAlign )
{
	SAFE_CALL( m_spEditBox )->SetTextAlignment( eAlignType, eAlign );
}

void ComboBox::AddItem( const CFoundation::String& sText, Integer64 i64Data )
{
    if ( m_spDropDownList == NULL )
    {
        return;
    }

    m_spDropDownList->AddColumnData( sText, i64Data );
    m_spDropDownList->AddRow();
}
       
Unsigned32 ComboBox::GetSelectedItemIdx() const
{
    if ( m_spDropDownList != NULL )
    {
        return m_spDropDownList->GetSelectedRow();
    }
    else
    {
        return MAX_U32;
    }
}
        
Integer64 ComboBox::GetSelectedItemData() const
{
    if ( m_spDropDownList != NULL )
    {
        return m_spDropDownList->GetCellData( m_spDropDownList->GetSelectedRow(), 0 );
    }
    else
    {
        return 0;
    }
}

void ComboBox::OnSelectedCell( Unsigned32 _u32SelectedRow, Unsigned32 _u32SelectedCol )
{
    if ( m_spDropDownList == NULL )
    {
        return;
    }

    const CFoundation::String& strValue = m_spDropDownList->GetCellValue( _u32SelectedRow, _u32SelectedCol );
    Unsigned64 i64Data = m_spDropDownList->GetCellData( _u32SelectedRow, _u32SelectedCol );
    SetText( strValue );

    // Hide drop down list by switching the button to normal state
    SAFE_CALL( m_spListButton )->SetActive( false );

    // Dispatch event
    DispatchEvent( EVENT_COMBOBOX_VALUE_CHANGED, GetSharedPtr< ComboBox >(), i64Data, strValue );
}
        
void ComboBox::OnButtonActivated()
{
    // Button is down
    ShowDropDownList( true );
}
        
void ComboBox::OnButtonDeactivated()
{
    // Button is up
    ShowDropDownList( false );
}

const CFoundation::String ComboBoxCreator::GetTypeId() const
{
	return ComboBox::GetTypeID();
}

GuiObjectPtr ComboBoxCreator::CreateObject( const CFoundation::String &sName ) const
{
	ComboBoxPtr spComboBox( new ComboBox( sName ) );

    // Init
    InitDefault( spComboBox );

	return spComboBox;
}

void ComboBoxCreator::InitDefault( GuiObjectPtr _spObject ) const
{
	CombinedCtrlCreator::InitDefault( _spObject );

	ComboBoxPtr spComboBox = static_pointer_cast< ComboBox >( _spObject );

    /*
    // Create edit box
	EditBoxPtr spEditBox = GuiMain::GetInstance().Create< EditBox >( "theEditBox" );
	spComboBox->AddPieceLast( spEditBox );
	spComboBox->m_spEditBox = spEditBox;

    // Init edit box
    spEditBox->SetText( "type text" );

    // Create button
	ButtonPtr spListButton = GuiMain::GetInstance().Create<Button>( "theListButton" );
	spComboBox->AddPieceLast( spListButton );
	spComboBox->m_spListButton = spListButton;

    // Init button
    spListButton->RegisterDelegate( CGui::STATE_EVENT_ACTIVATED, spComboBox->m_spDelegateButtonActivated );
    spListButton->RegisterDelegate( CGui::STATE_EVENT_DEACTIVATED, spComboBox->m_spDelegateButtonDeactivated );
	spListButton->SetSizeRatio( 1.0 );
    spListButton->SetLocalAttachPoint( ATTACH_POINT_TOP_RIGHT );
    spListButton->SetReferenceAttachPoint( ATTACH_POINT_TOP_RIGHT );

    // Create list box
	ListBoxPtr spListBox = GuiMain::GetInstance().Create<ListBox>( "theDropDownList" );
	//spComboBox->AddPieceLast( spListBox );
    spListBox->SetSizeModeY( SIZE_MODE_CONTENT );
	spComboBox->m_spDropDownList = spListBox;

    // Init list box
    spListBox->RegisterDelegate( EVENT_LISTBOX_SELECTED_CELL, spComboBox->m_spDelegateSelectedCell );

	ListBoxColumnPtr spListBoxColumn = GuiMain::GetInstance().Create<ListBoxColumn>( "theDropDownListColumn" );
	spListBox->AddColumn( spListBoxColumn );
	spListBox->SetHeaderVisible( false );
	//spListBox->SetVisible( false );
    */
}

void ComboBoxCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	CombinedCtrlCreator::InitFromXML( _spObject, _spXMLElement );

	ComboBoxPtr spComboBox = static_pointer_cast< ComboBox >( _spObject );

    // Get sub controls    
    spComboBox->m_spEditBox = static_pointer_cast< EditBox >( spComboBox->FindChild( "theEditBox" ) );

    ButtonPtr spListButton = static_pointer_cast< Button >( spComboBox->FindChild( "theListButton" ) );
    if ( spListButton != NULL )
    {
        spListButton->RegisterDelegate( CGui::STATE_EVENT_ACTIVATED, spComboBox->m_spDelegateButtonActivated );
        spListButton->RegisterDelegate( CGui::STATE_EVENT_DEACTIVATED, spComboBox->m_spDelegateButtonDeactivated );
    }
    spComboBox->m_spListButton = spListButton;

    ListBoxPtr spListBox = static_pointer_cast< ListBox >( spComboBox->FindChild( "theDropDownList" ) );
    if ( spListBox != NULL )
    {
        spListBox->RegisterDelegate( EVENT_LISTBOX_SELECTED_CELL, spComboBox->m_spDelegateSelectedCell );
    }
    spComboBox->m_spDropDownList = spListBox;

    spComboBox->SetText( _spXMLElement->GetAttribute( "text" ) );

    if ( _spXMLElement->HasAttribute( "font" ) ||
        _spXMLElement->HasAttribute( "font_size" ) )
    {
        FontCPtr spFont = spComboBox->GetFont();

        CFoundation::String strFontName = spFont != NULL ? spFont->GetName() : "";
        _spXMLElement->GetAttribute( "font", strFontName );

        Unsigned8 u8FontSize = spFont != NULL ? spFont->GetSize() : 12;
        _spXMLElement->GetAttributeAsUnsigned8( "font_size", u8FontSize );

        spFont = FontLoader::GetInstance().LoadFontFromFile( strFontName, u8FontSize );
        spComboBox->SetFont( spFont );
    }

    // Get bar height
    if ( _spXMLElement->HasAttribute( "bar_height" ) )
    {
        spComboBox->SetBarHeight( _spXMLElement->GetAttribute( "bar_height" ).ToUnsigned32() );
    }
}
/*
ComboBox& ComboBoxCreator::AssignStyle( GuiObject& clObject, const CGui::CtrlStyle& clStyle ) const
{
    CombinedCtrlCreator::AssignStyle( clObject, clStyle );

	ComboBox &clComboBox = static_cast< ComboBox& >( clObject );

    CFoundation::XMLElementConstPtr pRoot = clStyle.GetRoot();
    if ( pRoot == NULL )
    {
        return clComboBox;
    }

	CFoundation::String sBarHeight = pRoot->GetAttribute( "bar_height" );
    clComboBox.SetBarHeight( sBarHeight.ToUnsigned32() );

	CFoundation::XMLNodePtr pChild = pRoot->GetFirstChild();
	while ( pChild != NULL )
	{
		if ( pChild->IsElement() )
		{
            CFoundation::XMLElementPtr spElement = static_pointer_cast< CFoundation::XMLElement >( pChild );

		    const CFoundation::String sName = spElement->GetName();

		    if( sName == "editbox_style" )
		    {
			    clComboBox.GetEditBox().SetStyle( CtrlStyle( *spElement ) );
		    }
		    else if( sName == "button_style" )
		    {
			    clComboBox.GetButton().SetStyle( CtrlStyle( *spElement ) );
		    }
		    else if( sName == "listbox_style" )
		    {
			    clComboBox.GetListBox().SetStyle( CtrlStyle( spElement ) );
		    }
        }
			
        pChild = pChild->GetNextSibling();
	}

	return clComboBox;
}*/

//CFoundation::XMLElement& ComboBoxCreator::SaveToXML( CFoundation::XMLElement &clXMLElement, const GuiNode &clNode ) const
//{
//	CombinedCtrlCreator::SaveToXML( clXMLElement, clNode );
//
//	const ComboBox &clComboBox = reinterpret_cast<const ComboBox&>( clNode.GetObject() );
//
//	// set text settings
//	_spXMLElement->SetAttribute( "text", clComboBox.GetText() );
//	_spXMLElement->SetAttribute( "font", clComboBox.GetFontName() );
//	_spXMLElement->SetAttribute( "font_size", CFoundation::String( clComboBox.GetFontSize() ) );
//
//	return clXMLElement;
//}
