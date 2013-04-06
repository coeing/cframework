#ifndef CGUI_LISTBOXCELL_H
#define CGUI_LISTBOXCELL_H

#include <map>

#include "cgui/Font/Font.h"
#include "cgui/Gui/CombinedCtrl.h"

#include "cgui/Gui/CellType.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( ComboBox );
    TYPEDEF_DELEGATE3( ComboBoxEventI64Str, ComboBoxPtr, Integer64, const CFoundation::String& );
    INCLUDE_SHARED_CLASS( EditBox );
    TYPEDEF_DELEGATE3( EditBoxEvent, EditBoxPtr, const CFoundation::String&, const CFoundation::String& );
    INCLUDE_SHARED_CLASS( CheckBox );
    TYPEDEF_DELEGATE3( CheckBoxEvent, CheckBoxPtr, bool, bool );
}

namespace CGui
{
    INCLUDE_SHARED_CLASS( ListBoxCell );

    /// ListBoxCell event
    enum ListBoxCellEvent
    {
        EVENT_LISTBOXCELL_VALUE_CHANGED,
    };
    TYPEDEF_DELEGATE3( ListBoxCellEvent, ListBoxCellPtr, const CFoundation::String&, const CFoundation::String& );

	class ListBoxCell : public CombinedCtrl, public CFoundation::Delegate3Invoker< ListBoxCellEvent, ListBoxCellPtr, const CFoundation::String&, const CFoundation::String& >
	{
	public:
        DELEGATE3_INVOKER( RenderEvent, GuiObjectPtr, const RenderParams&, std::vector< RenderElementPtr >& );
        DELEGATE1_INVOKER( StateEvent, GuiObjectPtr );
        DELEGATE1_INVOKER( ObjectInputEvent, GuiObjectPtr );
        DELEGATE1_INVOKER( SizeEvent, GuiObjectPtr );
        DELEGATE3_INVOKER( PositionEvent, GuiObjectPtr, const CFoundation::Vector2Di&, const CFoundation::Vector2Di& );
        DELEGATE3_INVOKER( ListBoxCellEvent, ListBoxCellPtr, const CFoundation::String&, const CFoundation::String& );

        /// Constructor
		ListBoxCell( const CFoundation::String& _strName );

		static const CFoundation::String GetTypeID();
		virtual const CFoundation::String GetTypeId() const;

        /// Clears the object
		virtual void    Clear();

		void            SetType( const CellType &eType );
		const CellType& GetType() const;

        /// Sets the passed state flag
        virtual void    SetStateFlag( StateFlag _eStateFlag, bool _bSet );

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Input
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// Processes input as focused object
        /// Returns if input was processed. If not the input is send to the parent, 
        ///                                 if object isn't independent
        virtual bool    ProcessInputFocused( const CSystem::InputState& _State );

        /// Processes input as pointed object
        /// Returns if input was processed. If not the input is send to the parent, 
        ///                                 if object isn't independent
        virtual bool    ProcessInputPointed( const CSystem::InputState& _State );

        /// Called when the object gained the focus
        virtual void    OnGainedFocus();

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// ListBoxCell functions
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// Sets the value of the cell
		void            SetValue( const CFoundation::String& _strValue );

        /// Sets the values of the cell
        void            SetValues( const std::vector< CFoundation::String >& _astrValues );

		virtual void    SetFont( FontCPtr _spFont );

	private:
        /// Returns the cached cell or creates a new one
        /// of the passed type
        GuiObjectPtr    GetCell( CellType _eType );

        /// Sets the cell
        void            SetCell( GuiObjectPtr _spCell );

        /// Callbacks
        void            OnCellValueChanged( ComboBoxPtr _spComboBox, Integer64 _i64Data, const CFoundation::String& _strNewValue );
        void            OnCheckChanged( CheckBoxPtr _spCheckBox, bool _bNewState, bool _bOldState );

        /// Callback when text of cell changed
        void            OnCellTextChanged( EditBoxPtr _spEditBox, const CFoundation::String& _strNewValue, const CFoundation::String& _strOldValue );

        /// Callback when value of cell changed
        //void            OnCellCheckChanged( GuiObjectPtr _sp

        /// Delegates
        ComboBoxEventI64StrDelegate3Ptr m_spDelegateCellValueChanged;
        EditBoxEventDelegate3Ptr    m_spDelegateCellTextChanged;
        EditBoxEventDelegate3Ptr    m_spDelegateCellFocused;
        CheckBoxEventDelegate3Ptr   m_spDelegateCheckChanged;

        /// Current cell type
		CellType	    m_eType;

        /// Current cell
		GuiObjectPtr    m_spCell;

        /// Font
		FontCPtr        m_spFont;

		// Cell cache
		typedef std::map< CellType, GuiObjectPtr > ObjectMap;
		ObjectMap	    m_clObjects;
	};

    /// Listbox cell pointer
    typedef SharedPtr< ListBoxCell > ListBoxCellPtr;

    /// Listbox cell const pointer
    typedef SharedPtr< const ListBoxCell > ListBoxCellCPtr;

	class ListBoxCellCreator : public GuiObjectCreator, public CFoundation::Singleton<ListBoxCellCreator>
	{
	public:
		virtual const CFoundation::String GetTypeId() const;

		virtual GuiObjectPtr    CreateObject( const CFoundation::String &sName ) const;
	};
}

#endif
