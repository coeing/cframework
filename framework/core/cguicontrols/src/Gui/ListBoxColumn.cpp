#include "cgui/Gui/ListBoxColumn.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/XML/XMLElement.h"

#include "csystem/Input/InputState.h"

#include "cgui/Font/FontLoader.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/ListBoxCell.h"
#include "cgui/Gui/SizeMode.h"
#include "cgui/Gui/TextBox.h"

#include "cgui/Input/Input.h"

using namespace CGui;

ListBoxColumn::ListBoxColumn( const CFoundation::String &sName )
:	CombinedCtrl( sName ),
	m_u32MinWidth( 0 ),
	m_u32HeaderHeight( 0 ),
	m_u8GridSize( 0 ),
	m_u32RowHeight( 0 ),
    m_i32FocusedRow( -1 ),
	m_i32HighlightedRow( -1 )
{
	m_spCellFont = FontLoader::GetInstance().LoadFontFromFile( "common/fonts/arial.ttf", 12 );

    // Create delegates
    m_spDelegateCellFocusGained = ObjectInputEventDelegate1Ptr( new ObjectInputEventDelegate1( this, &ListBoxColumn::OnCellFocusGained ) );
    m_spDelegateCellFocusLost = ObjectInputEventDelegate1Ptr( new ObjectInputEventDelegate1( this, &ListBoxColumn::OnCellFocusLost ) );
    m_spDelegateClickedCell = ObjectInputEventDelegate1Ptr( new ObjectInputEventDelegate1( this, &ListBoxColumn::OnClickedCell ) );
    m_spDelegateDoubleClickedCell = ObjectInputEventDelegate1Ptr( new ObjectInputEventDelegate1( this, &ListBoxColumn::OnDoubleClickedCell ) );
    m_spDelegateCellValueChanged = ListBoxCellEventDelegate3Ptr( new ListBoxCellEventDelegate3( this, &ListBoxColumn::OnCellValueChanged ) );
}

const CFoundation::String ListBoxColumn::GetTypeID()
{
	return "column";
}

const CFoundation::String ListBoxColumn::GetTypeId() const
{
	return ListBoxColumn::GetTypeID();
}
		
void ListBoxColumn::Clear()
{
    // Remove cells
    CellVec aspCells = m_aspCells;
    for ( CellVec::iterator it = aspCells.begin(); it != aspCells.end(); ++it )
    {
        ListBoxCellPtr spCell = *it;
        spCell->Clear();
        RemoveCell( spCell );
    }

    CombinedCtrl::Clear();
}

void ListBoxColumn::SetMinWidth( Unsigned32 u32MinWidth )
{
	m_u32MinWidth = u32MinWidth;
	if ( m_u32MinWidth > GetWidth() )
	{
		SetCustomWidth( m_u32MinWidth );
	}
}

Unsigned32 ListBoxColumn::GetMinWidth() const
{
	return m_u32MinWidth;
}

void ListBoxColumn::SetSize( const CFoundation::Vector2Du& _clSize )
{
	if ( GetSize() != _clSize )
	{	
	    Unsigned32 u32OldHeight = GetHeight();

        CombinedCtrl::SetSize( _clSize );

		if ( u32OldHeight != GetHeight() )
		{
			if( u32OldHeight < GetHeight() &&
                GetSizeModeY() != SIZE_MODE_CONTENT )
			{
				CreateNewCells();
            }
			HideCells();
		}
	}
}

bool ListBoxColumn::ProcessInputFocused( const CSystem::InputState& _State )
{
    // Check for focus change
    bool bChangeFocus = false;
    Unsigned32 u32NewFocusRow = 0;
    if ( _State.m_eKeyStates[ CSystem::KEY_DOWN ] == CSystem::KEY_STATE_FIRST_DOWN )
    {
        // Focus next cell
        bChangeFocus = true;        
        if ( m_i32FocusedRow < 0 )
        {
            u32NewFocusRow = 0;
        }
        else
        {
            u32NewFocusRow = m_i32FocusedRow + 1;
        }
    }
    else if ( _State.m_eKeyStates[ CSystem::KEY_UP ] == CSystem::KEY_STATE_FIRST_DOWN )
    {
        // Focus previous cell
        bChangeFocus = true;     
        if ( m_i32FocusedRow <= 0 )
        {
            u32NewFocusRow = m_aspCells.size() - 1;
        }
        else
        {
            u32NewFocusRow = m_i32FocusedRow - 1;
        }
    }
    
    if ( bChangeFocus &&
         m_aspCells.size() > 0 )
    {
        if ( u32NewFocusRow >= m_aspCells.size() )
        {
            u32NewFocusRow = 0;
        }
        Input::GetInstance().SetFocusObject( m_aspCells[ u32NewFocusRow ] );
    }

    return CombinedCtrl::ProcessInputFocused( _State );
}

bool ListBoxColumn::ProcessInputPointed( const CSystem::InputState& _State )
{
    return CombinedCtrl::ProcessInputPointed( _State );
}

void ListBoxColumn::ClearContent()
{
    GuiObjectPtr spFocusObject = Input::GetInstance().GetFocusObject();

    Unsigned32 u32NumRows = GetNumRows();
	for ( Unsigned32 u32Row = 0; u32Row < u32NumRows; ++u32Row )
	{
	    ListBoxCellPtr spCell = m_aspCells[ u32Row ];

        // Reset data
        spCell->SetActive( false );

        // Hide cells
        spCell->SetVisible( false );

        // Clear focus object if cell is focused
        if ( spFocusObject == spCell ||
             spFocusObject != NULL &&
             spCell->IsDescendant( spFocusObject ) )
        {
            Input::GetInstance().SetFocusObject( GuiObjectPtr() );
            spFocusObject.reset();
        }

        /*SetCellType( u32Row, TYPE_TEXTBOX );
        SetCellValue( u32Row, "" );
        SetCellHighlighted( u32Row, false );*/
	}

    // Clear data
    m_clCellData.clear();

    m_i32HighlightedRow = -1;
}

void ListBoxColumn::SetHighlightedRow( Integer32 _i32Row )
{
    if ( m_i32HighlightedRow == _i32Row )
    {
        return;
    }

	// remove highlight from old row
	if ( m_i32HighlightedRow >= 0 )
	{
		CellData &clData = m_clCellData[ m_i32HighlightedRow ];
		clData.m_bHighlighted = false;
	    ListBoxCellPtr pCell = GetCell( m_i32HighlightedRow );
        if ( pCell != NULL )
        {
            pCell->SetActive( clData.m_bHighlighted );
        }
	}

	m_i32HighlightedRow = _i32Row;

	if ( m_i32HighlightedRow >= 0 )
	{
	    CellData &clData = m_clCellData[ m_i32HighlightedRow ];
	    clData.m_bHighlighted = true;
	    ListBoxCellPtr pCell = GetCell( m_i32HighlightedRow );
        if ( pCell != NULL )
        {
            pCell->SetActive( clData.m_bHighlighted );
        }
    }
}

ListBoxCellPtr ListBoxColumn::AddCell()
{
	// Create standard cell data
	Unsigned32 u32Row = GetNumRows();
	CellData &clData = m_clCellData[ u32Row ];
	clData.m_clTextColor = u32Row % 2 == 0 ? m_clTextColor1 : m_clTextColor2;

	// Create standard cell
	ListBoxCellPtr spCell = GuiMain::GetInstance().CreateDefault< ListBoxCell >( "ListBoxCell" );

	AddPieceLast( spCell );
	m_aspCells.push_back( spCell );

    // Set type
    //spCell->SetType( TYPE_TEXTBOX );

    // Register delegates
    spCell->RegisterDelegate( INPUT_EVENT_GAINED_FOCUS, m_spDelegateCellFocusGained );
    spCell->RegisterDelegate( INPUT_EVENT_LOST_FOCUS, m_spDelegateCellFocusLost );
    spCell->RegisterDelegate( INPUT_EVENT_CLICKED, m_spDelegateClickedCell );
    spCell->RegisterDelegate( INPUT_EVENT_DOUBLE_CLICKED, m_spDelegateDoubleClickedCell );
    spCell->RegisterDelegate( EVENT_LISTBOXCELL_VALUE_CHANGED, m_spDelegateCellValueChanged );

    // Set size mode
    spCell->SetSizeModeX( SIZE_MODE_STRETCH );

	// Set height
	spCell->SetCustomHeight( GetRowHeight() );

    // Set background colors
    spCell->SetBgColor( STATE_NORMAL, u32Row % 2 == 0 ? m_clRowColor1 : m_clRowColor2 );
    spCell->SetBgColor( STATE_ACTIVE, m_clHighlightColor );

	// Set font
	spCell->SetFont( m_spCellFont );

    // Reposition cells
    RepositionCells();

	return spCell;
}
       
void ListBoxColumn::RemoveCell( ListBoxCellPtr _spCell )
{
    CellVec::iterator it = std::find( m_aspCells.begin(), m_aspCells.end(), _spCell );
    if ( it == m_aspCells.end() )
    {
        CF_WARN( "ListBoxColumn::RemoveCell: Tried to remove cell that's no one of this column." );
        return;
    }

	// Remove delegates
    _spCell->RemoveDelegate( INPUT_EVENT_GAINED_FOCUS, m_spDelegateCellFocusGained );
    _spCell->RemoveDelegate( INPUT_EVENT_LOST_FOCUS, m_spDelegateCellFocusLost );
    _spCell->RemoveDelegate( INPUT_EVENT_CLICKED, m_spDelegateClickedCell );
    _spCell->RemoveDelegate( INPUT_EVENT_DOUBLE_CLICKED, m_spDelegateDoubleClickedCell );
    _spCell->RemoveDelegate( EVENT_LISTBOXCELL_VALUE_CHANGED, m_spDelegateCellValueChanged );

    // Remove from cells
	m_aspCells.erase( it );

    // Remove as piece
	RemovePiece( _spCell );
}
        
Integer32 ListBoxColumn::GetCellIndex( GuiObjectPtr _spCell ) const
{
    // Find index
    CellVec::const_iterator it = std::find( m_aspCells.begin(), m_aspCells.end(), _spCell );
    if ( it != m_aspCells.end() )
    {
        return it - m_aspCells.begin();
    }
    else
    {
        return -1;
    }
}

void ListBoxColumn::CreateNewCells()
{
	if( GetRowHeight() > 0 )
	{
		Unsigned32 u32OffsetY = 0;
		if( IsHeaderVisible() )
		{
			u32OffsetY += GetHeaderHeight() + GetGridSize();
		}

		Unsigned16 u16Row = 0;

		// create new TextBoxes
        Unsigned32 u32Height = GetHeight() + GetRowHeight();
		while ( u32OffsetY < u32Height )
		{
			ListBoxCellPtr pCell = GetCell( u16Row );
			if( pCell == NULL )	// => have to create a new textbox
			{
				pCell = AddCell();
                pCell->SetOffset( CFoundation::Vector2Di( 0, u32OffsetY ) );
			}

			u32OffsetY += GetRowHeight() + GetGridSize();
			++u16Row;
		}
	}
}

void ListBoxColumn::ResizeCells()
{
	for( Unsigned16 u16Row = 0; u16Row < m_aspCells.size(); ++u16Row )
	{
		ListBoxCellPtr spCell = m_aspCells[ u16Row ];

		if( !spCell->IsVisible() )
		{
			return;
		}

		// set size
		spCell->SetCustomHeight( GetRowHeight() );
	}
}

void ListBoxColumn::RepositionCells()
{
	Unsigned32 u32OffsetY = 0;
	if ( IsHeaderVisible() )
	{
		u32OffsetY += GetHeaderHeight() + GetGridSize();
	}

	for( Unsigned32 u32Row = 0; u32Row < m_aspCells.size(); ++u32Row )
	{
		ListBoxCellPtr spCell = m_aspCells[ u32Row ];

		// set position
		spCell->SetOffset( CFoundation::Vector2Di( 0, u32OffsetY ) );

		u32OffsetY += GetRowHeight() + GetGridSize();
	}
}

void ListBoxColumn::HideCells()
{
	for ( Unsigned32 u32Row = 0; u32Row < m_aspCells.size(); ++u32Row )
	{
		ListBoxCellPtr pCell = m_aspCells[ u32Row ];
        if ( pCell->GetRelativePosition().GetY() > static_cast< Integer32 >( GetHeight() ) )
		{
			pCell->SetVisible( false );
		}
		else
		{
			if ( !pCell->IsVisible() )
			{
				pCell->SetVisible( true );
				pCell->SetCustomSize( CFoundation::Vector2Du( GetWidth(), GetRowHeight() ) );
			}
		}
	}
}

ListBoxCellPtr ListBoxColumn::GetCell( Unsigned32 _u32Row )
{
	if( _u32Row < m_aspCells.size() )
	{
		return m_aspCells[ _u32Row ];
	}
	else
	{
		return ListBoxCellPtr();
	}
}

ListBoxCellCPtr ListBoxColumn::GetCell( Unsigned32 _u32Row ) const
{
	if( _u32Row < m_aspCells.size() )
	{
		return m_aspCells[ _u32Row ];
	}
	else
	{
		return ListBoxCellCPtr();
	}
}
       
ListBoxCellPtr ListBoxColumn::CreateCell( Unsigned32 _u32Row )
{
    ListBoxCellPtr spCell = GetCell( _u32Row );
    if ( spCell == NULL )
    {
        while ( m_aspCells.size() <= _u32Row )
        {
            spCell = AddCell();
        }
    }

    // Show cell
    spCell->SetVisible( true );

    return spCell;
}
        
CFoundation::Vector2Du ListBoxColumn::ComputeSize()
{
    CFoundation::Vector2Du vNewSize = CombinedCtrl::ComputeSize();

    if ( GetSizeModeY() == SIZE_MODE_CONTENT )
    {
        // Add grid size
        vNewSize.AddY( GetGridSize() );
    }

    return vNewSize;
}
       
void ListBoxColumn::OnPieceGainedFocus( GuiObjectPtr _spPiece )
{
    Integer32 i32Idx = GetCellIndex( _spPiece );
    if ( i32Idx >= 0 )
    {
        // Dispatch event
        DispatchEvent( EVENT_LISTBOXCOLUMN_FOCUSED_ROW, GetSharedPtr< GuiObject >(), i32Idx );
    }

    // Nothing to do, cells can gain focus on their own
}

void ListBoxColumn::OnPieceLostFocus( GuiObjectPtr _spPiece )
{
    // Nothing to do, cells can gain focus on their own
}

TextBoxPtr ListBoxColumn::GetHeader()
{
	return m_pHeader;
}

TextBoxCPtr ListBoxColumn::GetHeader() const
{
	return m_pHeader;
}

void ListBoxColumn::SetTitle( const CFoundation::String &sTitle )
{
	GetHeader()->SetText( sTitle );
}

const CFoundation::String& ListBoxColumn::GetTitle() const
{
	return GetHeader()->GetText();
}

Unsigned32 ListBoxColumn::GetHeaderHeight() const
{
	if( IsHeaderVisible() )
	{
		return m_u32HeaderHeight;
	}
	else
	{
		return 0;
	}
}

void ListBoxColumn::SetHeaderFont( FontCPtr _spFont )
{
    m_pHeader->SetFont( _spFont );
}

Unsigned8  ListBoxColumn::GetGridSize() const
{
	return m_u8GridSize;
}
        
void ListBoxColumn::SetGridColor( const CFoundation::Color& _clColor )
{
    SetBgColor( STATE_NORMAL, _clColor );
}

const CFoundation::Color& ListBoxColumn::GetGridColor() const
{
    return GetBgColor( STATE_NORMAL );
}

Unsigned32 ListBoxColumn::GetRowHeight() const
{
	return m_u32RowHeight;
}

void ListBoxColumn::SetGridSize( Unsigned8 u8GridSize )
{
	if( u8GridSize < m_u8GridSize )
	{
		m_u8GridSize = u8GridSize;
		CreateNewCells();
	}
	else
	{
		m_u8GridSize = u8GridSize;
	}
	RepositionCells();
	HideCells();
}

void ListBoxColumn::SetHeaderHeight( Unsigned32 u32HeaderHeight )
{
	if( u32HeaderHeight != GetHeaderHeight() )
	{
		m_pHeader->SetCustomHeight( u32HeaderHeight );
		if( u32HeaderHeight < GetHeaderHeight() )
		{
			CreateNewCells();
		}
		m_u32HeaderHeight = u32HeaderHeight;
		RepositionCells();
		HideCells();
	}
}

void ListBoxColumn::SetRowHeight( Unsigned32 _u32RowHeight )
{
    Unsigned32 u32OldRowHeight = m_u32RowHeight;
    m_u32RowHeight = _u32RowHeight;

	if ( GetSizeModeY() != SIZE_MODE_CONTENT &&
         ( m_u32RowHeight < u32OldRowHeight ||
		   u32OldRowHeight == 0 ) )
	{
		CreateNewCells();
	}

	ResizeCells();
	RepositionCells();
	HideCells();
}

void ListBoxColumn::SetCellFont( FontCPtr _spFont )
{
    m_spCellFont = _spFont;

    for( Unsigned32 u32Cell = 0; u32Cell < m_aspCells.size(); ++u32Cell )
    {
        m_aspCells[ u32Cell ]->SetFont( _spFont );
    }
}

void ListBoxColumn::SetRowColor1( const CFoundation::Color& _clColor )
{
    if ( _clColor == m_clRowColor1 )
    {
        return;
    }

	m_clRowColor1 = _clColor;

    // Set cells to new color
    Unsigned32 u32NumCells = m_aspCells.size();
    for( Unsigned32 u32Cell = 0; u32Cell < u32NumCells; u32Cell += 2 )
    {
        m_aspCells[ u32Cell ]->SetBgColor( STATE_NORMAL, m_clRowColor1 );
    }
}

void ListBoxColumn::SetTextColor1( const CFoundation::Color &clColor )
{
	m_clTextColor1 = clColor;
}

void ListBoxColumn::SetRowColor2( const CFoundation::Color& _clColor )
{
    if ( _clColor == m_clRowColor2 )
    {
        return;
    }

	m_clRowColor2 = _clColor;

    // Set cells to new color
    Unsigned32 u32NumCells = m_aspCells.size();
    for( Unsigned32 u32Cell = 1; u32Cell < u32NumCells; u32Cell += 2 )
    {
        m_aspCells[ u32Cell ]->SetBgColor( STATE_NORMAL, m_clRowColor2 );
    }
}

void ListBoxColumn::SetTextColor2( const CFoundation::Color &clColor )
{
	m_clTextColor2 = clColor;
}

void ListBoxColumn::SetHighlightColor( const CFoundation::Color& _clColor )
{
    if ( _clColor == m_clHighlightColor )
    {
        return;
    }

	m_clHighlightColor = _clColor;

    // Set cells to new color
    Unsigned32 u32NumCells = m_aspCells.size();
    for( Unsigned32 u32Cell = 0; u32Cell < u32NumCells; ++u32Cell )
    {
        m_aspCells[ u32Cell ]->SetBgColor( STATE_ACTIVE, m_clHighlightColor );
    }
}

void ListBoxColumn::SetHighlightTextColor( const CFoundation::Color &clColor )
{
	m_clHighlightTextColor = clColor;
}

Unsigned16 ListBoxColumn::GetNumRows() const
{
	return static_cast<Unsigned16>( m_aspCells.size() );
}

void ListBoxColumn::SetCellValue( Unsigned32 _u32Row, const CFoundation::String& _strValue )
{
    ListBoxCellPtr spCell = GetCell( _u32Row );
    if ( spCell == NULL )
    {
        return;
    }

	CellData &clData = m_clCellData[ _u32Row ];
	clData.m_sValue = _strValue;
    spCell->SetValue( clData.m_sValue );
}

const CFoundation::String& ListBoxColumn::GetCellValue( Unsigned32 _u32Row ) const
{
    CellDataMap::const_iterator it = m_clCellData.find( _u32Row );
    if( it != m_clCellData.end() )
    {
        return it->second.m_sValue;
    }
    else
    {
        static CFoundation::String sTmp( "" );
        return sTmp;
    }
}
       
void ListBoxColumn::SetCellValues( Unsigned32 _u32Row, const std::vector< CFoundation::String >& _astrValues )
{
    ListBoxCellPtr spCell = GetCell( _u32Row );
    if ( spCell == NULL )
    {
        return;
    }

	CellData& clData = m_clCellData[ _u32Row ];
	//clData.m_sValue = _strValue;
    spCell->SetValues( _astrValues );
}

void ListBoxColumn::SetCellData( Unsigned32 _u32Row, Integer64 i64Data )
{
    CellDataMap::iterator it = m_clCellData.find( _u32Row );
    if ( it != m_clCellData.end() )
    {
	    it->second.m_i64Data = i64Data;
    }
}

Integer64 ListBoxColumn::GetCellData( Unsigned32 _u32Row ) const
{
    CellDataMap::const_iterator it = m_clCellData.find( _u32Row );
    if ( it != m_clCellData.end() )
    {
        return it->second.m_i64Data;
    }
    else
    {
        return 0;
    }
}
        
void ListBoxColumn::SetCellData( Unsigned32 _u32Row, const std::vector< Integer64 >& _ai64Data )
{
}

void ListBoxColumn::SetCellBgColor( Unsigned16 _u16Row, const CFoundation::Color& _clColor )
{
    ListBoxCellPtr spCell = GetCell( _u16Row );
    if ( spCell == NULL )
    {
        return;
    }

    spCell->SetBgColor( STATE_NORMAL, _clColor );
}

void ListBoxColumn::SetCellType( Unsigned32 _u32Row, const CellType& _eType )
{
    ListBoxCellPtr spCell = CreateCell( _u32Row );

	CellType eNewType = _eType;
	if ( eNewType == TYPE_DEFAULT )
	{
		eNewType = TYPE_TEXTBOX;
	}

    spCell->SetType( eNewType );
}

void ListBoxColumn::SetHeaderOffset( Unsigned32 u32HeaderOffset )
{
	m_u32HeaderOffset = u32HeaderOffset;

	RepositionHeader();
}

void ListBoxColumn::SetHeaderVisible( bool bVisible )
{
	if( bVisible == m_bHeaderVisible )
	{
        return;
    }

	m_bHeaderVisible = bVisible;
	m_pHeader->SetVisible( bVisible );
	RepositionCells();
	HideCells();
}

bool ListBoxColumn::IsHeaderVisible() const
{
	return m_bHeaderVisible;
}

void ListBoxColumn::RepositionHeader()
{
	m_pHeader->SetOffset( CFoundation::Vector2Di( 0, m_u32HeaderOffset ) );
}

void ListBoxColumn::SetCellHighlighted( Unsigned32 _u32Row, bool _bHighlighted )
{
	ListBoxCellPtr pCell = GetCell( _u32Row );
	if ( pCell != NULL )
	{
		CellData& clData = m_clCellData[ _u32Row ];
        clData.m_bHighlighted = _bHighlighted;
        pCell->SetActive( clData.m_bHighlighted );
    }
}
        
void ListBoxColumn::OnCellFocusGained( GuiObjectPtr _spObject )
{
    Integer32 i32Idx = GetCellIndex( _spObject );
    if ( i32Idx < 0 )
    {
        return;
    }

    m_i32FocusedRow = i32Idx;
}

void ListBoxColumn::OnCellFocusLost( GuiObjectPtr _spObject )
{
    m_i32FocusedRow = -1;
}

void ListBoxColumn::OnClickedCell( GuiObjectPtr _spObject )
{
    Integer32 i32Idx = GetCellIndex( _spObject );
    if ( i32Idx < 0 )
    {
        return;
    }

    // Dispatch event
    DispatchEvent( EVENT_LISTBOXCOLUMN_CLICKED_ROW, GetSharedPtr< GuiObject >(), i32Idx );
}

void ListBoxColumn::OnDoubleClickedCell( GuiObjectPtr _spObject )
{
	for ( Unsigned32 u32Row = 0; u32Row < GetNumRows(); ++u32Row )
	{
		if ( _spObject == GetCell( u32Row ) )
		{
            // Dispatch event
            DispatchEvent( EVENT_LISTBOXCOLUMN_DOUBLE_CLICKED_ROW, GetSharedPtr< GuiObject >(), u32Row );
			break;
		}
	}
}

void ListBoxColumn::OnCellValueChanged( ListBoxCellPtr _spCell, const CFoundation::String& _strNewValue, const CFoundation::String& _strOldValue )
{
    Integer32 i32Idx = GetCellIndex( _spCell );
    if ( i32Idx < 0 )
    {
        return;
    }

	// Update saved data
    CellData& clData = m_clCellData[ i32Idx ];
    clData.m_sValue = _strNewValue;

    // Dispatch event
    DispatchEvent( EVENT_LISTBOXCOLUMN_VALUE_CHANGED, GetSharedPtr< ListBoxColumn >(), i32Idx, _strNewValue, _strOldValue );
}

const CFoundation::String ListBoxColumnCreator::GetTypeId() const
{
	return "column";
}

GuiObjectPtr ListBoxColumnCreator::CreateObject( const CFoundation::String &sName ) const
{
	ListBoxColumnPtr spListBoxColumn( new ListBoxColumn( sName ) );

    // Init
    InitDefault( spListBoxColumn );

	return spListBoxColumn;
}

void ListBoxColumnCreator::InitDefault( GuiObjectPtr _spObject ) const
{
	CombinedCtrlCreator::InitDefault( _spObject );

	ListBoxColumnPtr spColumn = static_pointer_cast< ListBoxColumn >( _spObject );

    // Create header textbox
	TextBoxPtr spHeader = GuiMain::GetInstance().Create< TextBox >( "theHeader" );
	spColumn->AddPieceLast( spHeader );
	spColumn->m_pHeader = spHeader;

    // Init header textbox
    spHeader->SetSizeModeX( SIZE_MODE_STRETCH );
}

void ListBoxColumnCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	CombinedCtrlCreator::InitFromXML( _spObject, _spXMLElement );

	ListBoxColumnPtr spColumn = static_pointer_cast< ListBoxColumn >( _spObject );

	// set header
	TextBoxPtr pHeader = spColumn->GetHeader();
	if ( pHeader != NULL )
	{
        if ( _spXMLElement->HasAttribute( "bg_color" ) )
        {
		    pHeader->SetBgColor( CFoundation::Color( _spXMLElement->GetAttribute( "bg_color" ).HexToInteger() ) );
        }
        if ( _spXMLElement->HasAttribute( "column_header" ) )
        {
		    pHeader->SetText( _spXMLElement->GetAttribute( "column_header" ) );
        }
	}
}
