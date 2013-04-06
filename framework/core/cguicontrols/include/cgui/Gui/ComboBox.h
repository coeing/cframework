#ifndef CGUI_COMBOBOX_H
#define CGUI_COMBOBOX_H

#include "cgui/Gui/AlignmentType.h"
#include "cgui/Gui/CombinedCtrl.h"
#include "cgui/Gui/TextColorType.h"

#include "cgui/Font/Font.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( Button );
    INCLUDE_SHARED_CLASS( ComboBox );
    INCLUDE_SHARED_CLASS( EditBox );
    INCLUDE_SHARED_CLASS( ListBox );

    TYPEDEF_DELEGATE2( ListBoxEvent, Unsigned32, Unsigned32 );

    enum ComboBoxEventI64Str
    {
        EVENT_COMBOBOX_VALUE_CHANGED,
    };
    TYPEDEF_DELEGATE3( ComboBoxEventI64Str, ComboBoxPtr, Integer64, const CFoundation::String& );

    class ComboBox : public CombinedCtrl, public CFoundation::Delegate3Invoker< ComboBoxEventI64Str, ComboBoxPtr, Integer64, const CFoundation::String& >
	{
	public:
        DELEGATE3_INVOKER( ComboBoxEventI64Str, ComboBoxPtr, Integer64, const CFoundation::String& );

        /// Destructor
		virtual ~ComboBox();

		//! Sets the height of the bar
		/*! The bar consists of the editbox and the
		 * 	button and is always visible unlike the
		 *  drop-down list, which can be shown and
		 *  hidden by the button.
		 */
		virtual void SetBarHeight( Unsigned32 u32BarHeight );

		//! Returns the bar height
		/*! The bar consists of the editbox and the
		 * 	button and is always visible unlike the
		 *  drop-down list, which can be shown and
		 *  hidden by the button.
		 */
		virtual Unsigned32 GetBarHeight() const;

		virtual void SetDropDownListHeight( Unsigned32 u32ListHeight );

		virtual Unsigned32 GetDropDownListHeight() const;

		//! redefine GuiObject functions
		static const CFoundation::String    GetTypeID();
		virtual const CFoundation::String   GetTypeId() const;

		//! define functions for the EditBox
		void                            SetText( const CFoundation::String &rText );
		const CFoundation::String&          GetText() const;

        /// Sets the font
		virtual void                    SetFont( FontCPtr _spFont );

        /// Returns the font
        virtual FontCPtr                GetFont() const;

		void                            SetTextColor( State _eState, const CFoundation::Color& _clColor );

		void                            SetTextAlignment( AlignmentType eAlignType, Alignment eAlign );

		//! Adds an item to the drop down list
		void                            AddItem( const CFoundation::String& sText, Integer64 i64Data = 0 );

        /// Returns the current selected item index
        Unsigned32                      GetSelectedItemIdx() const;

        /// Returns the current selected item data
        Integer64                       GetSelectedItemData() const;

	protected:
		virtual void UpdateEditBoxSize();
		virtual void UpdateButtonSize();
		virtual void UpdateListBoxSize();
		virtual void UpdateListBoxPosition();

		virtual void ShowDropDownList( bool bShow = true );

        /// Constructor
		ComboBox( const CFoundation::String& _strName );

        /// Sets the parent of this object
		virtual void                SetParent( PanelPtr _spParent );

        /// Changes the position of the object
        virtual bool                SetPosition( const CFoundation::Vector2Di& _vPosition );

        /// Changes the size of the object
        virtual void                SetSize( const CFoundation::Vector2Du& _vSize );

        /// Callbacks
        virtual void                OnAttachObjectPositionChanged();

	private:
		friend class ComboBoxCreator;

        /// Callback when cell of list box was selected
        void                        OnSelectedCell( Unsigned32 _u32SelectedRow, Unsigned32 _u32SelectedCol );

        /// Callbacks
        void                    OnButtonActivated();
        void                    OnButtonDeactivated();

        /// Delegate when cell of list box was selected
        ListBoxEventDelegate2Ptr    m_spDelegateSelectedCell;

        /// Delegates
        StateEventDelegate0Ptr  m_spDelegateButtonActivated;
        StateEventDelegate0Ptr  m_spDelegateButtonDeactivated;

		// settings
		Unsigned32		m_u32BarHeight;

		// Sub Controls
		EditBoxPtr      m_spEditBox;
		ButtonPtr       m_spListButton;
		ListBoxPtr      m_spDropDownList;
	};

    /// ComboBox pointer
    typedef SharedPtr< ComboBox > ComboBoxPtr;

    /// ComboBox const pointer
    typedef SharedPtr< const ComboBox > ComboBoxCPtr;

	class ComboBoxCreator : public CombinedCtrlCreator, public CFoundation::Singleton<ComboBoxCreator>
	{
	public:
		virtual const CFoundation::String GetTypeId() const;

		virtual GuiObjectPtr    CreateObject( const CFoundation::String &sName ) const;

		virtual void            InitDefault( GuiObjectPtr _spObject ) const;

        /// Initializes the passed object from a xml element
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

		/*
		virtual ComboBox& AssignStyle( GuiObject& clObject, const CGui::CtrlStyle& clStyle ) const;
		virtual CFoundation::XMLElement& SaveToXML( CFoundation::XMLElement &clXMLElement, const GuiNode &clNode ) const;*/
	};
};

#endif
