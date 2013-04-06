#ifndef CLIB_LISTBOX_H
#define CLIB_LISTBOX_H

#include <vector>

#include "cgui/Gui/CellType.h"
#include "cgui/Gui/CombinedCtrl.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( Font );
    INCLUDE_SHARED_CLASS( ListBox );
    INCLUDE_SHARED_CLASS( ListBoxColumn );
    INCLUDE_SHARED_CLASS( TextBox );
    
    TYPEDEF_DELEGATE2( ListBoxColumnEvent, GuiObjectPtr, Unsigned32 );
    TYPEDEF_DELEGATE4( ListBoxColumnEventStrStr, ListBoxColumnPtr, Unsigned32, const CFoundation::String&, const CFoundation::String& );

    enum ListBoxEvent
    {
        EVENT_LISTBOX_SELECTED_CELL,
        EVENT_LISTBOX_CLICKED_CELL,
        EVENT_LISTBOX_DOUBLE_CLICKED_CELL
    };
    TYPEDEF_DELEGATE2( ListBoxEvent, Unsigned32, Unsigned32 );

    enum ListBoxEventI32
    {
        EVENT_LISTBOX_SELECTED_ROW,
    };
    TYPEDEF_DELEGATE2( ListBoxEventI32, ListBoxPtr, Integer32 );

    enum ListBoxEventU32U32Str
    {
        EVENT_LISTBOX_VALUE_CHANGED,
    };
    TYPEDEF_DELEGATE4( ListBoxEventU32U32Str, ListBoxPtr, Unsigned32, Unsigned32, const CFoundation::String& );

    class ListBox : public CombinedCtrl, public CFoundation::Delegate2Invoker< ListBoxEvent, Unsigned32, Unsigned32 >,
                                         public CFoundation::Delegate2Invoker< ListBoxEventI32, ListBoxPtr, Integer32 >,
                                         public CFoundation::Delegate4Invoker< ListBoxEventU32U32Str, ListBoxPtr, Unsigned32, Unsigned32, const CFoundation::String& >
	{
	public:
        DELEGATE2_INVOKER( ListBoxEvent, Unsigned32, Unsigned32 );
        DELEGATE2_INVOKER( ListBoxEventI32, ListBoxPtr, Integer32 );
        DELEGATE4_INVOKER( ListBoxEventU32U32Str, ListBoxPtr, Unsigned32, Unsigned32, const CFoundation::String& );

        /// Destructor
		virtual ~ListBox();

        /// Clears the object
		virtual void        Clear();

        /// Clears the content of this listbox
		virtual void        ClearContent();

		virtual void SetColumnsResizable( bool bResizable );
		virtual bool AreColumnsResizable() const;

		virtual void SetHeaderVisible( bool bVisible );
		virtual bool IsHeaderVisible() const;

		/* GuiObject functions */
		static const CFoundation::String    GetTypeID();
		virtual const CFoundation::String   GetTypeId() const;

        //! Changes the size of the object
        virtual void                    SetSize( const CFoundation::Vector2Du& _clSize );

        /*
		virtual void SetDimensions( Unsigned32 u32Width, Unsigned32 u32Height );
		virtual void SetWidth( Unsigned32 u32Width );
		virtual void SetHeight( Unsigned32 u32Height );
*/

		virtual void            SetResizableAnchor( const CFoundation::BorderType& eAnchor );

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Input
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// Called when the object lost the focus
        virtual void            OnLostFocus();

        /// Sets the selected row
		void                    SetSelectedRow( Integer32 _i32Row );

        /// Returns the current selected row
		Integer32               GetSelectedRow() const;

        /// Adds the passed column to this listbox
		void                    AddColumn( ListBoxColumnPtr _spColumn );

        /// Removes the passed column to this listbox
        void                    RemoveColumn( ListBoxColumnPtr _spColumn );

		Unsigned16 GetNumMaxRows() const;
		Unsigned16 GetNumRows() const;
		Unsigned16 GetNumColumns() const;

		void AddColumnData( const char *szCellValue, Integer64 i64CellData = 0, const CellType &eType = TYPE_DEFAULT );
		void AddColumnData( const CFoundation::String &sCellValue, Integer64 i64CellData = 0, const CellType &eType = TYPE_DEFAULT );
		void AddColumnData( Unsigned32 u32CellValue, Integer64 i64CellData = 0, const CellType &eType = TYPE_DEFAULT );
		void AddColumnData( Integer32 i32CellValue, Integer64 i64CellData = 0, const CellType &eType = TYPE_DEFAULT );
		void AddColumnData( Integer64 i64CellValue, Integer64 i64CellData = 0, const CellType &eType = TYPE_DEFAULT );
		void AddColumnData( Float32 f32CellValue, Integer64 i64CellData = 0, const CellType &eType = TYPE_DEFAULT );
		void AddColumnData( bool bCellValue, Integer64 i64CellData = 0, const CellType &eType = TYPE_DEFAULT );

        void AddColumnData( const std::vector< CFoundation::String >& _astrCellValues, const std::vector< Integer64 >& _ai64CellData );

		void AddRow();

		// Settings
		void                    SetHeaderHeight( Unsigned32 u32RowHeight );
		Unsigned32              GetHeaderHeight() const;

		void                    SetHeaderFont( FontCPtr _spFont );

		enum EGridType
		{
		    GRID_OUTER,
		    GRID_INNER_HORIZ,
		    GRID_INNER_VERT,
		    NUM_GRID_TYPES
		};

		void                    SetGridSize( Unsigned8 u8GridSize );
		void                    SetGridSize( EGridType eType, Unsigned8 u8GridSize );
		Unsigned8               GetGridSize( EGridType eType ) const;

		void                    SetGridColor( const CFoundation::Color& clColor );
		const CFoundation::Color&   GetGridColor() const;

		void                    SetRowHeight( Unsigned32 u32RowHeight );
		Unsigned32              GetRowHeight() const;

		void                    SetCellFont( FontCPtr _spFont );

		void SetRowColor1( const CFoundation::Color &clColor );
		void SetTextColor1( const CFoundation::Color &clColor );
		void SetRowColor2( const CFoundation::Color &clColor );
		void SetTextColor2( const CFoundation::Color &clColor );
		void SetHighlightColor( const CFoundation::Color &clColor );
		void SetHighlightTextColor( const CFoundation::Color &clColor );

		const CFoundation::Color& GetRowColor1() const;
		const CFoundation::Color& GetTextColor1() const;
		const CFoundation::Color& GetRowColor2() const;
		const CFoundation::Color& GetTextColor2() const;
		const CFoundation::Color& GetHighlightColor() const;
		const CFoundation::Color& GetHighlightTextColor() const;

		void SetNumMaxRows( Unsigned16 u16NumMaxRows );

		void SetCellBgColor( Unsigned16 u16Row, Unsigned16 u16Col, const CFoundation::Color &clColor );
		void SetRowBgColor( Unsigned16 u16Row, const CFoundation::Color &clColor );

        /// Sets the cell value of the passed cell
		void                        SetCellValue( Unsigned32 _u32Row, Unsigned32 _u32Column, const CFoundation::String& _strValue );

        /// Returns the cell value of the passed cell
        const CFoundation::String&  GetCellValue( Unsigned32 _u32Row, Unsigned32 _u32Column ) const;

        /// Sets the cell data of the passed cell
		void                        SetCellData( Unsigned32 _u32Row, Unsigned32 _u32Column, Integer64 i64Data );

        /// Returns the cell data of the passed cell
        Integer64                   GetCellData( Unsigned32 _u32Row, Unsigned32 _u32Column ) const;

        /// Sets the cell type of the passed cell
		void                        SetCellType( Unsigned32 _u32Row, Unsigned32 _u32Column, const CellType& _eType );

		void SetMinColumnWidth( Unsigned32 u32MinWidth );
		Unsigned32 GetMinColumnWidth() const;

		void SetScrollableHorizontal( bool bHorizScrollable );
		bool IsScrollableHorizontal() const;

	protected:
		virtual void UpdateColumnsDimensions();
		virtual void UpdateColumnsPositions();

	private:
		friend class ListBoxCreator;

		struct CellData
		{
			CellData()
			:	m_bHighlighted( false ),
				m_sData( "" ),
				m_clColor( 0 )
			{
			}

			bool				m_bHighlighted;
			CFoundation::String		m_sData;
			CFoundation::Color		m_clColor;
		};

		ListBox( const CFoundation::String &rName );

		//! Returns the minimal width of the listBox, based on the minimal column width
		virtual Unsigned32      GetMinWidth() const;

		virtual Unsigned32	    GetWholeWidth() const;
		virtual Unsigned32      GetWholeHeight() const;

		void                    SetCellValue( Unsigned16 u16Row, Unsigned16 u16Col, const CFoundation::String &sData, const CFoundation::Color &clBgColor );

		ListBoxColumnCPtr       GetColumn( Unsigned32 _u32Col ) const;
		ListBoxColumnPtr        GetColumn( Unsigned32 _u32Col );

        /// Returns the index of the passed column
        Integer32               GetColumnIndex( GuiObjectPtr _spColumn ) const;

		void                    UpdateCell( Unsigned16 u16Row, Unsigned16 u16Col );
        void                    UpdateRow( Unsigned16 u16Row );

        /// Callback when a row of a column was focused
        void                    OnFocusedRow( GuiObjectPtr _spColumn, Unsigned32 _u32SelectedRow );

        /// Callback when a row of a column was clicked
        void                    OnClickedRow( GuiObjectPtr _spColumn, Unsigned32 _u32SelectedRow );

        /// Callback when a row of a column was double clicked
        void                    OnDoubleClickedRow( GuiObjectPtr _spColumn, Unsigned32 _u32SelectedRow );

        /// Callback when the value of a row in a column changed
        void                    OnValueChanged( ListBoxColumnPtr _spColumn, Unsigned32 _u32SelectedRow, const CFoundation::String& _strNewValue, const CFoundation::String& _strOldValue );

        /// Delegates
        ListBoxColumnEventDelegate2Ptr          m_spDelegateFocusedRow;
        ListBoxColumnEventDelegate2Ptr          m_spDelegateClickedRow;
        ListBoxColumnEventDelegate2Ptr          m_spDelegateDoubleClickedRow;
        ListBoxColumnEventStrStrDelegate4Ptr    m_spDelegateValueChanged;

		Unsigned16		m_u16NumRows;
		Unsigned16		m_u16NumMaxRows;

		Unsigned16		m_u16CurrentRow;
		Unsigned16		m_u16CurrentCol;

		// cell colors
		CFoundation::Color	m_clRowColor1;
		CFoundation::Color  m_clTextColor1;
		CFoundation::Color	m_clRowColor2;
		CFoundation::Color  m_clTextColor2;

		CFoundation::Color	m_clHighlightColor;
		CFoundation::Color	m_clHighlightTextColor;

		Integer32		    m_i32HighlightedRow;

		// Settings
		Unsigned32			m_u32HeaderHeight;

		FontCPtr            m_spHeaderFont;

		Unsigned32			m_u32RowHeight;

		FontCPtr            m_spCellFont;

		Unsigned32			m_u32Offset;

		Unsigned32			m_u32MinColumnWidth;

		bool				m_bScrollableHoriz;
		bool				m_bColumnsResizable;

		bool				m_bHeaderVisible;

		Unsigned8			m_u8GridSizes[ NUM_GRID_TYPES ];

		// SubCtrls
		typedef std::vector< ListBoxColumnPtr > ColumnVec;
		ColumnVec			m_aspColumns;
	};

    /// Listbox pointer
    typedef SharedPtr< ListBox > ListBoxPtr;

    /// Listbox const pointer
    typedef SharedPtr< const ListBox > ListBoxCPtr;

	class ListBoxCreator : public CombinedCtrlCreator, public CFoundation::Singleton<ListBoxCreator>
	{
	public:
		virtual const CFoundation::String GetTypeId() const;

		virtual GuiObjectPtr    CreateObject( const CFoundation::String &sName ) const;

        /// Initializes the passed object from a xml element
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

        /// Saves the passed object to a xml element
		virtual bool            SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const;
	};
}

#endif
