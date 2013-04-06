#include "cgui/Gui/ListBoxCell.h"

#include "cfoundation/Debug/Assert.h"

#include "cgui/Gui/CheckBox.h"
#include "cgui/Gui/ComboBox.h"
#include "cgui/Gui/EditBox.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/SizeMode.h"
#include "cgui/Gui/TextBox.h"

#include "cgui/Input/Input.h"

using namespace CGui;

///////////////////////////////////////////////////////////////////////////////
///
/// ListBoxCell
///
///////////////////////////////////////////////////////////////////////////////

ListBoxCell::ListBoxCell( const CFoundation::String& _strName )
:	CombinedCtrl( _strName ),
	m_eType( NUM_TYPES )
{
    // Create delegates
    m_spDelegateCellTextChanged = EditBoxEventDelegate3Ptr( new EditBoxEventDelegate3( this, &ListBoxCell::OnCellTextChanged ) );
    m_spDelegateCheckChanged = CheckBoxEventDelegate3Ptr( new CheckBoxEventDelegate3( this, &ListBoxCell::OnCheckChanged ) );
}
		
void ListBoxCell::Clear()
{
    // Remove cell
    SetCell( GuiObjectPtr() );

    // TODO CO Remove delegates from cached cells

    CombinedCtrl::Clear();
}

const CFoundation::String ListBoxCell::GetTypeID()
{
	return "listboxcell";
}

const CFoundation::String ListBoxCell::GetTypeId() const
{
	return ListBoxCell::GetTypeID();
}
        
void ListBoxCell::SetStateFlag( StateFlag _eStateFlag, bool _bSet )
{
    // Don't set state for list box pieces
    Panel::SetStateFlag( _eStateFlag, _bSet );
}
        
bool ListBoxCell::ProcessInputFocused( const CSystem::InputState& _State )
{
    bool bProcessed = CombinedCtrl::ProcessInputFocused( _State );
    if ( bProcessed )
    {
        return true;
    }

    // Let cell process input
    if ( Input::GetInstance().GetFocusObject() == GetSharedPtr() )
    {
        return m_spCell != NULL && m_spCell->ProcessInputFocused( _State );
    }
    else
    {
        return false;
    }
}

bool ListBoxCell::ProcessInputPointed( const CSystem::InputState& _State )
{
    bool bProcessed = CombinedCtrl::ProcessInputPointed( _State );
    if ( bProcessed )
    {
        return true;
    }

    // Let cell process input
    return m_spCell != NULL && m_spCell->ProcessInputPointed( _State );
}

void ListBoxCell::OnGainedFocus()
{
    CombinedCtrl::OnGainedFocus();
}

void ListBoxCell::SetType( const CellType& eType )
{
	if ( m_eType != eType )
	{
		m_eType = eType;

        // Get cell of new type
        GuiObjectPtr spNewCell = GetCell( m_eType );

        // Set new cell
        SetCell( spNewCell );
	}
}

const CellType& ListBoxCell::GetType() const
{
	return m_eType;
}

void ListBoxCell::SetValue( const CFoundation::String &sText )
{
	switch ( m_eType )
	{
	case TYPE_TEXTBOX:
        {
		    TextBoxPtr spTextCell = static_pointer_cast< TextBox >( m_spCell );
		    spTextCell->SetText( sText );
        }
		break;

	case TYPE_EDITBOX:
        {
		    EditBoxPtr spEditCell = static_pointer_cast< EditBox >( m_spCell );
		    spEditCell->SetText( sText );
        }
		break;

	case TYPE_CHECKBOX:
        {
		    CheckBoxPtr spCheckCell = static_pointer_cast< CheckBox >( m_spCell );
		    if( sText == "1" )
		    {
			    spCheckCell->SetChecked( true );
		    }
		    else
		    {
			    spCheckCell->SetChecked( false );
		    }
        }
		break;
    default:
        break;
	}
}
       
void ListBoxCell::SetValues( const std::vector< CFoundation::String >& _astrValues )
{
	switch ( m_eType )
	{
	case TYPE_COMBOBOX:
        {
		    ComboBoxPtr spComboBoxCell = static_pointer_cast< ComboBox >( m_spCell );
            Unsigned32 u32NumValues = _astrValues.size();
            for ( Unsigned32 u32Idx = 0; u32Idx < u32NumValues; ++u32Idx )
            {
                spComboBoxCell->AddItem( _astrValues[ u32Idx ], u32Idx );
            }
        }
		break;
    default:
        {
            CF_WARN( "Invalid cell type for multiple values." );
        }
        break;
	}
}

void ListBoxCell::SetFont( FontCPtr _spFont )
{
    m_spFont = _spFont;
	switch( m_eType )
	{
    case TYPE_TEXTBOX:
        {
		    TextBoxPtr spTextCell = static_pointer_cast< TextBox >( m_spCell );
            spTextCell->SetFont( m_spFont );
        }
        break;
    case TYPE_EDITBOX:
        {
		    EditBoxPtr spEditCell = static_pointer_cast< EditBox >( m_spCell );
            spEditCell->SetFont( m_spFont );
        }
        break;
    case TYPE_CHECKBOX:
        {
		    CheckBoxPtr spCheckCell = static_pointer_cast< CheckBox >( m_spCell );
            spCheckCell->SetFont( m_spFont );
        }
        break;
    case TYPE_COMBOBOX:
        {
		    ComboBoxPtr spCell = static_pointer_cast< ComboBox >( m_spCell );
            spCell->SetFont( m_spFont );            
        }
        break;
    default:
        break;
	}
}

GuiObjectPtr ListBoxCell::GetCell( CellType _eType )
{
	// Search in cache
	ObjectMap::iterator it = m_clObjects.find( _eType );
	if( it != m_clObjects.end() )
	{
		return it->second;
	}

    // Create cell
    GuiObjectPtr spCell;
	switch ( _eType )
	{
	case TYPE_TEXTBOX:
		{
			TextBoxPtr spTextCell = GuiMain::GetInstance().CreateDefault< TextBox >( "ListBoxCell" );

			// set font
			spTextCell->SetFont( m_spFont );

			spCell = spTextCell;
		}
		break;

	case TYPE_EDITBOX:
		{
			EditBoxPtr spEditCell = GuiMain::GetInstance().CreateDefault<EditBox>( "ListBoxCell" );

            // Register delegates
            spEditCell->RegisterDelegate( EVENT_EDITBOX_TEXT_CHANGED, m_spDelegateCellTextChanged );

			// set font
            spEditCell->SetFont( m_spFont );

			spCell = spEditCell;
		}
		break;

	case TYPE_CHECKBOX:
        {
            CheckBoxPtr spCheckCell = GuiMain::GetInstance().CreateDefault<CheckBox>( "ListBoxCell" );

            // Register delegates
            spCheckCell->RegisterDelegate( EVENT_CHECKBOX_CHECK_CHANGED, m_spDelegateCheckChanged );            

			// set font
            spCheckCell->SetFont( m_spFont );

            spCell = spCheckCell;
        }
		break;

    case TYPE_COMBOBOX:
        {
            ComboBoxPtr spComboBoxCell = GuiMain::GetInstance().CreateDefault< ComboBox >( "ListBoxCell" );

			// Set font
            spComboBoxCell->SetFont( m_spFont );

            spCell = spComboBoxCell;       
        }
        break;

    default:
        break;
	}

	// Enable messages
	if ( spCell != NULL )
	{
        // Set size mode
        spCell->SetSizeMode( SIZE_MODE_STRETCH );

        // Set background
        spCell->SetBgVisible( false );
	}

	// add to cache
	m_clObjects[ _eType ] = spCell;

    return spCell;
}
        
void ListBoxCell::SetCell( GuiObjectPtr _spCell )
{
    if ( m_spCell != NULL )
    {		
        // Remove as piece
        RemovePiece( m_spCell );
    }

    m_spCell = _spCell;

    if ( m_spCell != NULL )
    {
        // Add cell as piece
		AddPieceLast( m_spCell );
    }
}
        
void ListBoxCell::OnCellValueChanged( ComboBoxPtr _spComboBox, Integer64 _i64Data, const CFoundation::String& _strNewValue )
{
    // Dispatch event
    DispatchEvent( EVENT_LISTBOXCELL_VALUE_CHANGED, GetSharedPtr< ListBoxCell >(), _strNewValue, _strNewValue );
}
        
void ListBoxCell::OnCheckChanged( CheckBoxPtr _spCheckBox, bool _bNewState, bool _bOldState )
{
	CFoundation::String strOldValue = _bOldState ? "1" : "0";
	CFoundation::String strNewValue = _bNewState ? "1" : "0";

    // Dispatch event
    DispatchEvent( EVENT_LISTBOXCELL_VALUE_CHANGED, GetSharedPtr< ListBoxCell >(), strNewValue, strOldValue );
}
       
void ListBoxCell::OnCellTextChanged( EditBoxPtr _spEditBox, const CFoundation::String& _strNewValue, const CFoundation::String& _strOldValue )
{
    // Dispatch event
    DispatchEvent( EVENT_LISTBOXCELL_VALUE_CHANGED, GetSharedPtr< ListBoxCell >(), _strNewValue, _strOldValue );
}

///////////////////////////////////////////////////////////////////////////////
///
/// ListBoxCellCreator
///
///////////////////////////////////////////////////////////////////////////////

const CFoundation::String ListBoxCellCreator::GetTypeId() const
{
	return ListBoxCell::GetTypeID();
}

GuiObjectPtr ListBoxCellCreator::CreateObject( const CFoundation::String &sName ) const
{
	ListBoxCellPtr spObject( new ListBoxCell( sName ) );
	return spObject;
}
