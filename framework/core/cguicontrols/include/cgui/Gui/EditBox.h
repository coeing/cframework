#ifndef CGUI_EDITBOX_H
#define CGUI_EDITBOX_H

#include "cgui/Gui/Button.h"
#include "cgui/Gui/Image.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( EditBox );

    /// EditBox event
    enum EditBoxEvent
    {
        EVENT_EDITBOX_TEXT_CHANGED,
    };
    TYPEDEF_DELEGATE3( EditBoxEvent, EditBoxPtr, const CFoundation::String&, const CFoundation::String& );

	class EditBox : public Button, public CFoundation::Delegate3Invoker< EditBoxEvent, EditBoxPtr, const CFoundation::String&, const CFoundation::String& >
	{
	public:
        DELEGATE3_INVOKER( EditBoxEvent, EditBoxPtr, const CFoundation::String&, const CFoundation::String& );

        /// Destructor
		~EditBox();

		static const CFoundation::String    GetTypeID();
		virtual const CFoundation::String   GetTypeId() const;

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Input
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// Processes input as focused object
        virtual bool                    ProcessInputFocused( const CSystem::InputState& _state );

        /// Processes input as pointed object
        virtual bool                    ProcessInputPointed( const CSystem::InputState& _state );

        /// Called when the object gained the focus
        virtual void                    OnGainedFocus();

        /// Called when the object lost the focus
        virtual void                    OnLostFocus();

        /// Per frame update
		virtual void                    Update( Float32 _f32TimeDifference );

        /// Sets the passed state flag
        virtual void                    SetStateFlag( StateFlag _eStateFlag, bool _bSet );

        /// Sets the text of the editbox
		virtual void                    SetText( const CFoundation::String& _strText );

        /// Sets the font of the editbox
		virtual void                    SetFont( FontCPtr _spFont );

        /// Sets the caret blink speed (in sec)
        virtual void                    SetCaretBlinkSpeed( Float32 _f32BlinkSpeedMs );

        /// Returns the caret blink speed (in sec)
        virtual Float32                 GetCaretBlinkSpeed() const;

        /// Sets the caret position
        virtual void                    SetCaretPosition( Unsigned32 _u32CaretPosition );

        /// Returns the caret position
        virtual Unsigned32              GetCaretPosition() const;

	protected:
		EditBox( const CFoundation::String &rName );

        /// Sets the text of the editbox
		virtual void                    SetText( const CFoundation::String& _strText, bool _bResetCaret );

        /// Changes the size of the object
        virtual void                    SetSize( const CFoundation::Vector2Du& _vSize );

        /// Returns the caret
        virtual ImagePtr                GetCaret() const;

        /// Computes the text position and caret position from the current settings
        virtual CFoundation::Vector2Di  ComputeTextPosition( CFoundation::Vector2Di& _vCaretPosition ) const;

	private:
		friend class EditBoxCreator;

		void        UpdateCaretPosition();
		void        UpdateCaretHeight();

		/// Indicates how long stays the caret visible/invisible (in seconds)
		Float32     m_f32CaretBlinkSpeed;

        /// Counts the passed time (in seconds)
		Float32     m_f32PassedTime;

		/// Caret object
        ImagePtr    m_spImgCaret;

        /// Initial text
        CFoundation::String m_strInitialText;

        /// Current caret position
        Unsigned32  m_u32CaretPosition;
	};

    /// EditBox pointer
    typedef SharedPtr< EditBox > EditBoxPtr;

    /// EditBox const pointer
    typedef SharedPtr< const EditBox > EditBoxCPtr;

	class EditBoxCreator : public ButtonCreator
	{
	public:
		virtual const CFoundation::String GetTypeId() const;

		virtual GuiObjectPtr    CreateObject( const CFoundation::String &sName ) const;
		virtual void            InitDefault( GuiObjectPtr _spObject ) const;

        /// Initializes the passed object from a xml element
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

        /// Initializes an object from another
		virtual void            InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const;
	};
}

#endif
