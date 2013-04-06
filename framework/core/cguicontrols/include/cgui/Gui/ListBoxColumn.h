#ifndef CGUI_LISTBOX_COLUMN_H
#define CGUI_LISTBOX_COLUMN_H

#include <map>

#include "cgui/Font/Font.h"

#include "cgui/Gui/CellType.h"
#include "cgui/Gui/CombinedCtrl.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( ListBox );
    INCLUDE_SHARED_CLASS( ListBoxCell );
    INCLUDE_SHARED_CLASS( ListBoxColumn );
    INCLUDE_SHARED_CLASS( TextBox );
    
    TYPEDEF_DELEGATE3( ListBoxCellEvent, ListBoxCellPtr, const CFoundation::String&, const CFoundation::String& );

    enum ListBoxColumnEvent
    {
        EVENT_LISTBOXCOLUMN_FOCUSED_ROW,
        EVENT_LISTBOXCOLUMN_CLICKED_ROW,
        EVENT_LISTBOXCOLUMN_DOUBLE_CLICKED_ROW,
    };
    TYPEDEF_DELEGATE2( ListBoxColumnEvent, GuiObjectPtr, Unsigned32 );

    enum ListBoxColumnEventStrStr
    {
        EVENT_LISTBOXCOLUMN_VALUE_CHANGED,
    };
    TYPEDEF_DELEGATE4( ListBoxColumnEventStrStr, ListBoxColumnPtr, Unsigned32, const CFoundation::String&, const CFoundation::String& );

    class ListBoxColumn : public CombinedCtrl, public CFoundation::Delegate2Invoker< ListBoxColumnEvent, GuiObjectPtr, Unsigned32 >, 
                                               public CFoundation::Delegate4Invoker< ListBoxColumnEventStrStr, ListBoxColumnPtr, Unsigned32, const CFoundation::String&, const CFoundation::String& >
	{
	public:
        DELEGATE2_INVOKER( ListBoxColumnEvent, GuiObjectPtr, Unsigned32 );
        DELEGATE4_INVOKER( ListBoxColumnEventStrStr, ListBoxColumnPtr, Unsigned32, const CFoundation::String&, const CFoundation::String& );

		// redefine GuiObject functions
		static const CFoundation::String    GetTypeID();
		virtual const CFoundation::String   GetTypeId() const;

        /// Clears the object
		virtual void                    Clear();

		virtual void                    SetMinWidth( Unsigned32 u32MinWidth );
		virtual Unsigned32              GetMinWidth() const;

        virtual void                    SetSize( const CFoundation::Vector2Du& _clSize );

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Input
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// Processes input as focused object
        /// Returns if input was processed. If not the input is send to the parent, 
        ///                                 if object isn't independent
        virtual bool                    ProcessInputFocused( const CSystem::InputState& _State );

        /// Processes input as pointed object
        /// Returns if input was processed. If not the input is send to the parent, 
        ///                                 if object isn't independent
        virtual bool                    ProcessInputPointed( const CSystem::InputState& _State );

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// ListBox column functions
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// Clears all content
		void                            ClearContent();

		void					        SetTitle( const CFoundation::String &sTitle );
		const CFoundation::String& 	    GetTitle() const;

		void                            SetHighlightedRow( Integer32 _i32Row );

		// Settings
		void                            SetHeaderHeight( Unsigned32 u32HeaderHeight );
		Unsigned32                      GetHeaderHeight() const;

		void                            SetHeaderFont( FontCPtr _spFont );

		void                            SetGridSize( Unsigned8 u8GridSize );
		Unsigned8                       GetGridSize() const;

        /// Sets the grid color
        void                            SetGridColor( const CFoundation::Color& _clColor );

        /// Returns the grid color
        const CFoundation::Color&       GetGridColor() const;

		void                            SetRowHeight( Unsigned32 u32RowHeight );
		Unsigned32                      GetRowHeight() const;

		void                            SetCellFont( FontCPtr _spFont );

		void                            SetRowColor1( const CFoundation::Color &clColor );
		void                            SetTextColor1( const CFoundation::Color &clColor );
		void                            SetRowColor2( const CFoundation::Color &clColor );
		void                            SetTextColor2( const CFoundation::Color &clColor );
		void                            SetHighlightColor( const CFoundation::Color &clColor );
		void                            SetHighlightTextColor( const CFoundation::Color &clColor );

        /// Sets a cell's type
        void                            SetCellType( Unsigned32 _u32Row, const CGui::CellType& _eType );

		void                            SetHeaderOffset( Unsigned32 u32HeaderOffset );

		void                            SetHeaderVisible( bool bVisible );
		bool                            IsHeaderVisible() const;

		// Data
		Unsigned16                      GetNumRows() const;

		void                            SetCellValue( Unsigned32 _u32Row, const CFoundation::String& _strValue );
		const CFoundation::String&      GetCellValue( Unsigned32 _u32Row ) const;

        /// Sets the cell values
        void                            SetCellValues( Unsigned32 _u32Row, const std::vector< CFoundation::String >& _astrValues );

		void                            SetCellData( Unsigned32 _u32Row, Integer64 i64Data );
		Integer64                       GetCellData( Unsigned32 _u32Row ) const;

        /// Sets the cell values
        void                            SetCellData( Unsigned32 _u32Row, const std::vector< Integer64 >& _ai64Data );

		void                            SetCellBgColor( Unsigned16 u16Row, const CFoundation::Color &clColor );

	protected:
		TextBoxPtr                      GetHeader();
		TextBoxCPtr                     GetHeader() const;

		ListBoxCellPtr                  GetCell( Unsigned32 _u32Row );
		ListBoxCellCPtr                 GetCell( Unsigned32 _u32Row ) const;
        
        /// Returns the cell for the passed cell or creates it if not found
        ListBoxCellPtr                  CreateCell( Unsigned32 _u32Row );

        //! Computes the size of the object
        /*! Depending on the size mode the real size
         *  of the object is computed.
         */
        virtual CFoundation::Vector2Du  ComputeSize();

        /// Called when a piece gained focus
        virtual void                    OnPieceGainedFocus( GuiObjectPtr _spPiece );

        /// Called when a piece lost focus
        virtual void                    OnPieceLostFocus( GuiObjectPtr _spPiece );

	private:
		friend class ListBoxColumnCreator;

		struct CellData
		{
			CellData()
			:	m_bHighlighted( false ),
				m_sValue( "" ),
				m_i64Data( 0 ),
				m_clTextColor( 0xffffff )
			{
			}

			void Clear()
			{
				m_bHighlighted = false;
				m_sValue = "";
				m_i64Data = 0;
			}

			bool				m_bHighlighted;
			CellType			m_eType;
			CFoundation::String m_sValue;
			Integer64           m_i64Data;
			CFoundation::Color  m_clTextColor;
		};

		ListBoxColumn( const CFoundation::String &sName );

        /// Adds a cell
        /// Returns the added cell
		ListBoxCellPtr  AddCell();

        /// Removes the passed cell
        void            RemoveCell( ListBoxCellPtr _spCell );

        /// Returns the index of the passed cell
        Integer32       GetCellIndex( GuiObjectPtr _spCell ) const;

		void            ResizeCells();
		void            RepositionHeader();
		void            RepositionCells();
		void            HideCells();
		void            CreateNewCells();

        /// Sets a cell's highlight state
        void                    SetCellHighlighted( Unsigned32 _u32Row, bool _bHighlighted );

        /// Called when cell gained the focus
        void                    OnCellFocusGained( GuiObjectPtr _spObject );

        /// Called when cell lost the focus
        void                    OnCellFocusLost( GuiObjectPtr _spObject );

        /// Called when a cell was clicked
        void                    OnClickedCell( GuiObjectPtr _spObject );

        /// Called when a cell was double clicked
        void                    OnDoubleClickedCell( GuiObjectPtr _spObject );

        /// Callback when the value of a cell changed
        void                    OnCellValueChanged( ListBoxCellPtr _spCell, const CFoundation::String& _strNewValue, const CFoundation::String& _strOldValue );

        /// Delegates
        ObjectInputEventDelegate1Ptr m_spDelegateCellFocusGained;
        ObjectInputEventDelegate1Ptr m_spDelegateCellFocusLost;
        ObjectInputEventDelegate1Ptr m_spDelegateClickedCell;
        ObjectInputEventDelegate1Ptr m_spDelegateDoubleClickedCell;
        ListBoxCellEventDelegate3Ptr    m_spDelegateCellValueChanged;

		// Settings
		Unsigned32 			m_u32MinWidth;

		Unsigned32			m_u32HeaderHeight;

		Unsigned8			m_u8GridSize;
		Unsigned32			m_u32RowHeight;

		FontCPtr            m_spCellFont;

		CFoundation::Color		m_clRowColor1;
		CFoundation::Color		m_clTextColor1;
		CFoundation::Color		m_clRowColor2;
		CFoundation::Color		m_clTextColor2;
		CFoundation::Color		m_clHighlightColor;
		CFoundation::Color		m_clHighlightTextColor;

		bool				m_bHeaderVisible;

		// Data
		typedef std::map< Unsigned32, CellData > CellDataMap;
		CellDataMap			m_clCellData;

        /// Current focused cell        
		Integer32			m_i32FocusedRow;

		Integer32			m_i32HighlightedRow;

		// SubCtrls
		TextBoxPtr			m_pHeader;
		Unsigned32			m_u32HeaderOffset;

		typedef std::vector< ListBoxCellPtr > CellVec;
		CellVec				m_aspCells;
	};

    /// Listbox column pointer
    typedef SharedPtr< ListBoxColumn > ListBoxColumnPtr;

    /// Listbox column const pointer
    typedef SharedPtr< const ListBoxColumn > ListBoxColumnCPtr;

	class ListBoxColumnCreator : public CombinedCtrlCreator, public CFoundation::Singleton<ListBoxColumnCreator>
	{
	public:
		virtual const CFoundation::String GetTypeId() const;

		virtual GuiObjectPtr    CreateObject( const CFoundation::String &sName ) const;
		virtual void            InitDefault( GuiObjectPtr _spObject ) const;

        /// Initializes the passed object from a xml element
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;
	};
}

#endif
