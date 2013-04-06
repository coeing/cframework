#include "cgui/Gui/CheckBox.h"

#include "cfoundation/XML/XMLElement.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/CtrlStyle.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/SizeMode.h"
#include "cgui/Gui/TextBox.h"

using namespace CGui;

CheckBox::CheckBox( const CFoundation::String &sName )
:	CombinedCtrl( sName ),
	m_f32ButtonRatio( 1.0f ),
	m_bButtonScaleable( false )
{
    // Create delegates
    m_spDelegateButtonActivated = StateEventDelegate0Ptr( new StateEventDelegate0( this, &CheckBox::OnButtonActivated ) );
    m_spDelegateButtonDeactivated = StateEventDelegate0Ptr( new StateEventDelegate0( this, &CheckBox::OnButtonDeactivated ) );
}

CheckBox::~CheckBox()
{
}

const CFoundation::String CheckBox::GetTypeID()
{
	return "checkbox";
}

const CFoundation::String CheckBox::GetTypeId() const
{
	return CheckBox::GetTypeID();
}
		
void CheckBox::Clear()
{
    // Remove delegates
    if ( m_spButton != NULL )
    {
        m_spButton->RemoveDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateButtonActivated );
        m_spButton->RemoveDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateButtonDeactivated );
    }

    CombinedCtrl::Clear();
}

void CheckBox::SetChecked( bool _bChecked )
{
    if ( _bChecked == IsActive() )
    {
        return;
    }

	SetActive( _bChecked );

    // Dispatch event
    DispatchEvent( EVENT_CHECKBOX_CHECK_CHANGED, GetSharedPtr< CheckBox >(), _bChecked, !_bChecked );
}

bool CheckBox::IsChecked() const
{
	return IsActive();
}

void CheckBox::SetText( const CFoundation::String& sText )
{
    m_spTextBox->SetText( sText );
}

const CFoundation::String& CheckBox::GetText() const
{
    return m_spTextBox->GetText();
}

void CheckBox::SetFont( FontCPtr _spFont )
{
    m_spTextBox->SetFont( _spFont );
}
        
void CheckBox::InitButton()
{
    if ( m_spButton != NULL )
    {
        DeinitButton();
    }

    m_spButton = dynamic_pointer_cast< Button >( FindChild( "theButton" ) );

    if ( m_spButton != NULL )
    {        
        // Register delegates
        m_spButton->RegisterDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateButtonActivated );
        m_spButton->RegisterDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateButtonDeactivated );
    }
}

void CheckBox::DeinitButton()
{
    if ( m_spButton == NULL )
    {   
        return;
    }
    
    // Remove delegates
    m_spButton->RemoveDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateButtonActivated );
    m_spButton->RemoveDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateButtonDeactivated );

    m_spButton.reset();
}

void CheckBox::SetButtonScaleable( bool bScaleable )
{
    if( m_bButtonScaleable != bScaleable )
    {
        m_bButtonScaleable = bScaleable;

        SAFE_CALL( m_spButton )->SetSizeModeY( m_bButtonScaleable ? SIZE_MODE_STRETCH : SIZE_MODE_CUSTOM );
    }
}
        
void CheckBox::OnButtonActivated()
{
    SetChecked( true );
}
        
void CheckBox::OnButtonDeactivated()
{
    SetChecked( false );
}

const CFoundation::String CheckBoxCreator::GetTypeId() const
{
	return CheckBox::GetTypeID();
}

GuiObjectPtr CheckBoxCreator::CreateObject( const CFoundation::String &sName ) const
{
	CheckBoxPtr spCheckBox( new CheckBox( sName ) );

    // Init
    InitDefault( spCheckBox );

	return spCheckBox;
}

void CheckBoxCreator::InitDefault( GuiObjectPtr _spObject ) const
{
	CombinedCtrlCreator::InitDefault( _spObject );

	CheckBoxPtr spCheckBox = static_pointer_cast< CheckBox >( _spObject );

    /*
    // Create button
    ButtonPtr spButton = GuiMain::GetInstance().Create< Button >( "theButton" );
    spCheckBox->AddPieceLast( spButton );
	spCheckBox->m_spButton = spButton;

    // Init button
    spButton->SetType( Button::TYPE_TOGGLE );
    spButton->AddListener( spCheckBox, StateChangedMsg::GetType() );

    // Create text box
	TextBoxPtr spTextBox = GuiMain::GetInstance().Create< TextBox >( "theTextBox" );
    spCheckBox->AddPieceLast( spTextBox );
	spCheckBox->m_spTextBox = spTextBox;

    // Init text box
    spTextBox->SetAttachObject( ATTACH_OBJECT_PREV_SIBLING );
    spTextBox->SetReferenceAttachPoint( ATTACH_POINT_TOP_RIGHT );
    spTextBox->SetSizeMode( SIZE_MODE_STRETCH );*/
}

void CheckBoxCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	CombinedCtrlCreator::InitFromXML( _spObject, _spXMLElement );

	CheckBoxPtr spCheckBox = static_pointer_cast< CheckBox >( _spObject );

    // Get controls
    spCheckBox->m_spTextBox = dynamic_pointer_cast< TextBox >( spCheckBox->FindChild( "theTextBox" ) );

    // Init controls
    spCheckBox->InitButton();

    // Get text
    if ( _spXMLElement->HasAttribute( "text" ) )
    {
        spCheckBox->SetText( _spXMLElement->GetAttribute( "text" ) );
    }

    // Check if button should be scaled
    if ( _spXMLElement->HasAttribute( "scale_button" ) )
    {
        spCheckBox->SetButtonScaleable( _spXMLElement->GetAttributeAsBool( "scale_button" ) );
    }

	CFoundation::XMLNodePtr pChild = _spXMLElement->GetFirstChild();
	while ( pChild != NULL )
	{
		if ( pChild->IsElement() )
		{
            CFoundation::XMLElementPtr spElement = static_pointer_cast< CFoundation::XMLElement >( pChild );

            const CFoundation::String& sName = spElement->GetName();
            if( sName == "button_style" )
            {
                SAFE_CALL( spCheckBox->m_spButton )->SetStyle( CtrlStylePtr( new CtrlStyle( spElement ) ) );
            }
            else if( sName == "textbox_style" )
            {
                SAFE_CALL( spCheckBox->m_spTextBox )->SetStyle( CtrlStylePtr( new CtrlStyle( spElement ) ) );
            }
        }
			
        pChild = pChild->GetNextSibling();
    }
}
		
void CheckBoxCreator::InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const
{
	CombinedCtrlCreator::InitFromTemplate( _spObject, _spTemplate );

	CheckBoxPtr spCheckBox = static_pointer_cast< CheckBox >( _spObject );
	CheckBoxCPtr spTemplate = static_pointer_cast< const CheckBox >( _spTemplate );
}
