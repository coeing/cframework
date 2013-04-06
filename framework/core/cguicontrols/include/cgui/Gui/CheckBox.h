#ifndef CLIB_CHECKBOX_H
#define CLIB_CHECKBOX_H

#include "cgui/Font/Font.h"
#include "cgui/Gui/CombinedCtrl.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( Button );
    INCLUDE_SHARED_CLASS( CheckBox );
    INCLUDE_SHARED_CLASS( Font );
    INCLUDE_SHARED_CLASS( TextBox );

    /// Checkbox events
    enum CheckBoxEvent
    {
        EVENT_CHECKBOX_CHECK_CHANGED
    };
    TYPEDEF_DELEGATE3( CheckBoxEvent, CheckBoxPtr, bool, bool );

    class CheckBox : public CombinedCtrl, public CFoundation::Delegate3Invoker< CheckBoxEvent, CheckBoxPtr, bool, bool >
	{
	public:
        DELEGATE3_INVOKER( CheckBoxEvent, CheckBoxPtr, bool, bool );

        /// Destructor
        virtual ~CheckBox();

        /// Returns the type of this class
        static const CFoundation::String    GetTypeID();

        /// Returns the type of this object
		virtual const CFoundation::String   GetTypeId() const;

        /// Clears the object
		virtual void                        Clear();

		void                                SetChecked( bool bCheck );
		bool                                IsChecked() const;

		void                                SetText( const CFoundation::String& sText );
		const CFoundation::String&          GetText() const;

		void                                SetFont( FontCPtr _spFont );

	private:
		friend class CheckBoxCreator;

        /// Constructor
		CheckBox( const CFoundation::String& _strName );

        /// Initializes the button
        void                    InitButton();

        /// Deinitializes the button
        void                    DeinitButton();

		virtual void            SetButtonScaleable( bool bScaleable );

        /// Callbacks
        void                    OnButtonActivated();
        void                    OnButtonDeactivated();

        /// Delegates
        StateEventDelegate0Ptr  m_spDelegateButtonActivated;
        StateEventDelegate0Ptr  m_spDelegateButtonDeactivated;

		ButtonPtr       m_spButton;
		TextBoxPtr      m_spTextBox;

		Float32			m_f32ButtonRatio;
		bool            m_bButtonScaleable;
	};

	class CheckBoxCreator : public CombinedCtrlCreator
	{
	public:
        /// Returns the type id of the objects that are created with this creator
		virtual const CFoundation::String GetTypeId() const;

        /// Creates an object with the passed name
		virtual GuiObjectPtr    CreateObject( const CFoundation::String& _strName ) const;

        /// Does the default initialization
		virtual void            InitDefault( GuiObjectPtr _spObject ) const;

        /// Initializes the passed object from the passed xml element
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

        /// Initializes an object from another
		virtual void            InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const;
	};
}

#endif
