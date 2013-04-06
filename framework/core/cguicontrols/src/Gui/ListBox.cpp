#include "cgui/Gui/ListBox.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/XML/XMLDocument.h"
#include "cfoundation/XML/XMLElement.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/ListBoxColumn.h"
#include "cgui/Gui/SizeMode.h"
#include "cgui/Gui/TextBox.h"

#include "cgui/Font/FontLoader.h"

#include "cgui/Messages/ScrollMsg.h"

using namespace CGui;

ListBox::ListBox( const CFoundation::String &sName )
:	CombinedCtrl( sName ),
	m_u16NumRows( 0 ),
	m_u16NumMaxRows( MAX_U16 ),
	m_u16CurrentRow( 0 ),
	m_u16CurrentCol( 0 ),
	m_clRowColor1( 0xaaaaaa ),
	m_clRowColor2( 0x777777 ),
	m_clHighlightColor( 0xaaaa22 ),
	m_i32HighlightedRow( -1 ),
	m_u32HeaderHeight( 2 ),
	m_u32RowHeight( 20 ),
	m_u32Offset( 0 ),
	m_u32MinColumnWidth( 20 ),
	m_bScrollableHoriz( false ),
	m_bColumnsResizable( true ),
	m_bHeaderVisible( true )
{
    SetGridSize( 1 );
	m_spHeaderFont = FontLoader::GetInstance().LoadFontFromFile( "common/fonts/arial.ttf", 12 );
	m_spCellFont = FontLoader::GetInstance().LoadFontFromFile( "common/fonts/arial.ttf", 12 );

    // Create delegates
    m_spDelegateFocusedRow = ListBoxColumnEventDelegate2Ptr( new ListBoxColumnEventDelegate2( this, &ListBox::OnFocusedRow ) );
    m_spDelegateClickedRow = ListBoxColumnEventDelegate2Ptr( new ListBoxColumnEventDelegate2( this, &ListBox::OnClickedRow ) );
    m_spDelegateDoubleClickedRow = ListBoxColumnEventDelegate2Ptr( new ListBoxColumnEventDelegate2( this, &ListBox::OnDoubleClickedRow ) );
    m_spDelegateValueChanged = ListBoxColumnEventStrStrDelegate4Ptr( new ListBoxColumnEventStrStrDelegate4( this, &ListBox::OnValueChanged ) );
}

ListBox::~ListBox()
{
}

const CFoundation::String ListBox::GetTypeID()
{
	return "listbox";
}

const CFoundation::String ListBox::GetTypeId() const
{
	return ListBox::GetTypeID();
}
		
void ListBox::Clear()
{
    // Remove columns
    ColumnVec aspColumns = m_aspColumns;
    for ( ColumnVec::iterator it = aspColumns.begin(); it != aspColumns.end(); ++it )
    {
        ListBoxColumnPtr spColumn = *it;
        spColumn->Clear();
        RemoveColumn( spColumn );
    }

    CombinedCtrl::Clear();
}

//void ListBox::ProcessMessage( const CFoundation::Message &clMessage )
//{
//	for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
//	{
//		ListBoxColumnPtr spColumn = GetColumn( u16Col );
//
//        // Check if column size changed
//		//const SizeChangedMsg* pSizeChangedMsg = clMessage.CastTo< SizeChangedMsg >();
//		//if ( pSizeChangedMsg != NULL &&
//  //           pSizeChangedMsg->GetObject() == spColumn )
//		//{
//		//	if ( !IsScrollableHorizontal() )
//		//	{
//		//		SetCustomWidth( GetWholeWidth() );
//		//	}
//		//	if ( u16Col != GetNumColumns() - 1 )
//		//	{
//		//		UpdateColumnsPositions();
//		//	}
//
//  //          return;
//		//}
//
//		const ScrollMsg *pScrollMsg = clMessage.CastTo< ScrollMsg >();
//		if( pScrollMsg != NULL &&
//            pScrollMsg->GetObject() == spColumn )
//		{
//			UpdateColumnsPositions();
//            return;
//		}
//	}
//
//	const ScrollMsg *pScrollMsg = clMessage.CastTo<ScrollMsg>();
//	if( pScrollMsg != NULL &&
//		pScrollMsg->GetOrientation() == ScrollBar::SCROLLBAR_VERTICAL )
//	{
//		for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
//		{
//			GetColumn( u16Col )->SetHeaderOffset( pScrollMsg->GetScrollOffset() );
//		}
//	}
//
//	CombinedCtrl::ProcessMessage( clMessage );
//}

void ListBox::SetSize( const CFoundation::Vector2Du& _clSize )
{
	if ( GetSize() != _clSize )
	{
		CombinedCtrl::SetSize( _clSize );

        bool bHeightChanged = _clSize.GetY() != GetHeight();
		if ( !IsScrollableHorizontal() ||
			 bHeightChanged )
		{
			UpdateColumnsDimensions();
		}
	}
}
/*
void ListBox::SetWidth( Unsigned32 u32Width )
{
	if( u32Width < GetMinWidth() )
	{
		u32Width = GetMinWidth();
	}
	if( u32Width != GetWidth() )
	{
		CombinedCtrl::SetWidth( u32Width );
		UpdateColumnsDimensions();
	}
}

void ListBox::SetHeight( Unsigned32 u32Height )
{
	if( u32Height != GetHeight() )
	{
		CombinedCtrl::SetHeight( u32Height );
		UpdateColumnsDimensions();
	}
}
*/

void ListBox::SetResizableAnchor( const CFoundation::BorderType& eAnchor )
{
	if( eAnchor != GetResizableAnchor() )
	{
		CombinedCtrl::SetResizableAnchor( eAnchor );

		if( eAnchor == CFoundation::BORDER_ALL )
		{
			SetColumnsResizable( true );
		}
		else if( eAnchor == CFoundation::BORDER_NONE )
		{
			SetColumnsResizable( false );
		}
	}
}
        
void ListBox::OnLostFocus()
{
    // Select no row
    SetSelectedRow( -1 );

    CombinedCtrl::OnLostFocus();
}

void ListBox::SetColumnsResizable( bool bResizable )
{
	if( bResizable != AreColumnsResizable() )
	{
		m_bColumnsResizable = bResizable;

		// Make columns resizable or not
		for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
		{
			ListBoxColumnPtr pColumn = GetColumn( u16Col );
			if( bResizable &&
				u16Col != GetNumColumns() - 1 )
			{
				pColumn->SetResizableAnchor( CFoundation::BORDER_RIGHT );
			}
			else
			{
				pColumn->SetResizableAnchor( CFoundation::BORDER_NONE );
			}
		}
	}
}

bool ListBox::AreColumnsResizable() const
{
	return m_bColumnsResizable;
}

void ListBox::SetHeaderVisible( bool bVisible )
{
	if( bVisible != IsHeaderVisible() )
	{
		m_bHeaderVisible = bVisible;

		for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
		{
			ListBoxColumnPtr pColumn = GetColumn( u16Col );
			pColumn->SetHeaderVisible( bVisible );
		}
	}
}

bool ListBox::IsHeaderVisible() const
{
	return m_bHeaderVisible;
}

void ListBox::UpdateColumnsDimensions()
{
    Integer32 i32ColumnHeight = std::max( static_cast< Integer32 >( GetHeight() ) - 2 * m_u8GridSizes[ GRID_OUTER ], 0 );
	for( Unsigned16 u16Col = 0; u16Col < m_aspColumns.size(); ++u16Col )
	{
		ListBoxColumnPtr pColumn = m_aspColumns[ u16Col ];
		pColumn->SetCustomHeight( i32ColumnHeight );
	}

	// resize last column if there is free space
	// and not scrollable horizontal
	if( !IsScrollableHorizontal() &&
		m_aspColumns.size() > 0 )
	{
		Integer16 i16Col = GetNumColumns() - 1;
		while( GetWidth() != GetWholeWidth() &&
			   i16Col >= 0 )
		{
			ListBoxColumnPtr pColumn = GetColumn( i16Col );
            Integer32 i32RemainingWidth = GetWidth() - GetWholeWidth();
			Unsigned32 u32NewWidth = std::max( i32RemainingWidth +  pColumn->GetWidth(), GetMinColumnWidth() );
			pColumn->SetCustomWidth( u32NewWidth );
			--i16Col;
		}

		// if not only last column was changed column
		// positions need to be changed
		if( i16Col < GetNumColumns() - 2 )
		{
			UpdateColumnsPositions();
		}
	}
}

void ListBox::UpdateColumnsPositions()
{
	Integer32 i32OffsetX = m_u8GridSizes[ GRID_OUTER ];
	for( Unsigned16 u16Col = 0; u16Col < m_aspColumns.size(); ++u16Col )
	{
		ListBoxColumnPtr pColumn = m_aspColumns[ u16Col ];
        pColumn->SetOffset( CFoundation::Vector2Di( i32OffsetX, m_u8GridSizes[ GRID_OUTER ] ) );
		i32OffsetX += pColumn->GetWidth() + m_u8GridSizes[ GRID_INNER_VERT ];
	}
}

void ListBox::ClearContent()
{
	for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
	{
		GetColumn( u16Col )->ClearContent();
	}
	m_u16NumRows = 0;
	m_u16CurrentRow = 0;
	m_u16CurrentCol = 0;
    m_i32HighlightedRow = -1;
}

void ListBox::SetNumMaxRows( Unsigned16 u16NumMaxRows )
{
	m_u16NumMaxRows = u16NumMaxRows;
}

Unsigned16 ListBox::GetNumMaxRows() const
{
	return m_u16NumMaxRows;
}

Unsigned16 ListBox::GetNumRows() const
{
	return m_u16NumRows;
}

Unsigned16 ListBox::GetNumColumns() const
{
	return static_cast<Unsigned16>( m_aspColumns.size() );
}

ListBoxColumnPtr ListBox::GetColumn( Unsigned32 _u32Col )
{
	if ( _u32Col < m_aspColumns.size() )
	{
		return m_aspColumns[ _u32Col ];
	}
	else
	{
		return ListBoxColumnPtr();
	}
}

ListBoxColumnCPtr ListBox::GetColumn( Unsigned32 _u32Col ) const
{
	if ( _u32Col < m_aspColumns.size() )
	{
		return m_aspColumns[ _u32Col ];
	}
	else
	{
		return ListBoxColumnCPtr();
	}
}
        
Integer32 ListBox::GetColumnIndex( GuiObjectPtr _spColumn ) const
{
    // Find index
    ColumnVec::const_iterator it = std::find( m_aspColumns.begin(), m_aspColumns.end(), _spColumn );
    if ( it != m_aspColumns.end() )
    {
        return it - m_aspColumns.begin();
    }
    else
    {
        return -1;
    }
}

void ListBox::SetCellBgColor( Unsigned16 u16Row, Unsigned16 u16Col, const CFoundation::Color &clColor )
{
	if( u16Col < GetNumColumns() )
	{
		GetColumn( u16Col )->SetCellBgColor( u16Row, clColor );
	}
}

void ListBox::SetRowBgColor( Unsigned16 u16Row, const CFoundation::Color &clColor )
{
	for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
	{
		SetCellBgColor( u16Row, u16Col, clColor );
	}
}

void ListBox::SetCellValue( Unsigned32 _u32Row, Unsigned32 _u32Col, const CFoundation::String& sData )
{
	if ( _u32Col < m_aspColumns.size() )
	{
		m_aspColumns[ _u32Col ]->SetCellValue( _u32Row, sData );
	}
}

const CFoundation::String& ListBox::GetCellValue( Unsigned32 _u32Row, Unsigned32 _u32Col ) const
{
	if ( _u32Col < m_aspColumns.size() )
	{
		return m_aspColumns[ _u32Col ]->GetCellValue( _u32Row );
	}
	else
	{
	    static CFoundation::String sTmp( "" );
	    return sTmp;
	}
}

void ListBox::SetCellData( Unsigned32 _u32Row, Unsigned32 _u32Column, Integer64 _i64Data )
{
	if ( _u32Column < m_aspColumns.size() )
	{
		m_aspColumns[ _u32Column ]->SetCellData( _u32Row, _i64Data );
	}
}

Integer64 ListBox::GetCellData( Unsigned32 _u32Row, Unsigned32 _u32Column ) const
{
	if ( _u32Column < m_aspColumns.size() )
	{
		return m_aspColumns[ _u32Column ]->GetCellData( _u32Row );
	}
	else
	{
	    return 0;
	}
}

void ListBox::SetCellType( Unsigned32 _u32Row, Unsigned32 _u32Column, const CellType& _eType )
{
	if ( _u32Column < GetNumColumns() )
	{
		GetColumn( _u32Column )->SetCellType( _u32Row, _eType );
	}
}

void ListBox::SetSelectedRow( Integer32 _i32Row )
{
    if ( _i32Row == m_i32HighlightedRow )
    {
        return;
    }

	m_i32HighlightedRow = _i32Row;

	for( Unsigned32 u32Col = 0; u32Col < GetNumColumns(); ++u32Col )
	{
		GetColumn( u32Col )->SetHighlightedRow( m_i32HighlightedRow );
	}

    // Dispatch event
    DispatchEvent( EVENT_LISTBOX_SELECTED_ROW, GetSharedPtr< ListBox >(), m_i32HighlightedRow );
}

Integer32 ListBox::GetSelectedRow() const
{
    return m_i32HighlightedRow;
}

void ListBox::AddColumnData( const char *szCellValue, Integer64 i64CellData, const CellType &eType )
{
	return AddColumnData( CFoundation::String( szCellValue ), i64CellData, eType );
}

void ListBox::AddColumnData( const CFoundation::String &sCellValue, Integer64 i64CellData, const CellType &eType )
{
	CFoundation::Color clBgColor;
	if( m_u16CurrentRow % 2 == 0 )
	{
		clBgColor = m_clRowColor1;
	}
	else
	{
		clBgColor = m_clRowColor2;
	}

	if( m_u16CurrentCol < GetNumColumns() )
	{
		ListBoxColumnPtr pColumn = GetColumn( m_u16CurrentCol );
		pColumn->SetCellType( m_u16CurrentRow, eType );
		pColumn->SetCellValue( m_u16CurrentRow, sCellValue );
		pColumn->SetCellData( m_u16CurrentRow, i64CellData );
		pColumn->SetCellBgColor( m_u16CurrentRow, clBgColor );
	}

	++m_u16CurrentCol;
}

void ListBox::AddColumnData( Unsigned32 u32CellValue, Integer64 i64CellData, const CellType &eType )
{
	return AddColumnData( CFoundation::String( u32CellValue ), i64CellData, eType );
}

void ListBox::AddColumnData( Integer32 i32CellValue, Integer64 i64CellData, const CellType &eType )
{
	return AddColumnData( CFoundation::String( i32CellValue ), i64CellData, eType );
}

void ListBox::AddColumnData( Integer64 i64CellValue, Integer64 i64CellData, const CellType &eType )
{
	return AddColumnData( CFoundation::String( i64CellValue ), i64CellData, eType );
}

void ListBox::AddColumnData( Float32 f32CellValue, Integer64 i64CellData, const CellType &eType )
{
	return AddColumnData( CFoundation::String( f32CellValue ), i64CellData, eType );
}

void ListBox::AddColumnData( bool bCellValue, Integer64 i64CellData, const CellType &eType )
{
	if( bCellValue )
	{
		return AddColumnData( CFoundation::String( "1" ), i64CellData, eType );
	}
	else
	{
		return AddColumnData( CFoundation::String( "0" ), i64CellData, eType );
	}
}
        
void ListBox::AddColumnData( const std::vector< CFoundation::String >& _astrCellValues, const std::vector< Integer64 >& _ai64CellData )
{
    CFoundation::Color clBgColor = m_u16CurrentRow % 2 == 0 ? m_clRowColor1 : m_clRowColor2;

    if ( m_u16CurrentCol < m_aspColumns.size() )
	{
		ListBoxColumnPtr pColumn = m_aspColumns[ m_u16CurrentCol ];
		pColumn->SetCellType( m_u16CurrentRow, TYPE_COMBOBOX );
		pColumn->SetCellValues( m_u16CurrentRow, _astrCellValues );
		pColumn->SetCellData( m_u16CurrentRow, _ai64CellData );
		pColumn->SetCellBgColor( m_u16CurrentRow, clBgColor );
	}

	++m_u16CurrentCol;
}

void ListBox::AddRow()
{
	m_u16CurrentCol = 0;
	++m_u16CurrentRow;
	++m_u16NumRows;
}
        
void ListBox::SetGridSize( Unsigned8 u8GridSize )
{
    for( Unsigned32 u32GridType = 0; u32GridType < NUM_GRID_TYPES; ++u32GridType )
    {
        m_u8GridSizes[ u32GridType ] = u8GridSize;
    }

	for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
	{
		GetColumn( u16Col )->SetGridSize( u8GridSize );
	}

	UpdateColumnsPositions();
}

void ListBox::SetGridSize( ListBox::EGridType eGrid, Unsigned8 u8GridSize )
{
    m_u8GridSizes[ eGrid ] = u8GridSize;

    if( eGrid == GRID_INNER_HORIZ )
    {
        for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
        {
            GetColumn( u16Col )->SetGridSize( u8GridSize );
        }
    }

	UpdateColumnsPositions();
}

void ListBox::SetGridColor( const CFoundation::Color& clGridColor )
{
	SetBgColor( STATE_NORMAL, clGridColor );
}

void ListBox::SetHeaderHeight( Unsigned32 u32HeaderHeight )
{
	m_u32HeaderHeight = u32HeaderHeight;
	for( ColumnVec::iterator it = m_aspColumns.begin(); it != m_aspColumns.end(); ++it )
	{
	    ListBoxColumnPtr pColumn = *it;
		pColumn->SetHeaderHeight( m_u32HeaderHeight );
	}
}

void ListBox::SetHeaderFont( FontCPtr _spFont )
{
	m_spHeaderFont = _spFont;
	for( ColumnVec::iterator it = m_aspColumns.begin(); it != m_aspColumns.end(); ++it )
	{
	    ListBoxColumnPtr pColumn = *it;
		pColumn->SetHeaderFont( _spFont );
	}
}

void ListBox::SetRowHeight( Unsigned32 u32RowHeight )
{
	m_u32RowHeight = u32RowHeight;
	for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
	{
		GetColumn( u16Col )->SetRowHeight( m_u32RowHeight );
	}
}

void ListBox::SetCellFont( FontCPtr _spFont )
{
	m_spCellFont = _spFont;
	for( Unsigned32 u32Col = 0; u32Col < m_aspColumns.size(); ++u32Col )
	{
		m_aspColumns[ u32Col ]->SetCellFont( _spFont );
	}
}

void ListBox::SetRowColor1( const CFoundation::Color &clColor )
{
	m_clRowColor1 = clColor;
	for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
	{
		GetColumn( u16Col )->SetRowColor1( clColor );
	}
}

void ListBox::SetRowColor2( const CFoundation::Color &clColor )
{
	m_clRowColor2 = clColor;
	for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
	{
		GetColumn( u16Col )->SetRowColor2( m_clRowColor2 );
	}
}

void ListBox::SetTextColor1( const CFoundation::Color &clColor )
{
	if( clColor != m_clTextColor1 )
	{
		m_clTextColor1 = clColor;
		for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
		{
			GetColumn( u16Col )->SetTextColor1( m_clTextColor1 );
		}
	}
}

void ListBox::SetTextColor2( const CFoundation::Color &clColor )
{
	if( clColor != m_clTextColor2 )
	{
		m_clTextColor1 = clColor;
		for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
		{
			GetColumn( u16Col )->SetTextColor2( m_clTextColor2 );
		}
	}
}

void ListBox::SetHighlightColor( const CFoundation::Color &clColor )
{
	if( clColor != m_clHighlightColor )
	{
		m_clHighlightColor = clColor;
		for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
		{
			GetColumn( u16Col )->SetHighlightColor( m_clHighlightColor );
		}
	}
}

void ListBox::SetHighlightTextColor( const CFoundation::Color &clColor )
{
	if( clColor != m_clHighlightTextColor )
	{
		m_clHighlightTextColor = clColor;
		for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
		{
			GetColumn( u16Col )->SetHighlightTextColor( m_clHighlightTextColor );
		}
	}
}

const CFoundation::Color& ListBox::GetRowColor1() const
{
	return m_clRowColor1;
}

const CFoundation::Color& ListBox::GetTextColor1() const
{
	return m_clTextColor1;
}

const CFoundation::Color& ListBox::GetRowColor2() const
{
	return m_clRowColor2;
}

const CFoundation::Color& ListBox::GetTextColor2() const
{
	return m_clTextColor2;
}

const CFoundation::Color& ListBox::GetHighlightColor() const
{
	return m_clHighlightColor;
}

const CFoundation::Color& ListBox::GetHighlightTextColor() const
{
	return m_clHighlightTextColor;
}

void ListBox::AddColumn( ListBoxColumnPtr _spColumn )
{
	// Listen to row selections
    _spColumn->RegisterDelegate( EVENT_LISTBOXCOLUMN_FOCUSED_ROW, m_spDelegateFocusedRow );
    _spColumn->RegisterDelegate( EVENT_LISTBOXCOLUMN_CLICKED_ROW, m_spDelegateClickedRow );
    _spColumn->RegisterDelegate( EVENT_LISTBOXCOLUMN_DOUBLE_CLICKED_ROW, m_spDelegateDoubleClickedRow );
    _spColumn->RegisterDelegate( EVENT_LISTBOXCOLUMN_VALUE_CHANGED, m_spDelegateValueChanged );

	// set settings
    _spColumn->SetSizeModeY( SIZE_MODE_CONTENT );
	_spColumn->SetMinWidth( GetMinColumnWidth() );

	_spColumn->SetHeaderHeight( GetHeaderHeight() );

    _spColumn->SetHeaderFont( m_spHeaderFont );

	_spColumn->SetRowHeight( GetRowHeight() );

	_spColumn->SetCellFont( m_spCellFont );

	_spColumn->SetRowColor1( GetRowColor1() );
	_spColumn->SetRowColor2( GetRowColor2() );
	_spColumn->SetTextColor1( GetTextColor1() );
	_spColumn->SetTextColor2( GetTextColor2() );
	_spColumn->SetHighlightColor( GetHighlightColor() );
	_spColumn->SetHighlightTextColor( GetHighlightTextColor() );

    _spColumn->SetGridColor( GetGridColor() );
	_spColumn->SetGridSize( GetGridSize( GRID_INNER_HORIZ ) );

	if( AreColumnsResizable() &&
		GetNumColumns() > 0 )
	{
		GetColumn( GetNumColumns() - 1 )->SetResizableAnchor( CFoundation::BORDER_RIGHT );
	}

	_spColumn->SetHeaderVisible( IsHeaderVisible() );

	AddPieceLast( _spColumn );
	m_aspColumns.push_back( _spColumn );

	// Set new dimensions
	/*if( !IsLayoutSuspended() &&
         GetWidth() < GetMinWidth() )
	{
		SetCustomWidth( GetMinWidth() );
	}*/
	if( GetNumColumns() > 1 )
	{
		UpdateColumnsPositions();
	}
	else
	{
        _spColumn->SetOffset( CFoundation::Vector2Di( GetGridSize( GRID_OUTER ), GetGridSize( GRID_OUTER ) ) );
	}
}

void ListBox::RemoveColumn( ListBoxColumnPtr _spColumn )
{
    ColumnVec::iterator it = std::find( m_aspColumns.begin(), m_aspColumns.end(), _spColumn );
    if ( it == m_aspColumns.end() )
    {
        CF_WARN( "ListBox::RemoveColumn: Tried to remove column that's no one of this listbox." );
        return;
    }

	// Remove delegates
    _spColumn->RemoveDelegate( EVENT_LISTBOXCOLUMN_FOCUSED_ROW, m_spDelegateFocusedRow );
    _spColumn->RemoveDelegate( EVENT_LISTBOXCOLUMN_CLICKED_ROW, m_spDelegateClickedRow );
    _spColumn->RemoveDelegate( EVENT_LISTBOXCOLUMN_DOUBLE_CLICKED_ROW, m_spDelegateDoubleClickedRow );
    _spColumn->RemoveDelegate( EVENT_LISTBOXCOLUMN_VALUE_CHANGED, m_spDelegateValueChanged );

    // Remove from columns
	m_aspColumns.erase( it );

    // Remove as piece
	RemovePiece( _spColumn );
}

void ListBox::SetCellValue( Unsigned16 u16Row, Unsigned16 u16Col, const CFoundation::String &sData, const CFoundation::Color &clBgColor )
{
	if( u16Col < m_aspColumns.size() )
	{
		m_aspColumns[ u16Col ]->SetCellValue( u16Row, sData );
		m_aspColumns[ u16Col ]->SetCellBgColor( u16Row, clBgColor );
	}
}

void ListBox::UpdateCell( Unsigned16 /*u16Row*/, Unsigned16 u16Col )
{
	if( u16Col < m_aspColumns.size() )
	{
		//m_aspColumns[ u16Col ]->UpdateCell( u16Row );
	}
}

void ListBox::UpdateRow( Unsigned16 u16Row )
{
	Unsigned16 u16MaxCols = GetNumColumns();
	for( Unsigned16 u16Col = 0; u16Col < u16MaxCols; ++u16Col )
	{
		UpdateCell( u16Row, u16Col );
	}
}

Unsigned32 ListBox::GetHeaderHeight() const
{
	return m_u32HeaderHeight;
}

Unsigned8 ListBox::GetGridSize( ListBox::EGridType eGrid ) const
{
	return m_u8GridSizes[ eGrid ];
}

const CFoundation::Color& ListBox::GetGridColor() const
{
	return GetBgColor( STATE_NORMAL );
}

Unsigned32 ListBox::GetRowHeight() const
{
	return m_u32RowHeight;
}

void ListBox::SetMinColumnWidth( Unsigned32 u32MinWidth )
{
	if( m_u32MinColumnWidth != u32MinWidth )
	{
		m_u32MinColumnWidth = u32MinWidth;
		if( GetMinWidth() > GetWidth() )
		{
			SetCustomWidth( GetMinWidth() );
		}
		for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
		{
			ListBoxColumnPtr pColumn = GetColumn( u16Col );
			pColumn->SetMinWidth( u32MinWidth );
		}
	}
}

Unsigned32 ListBox::GetMinColumnWidth() const
{
	return m_u32MinColumnWidth;
}

void ListBox::SetScrollableHorizontal( bool bHorizScrollable )
{
	m_bScrollableHoriz = bHorizScrollable;
}

bool ListBox::IsScrollableHorizontal() const
{
	return m_bScrollableHoriz;
}

Unsigned32 ListBox::GetMinWidth() const
{
	return GetNumColumns() * ( GetMinColumnWidth() + GetGridSize( GRID_INNER_VERT ) ) + 2 * GetGridSize( GRID_OUTER );
}

Unsigned32	ListBox::GetWholeWidth() const
{
	Unsigned32 u32WholeWidth = 2 * GetGridSize( GRID_OUTER );
	for( Unsigned16 u16Col = 0; u16Col < GetNumColumns(); ++u16Col )
	{
		ListBoxColumnCPtr pColumn = GetColumn( u16Col );
		u32WholeWidth += pColumn->GetWidth() + GetGridSize( GRID_INNER_VERT );
	}
	u32WholeWidth -= GetGridSize( GRID_INNER_VERT );
	return u32WholeWidth;
}

Unsigned32  ListBox::GetWholeHeight() const
{
	Unsigned32 u32WholeHeight = GetHeaderHeight() + 2 * GetGridSize( GRID_OUTER );
	u32WholeHeight += GetNumRows() * ( GetRowHeight() + GetGridSize( GRID_INNER_HORIZ ) );
	if( GetHeight() > u32WholeHeight )
	{
		return GetHeight();
	}
	else
	{
		return u32WholeHeight;
	}
}
        
void ListBox::OnFocusedRow( GuiObjectPtr _spColumn, Unsigned32 _u32SelectedRow )
{
	if ( _u32SelectedRow >= GetNumRows() )
	{
        return;
    }

    // Search column index
    Integer32 i32Column = GetColumnIndex( _spColumn );
    if ( i32Column < 0 )
    {
        return;
    }

	SetSelectedRow( _u32SelectedRow );

    // Dispatch event
    DispatchEvent( EVENT_LISTBOX_SELECTED_CELL, _u32SelectedRow, i32Column );
}
        
void ListBox::OnClickedRow( GuiObjectPtr _spColumn, Unsigned32 _u32SelectedRow )
{
	if ( _u32SelectedRow >= GetNumRows() )
	{
        return;
    }

    // Search column index
    Integer32 i32Column = GetColumnIndex( _spColumn );
    if ( i32Column < 0 )
    {
        return;
    }

    // Dispatch event
    DispatchEvent( EVENT_LISTBOX_CLICKED_CELL, _u32SelectedRow, i32Column );
}
        
void ListBox::OnDoubleClickedRow( GuiObjectPtr _spColumn, Unsigned32 _u32SelectedRow )
{
	if ( _u32SelectedRow >= GetNumRows() )
	{
        return;
    }

    // Search column index
    Integer32 i32Column = GetColumnIndex( _spColumn );
    if ( i32Column < 0 )
    {
        return;
    }

    // Dispatch event
    DispatchEvent( EVENT_LISTBOX_DOUBLE_CLICKED_CELL, _u32SelectedRow, i32Column );
}
        
void ListBox::OnValueChanged( ListBoxColumnPtr _spColumn, Unsigned32 _u32SelectedRow, const CFoundation::String& _strNewValue, const CFoundation::String& _strOldValue )
{
    // Search column index
    Integer32 i32Column = GetColumnIndex( _spColumn );
    if ( i32Column < 0 )
    {
        return;
    }

    // Dispatch event
    DispatchEvent( EVENT_LISTBOX_VALUE_CHANGED, GetSharedPtr< ListBox >(), _u32SelectedRow, i32Column, _strNewValue );
}

const CFoundation::String ListBoxCreator::GetTypeId() const
{
	return ListBox::GetTypeID();
}

GuiObjectPtr ListBoxCreator::CreateObject( const CFoundation::String &sName ) const
{
	ListBoxPtr spListBox( new ListBox( sName ) );
	return spListBox;
}

void ListBoxCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	CombinedCtrlCreator::InitFromXML( _spObject, _spXMLElement );

	ListBoxPtr spListBox = static_pointer_cast< ListBox >( _spObject );

	// set listbox attributes
	CFoundation::String sTmp;

	// Get header font and font size
    if ( _spXMLElement->HasAttribute( "header_font" ) &&
         _spXMLElement->HasAttribute( "header_font_size" ) )
    {
	    CFoundation::String sFontName = _spXMLElement->GetAttribute( "header_font" );
	    Unsigned8 u8FontSize = _spXMLElement->GetAttribute( "header_font_size" ).ToUnsigned8();
	    FontPtr spFont = FontLoader::GetInstance().LoadFontFromFile( sFontName, u8FontSize );
	    if ( spFont != NULL )
	    {
	        spListBox->SetHeaderFont( spFont );
	    }
    }

	// Get cell font and font size
    if ( _spXMLElement->HasAttribute( "cell_font" ) &&
         _spXMLElement->HasAttribute( "cell_font_size" ) )
    {
	    CFoundation::String sFontName = _spXMLElement->GetAttribute( "cell_font" );
	    Unsigned8 u8FontSize = _spXMLElement->GetAttribute( "cell_font_size" ).ToUnsigned8();
	    FontPtr spFont = FontLoader::GetInstance().LoadFontFromFile( sFontName, u8FontSize );
        if ( spFont != NULL )
	    {
	        spListBox->SetCellFont( spFont );
	    }
    }

    if ( _spXMLElement->HasAttribute( "row_color1" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "row_color1" );
	    spListBox->SetRowColor1( CFoundation::Color( sTmp ) );
    }
    if ( _spXMLElement->HasAttribute( "row_color2" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "row_color2" );
	    spListBox->SetRowColor2( CFoundation::Color( sTmp ) );
    }
    if ( _spXMLElement->HasAttribute( "highlight_color" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "highlight_color" );
	    spListBox->SetHighlightColor( CFoundation::Color( sTmp ) );
    }
    if ( _spXMLElement->HasAttribute( "grid_color" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "grid_color" );
	    spListBox->SetGridColor( CFoundation::Color( sTmp ) );
    }

    if ( _spXMLElement->HasAttribute( "grid_inner_horiz_size" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "grid_inner_horiz_size" );
	    spListBox->SetGridSize( ListBox::GRID_INNER_HORIZ, sTmp.ToUnsigned8() );
    }
    if ( _spXMLElement->HasAttribute( "grid_inner_vert_size" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "grid_inner_vert_size" );
	    spListBox->SetGridSize( ListBox::GRID_INNER_VERT, sTmp.ToUnsigned8() );
    }
    if ( _spXMLElement->HasAttribute( "grid_outer_size" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "grid_outer_size" );
	    spListBox->SetGridSize( ListBox::GRID_OUTER, sTmp.ToUnsigned8() );
    }
    if ( _spXMLElement->HasAttribute( "header_visible" ) )
    {
        spListBox->SetHeaderVisible( _spXMLElement->GetAttributeAsBool( "header_visible" ) );
    }
    if ( _spXMLElement->HasAttribute( "header_height" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "header_height" );
        spListBox->SetHeaderHeight( sTmp.ToUnsigned32() );
    }
    if ( _spXMLElement->HasAttribute( "row_height" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "row_height" );
	    spListBox->SetRowHeight( sTmp.ToUnsigned32() );
    }

	// add columns
	CFoundation::XMLNodePtr pChild = _spXMLElement->GetFirstChild();
	while ( pChild != NULL )
	{
		if ( pChild->IsElement() )
		{
            CFoundation::XMLElementCPtr spElement = static_pointer_cast< CFoundation::XMLElement >( pChild );

            const CFoundation::String& strName = spElement->GetName();

            if ( strName == "column" )
            {
                // add new column to listbox
                ListBoxColumnPtr spColumn = GuiMain::GetInstance().CreateFromXML< ListBoxColumn >( spElement  );
                spListBox->AddColumn( spColumn );
            }
        }
			
        pChild = pChild->GetNextSibling();
	}

    spListBox->UpdateColumnsDimensions();
}

/*
ListBox& ListBoxCreator::AssignStyle( GuiObject& clObject, const CGui::CtrlStyle& clStyle ) const
{
	ListBox &clListBox = static_cast<ListBox&>( clObject );

	CFoundation::XMLElementConstPtr pRoot = clStyle.GetRoot();
    if ( pRoot == NULL )
    {
        return clListBox;
    }

	// set attributes

	CFoundation::String sHeaderHeight = pRoot->GetAttribute( "header_height" );
	spListBox->SetHeaderHeight( sHeaderHeight.ToUnsigned32() );

	CFoundation::String sRowColor1 = pRoot->GetAttribute( "row_color1" );
	spListBox->SetRowColor1( CFoundation::Color( sRowColor1.HexToInteger() ) );
	CFoundation::String sRowColor2 = pRoot->GetAttribute( "row_color2" );
	spListBox->SetRowColor2( CFoundation::Color( sRowColor2.HexToInteger() ) );
	CFoundation::String sTextColor1 = pRoot->GetAttribute( "text_color1" );
	spListBox->SetTextColor1( CFoundation::Color( sTextColor1.HexToInteger() ) );
	CFoundation::String sTextColor2 = pRoot->GetAttribute( "text_color2" );
	spListBox->SetTextColor2( CFoundation::Color( sTextColor2.HexToInteger() ) );

	CFoundation::String sHighlightColor = pRoot->GetAttribute( "highlight_color" );
	spListBox->SetHighlightColor( CFoundation::Color( sHighlightColor.HexToInteger() ) );
	CFoundation::String sHighlightTextColor = pRoot->GetAttribute( "highlight_text_color" );
	spListBox->SetHighlightTextColor( CFoundation::Color( sHighlightTextColor.HexToInteger() ) );

	CFoundation::String sGridColor = pRoot->GetAttribute( "grid_color" );
	spListBox->SetGridColor( CFoundation::Color( sGridColor.HexToInteger() ) );

	CFoundation::String sGridSize = pRoot->GetAttribute( "grid_inner_horiz_size" );
	spListBox->SetGridSize( ListBox::GRID_INNER_HORIZ, sGridSize.ToUnsigned8() );
	sGridSize = pRoot->GetAttribute( "grid_inner_vert_size" );
	spListBox->SetGridSize( ListBox::GRID_INNER_VERT, sGridSize.ToUnsigned8() );
	sGridSize = pRoot->GetAttribute( "grid_outer_size" );
	spListBox->SetGridSize( ListBox::GRID_OUTER, sGridSize.ToUnsigned8() );
}
*/

bool ListBoxCreator::SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const
{
    GuiObjectCreator::SaveToXML( _spObject, _spDoc, _spXMLElement );

    ListBoxCPtr spListBox = static_pointer_cast< const ListBox >( _spObject );

    // Save columns
    Unsigned32 u32NumColumns = spListBox->GetNumColumns();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumColumns; ++u32Idx )
    {
        ListBoxColumnCPtr spColumn = spListBox->GetColumn( u32Idx );

		// Create new XML node
		CFoundation::XMLElementPtr spChildXMLElement = _spDoc->CreateElement( spColumn->GetTypeId() );

		// Add as child
		_spXMLElement->InsertChildLast( spChildXMLElement );

        // Set name
        spChildXMLElement->SetAttribute( "name", spColumn->GetName() );
	}

    return true;
}
