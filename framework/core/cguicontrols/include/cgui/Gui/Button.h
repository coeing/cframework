#ifndef CGUI_BUTTON_H
#define CGUI_BUTTON_H

#include "cgui/GUI/AlignmentType.h"
#include "cgui/GUI/CombinedCtrl.h"
#include "cgui/Font/Font.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( Button );
    INCLUDE_SHARED_CLASS( TextBox );

    /// Button event
    enum ButtonInputEvent
    {
        INPUT_EVENT_PUSHED = INPUT_EVENT_END,
    };
    TYPEDEF_DELEGATE1( ButtonInputEvent, ButtonPtr );

    class Button : public CombinedCtrl, public CFoundation::Delegate1Invoker< ButtonInputEvent, ButtonPtr >
	{
	public:
        DELEGATE3_INVOKER( RenderEvent, GuiObjectPtr, const RenderParams&, std::vector< RenderElementPtr >& );
        DELEGATE1_INVOKER( StateEvent, GuiObjectPtr );
        DELEGATE1_INVOKER( ObjectInputEvent, GuiObjectPtr );
        DELEGATE1_INVOKER( ButtonInputEvent, ButtonPtr );

	    /// The button type defines how the button
	    /// reacts on inputs
		enum ButtonType
		{
			TYPE_NONE,
			TYPE_NORMAL,
			TYPE_TOGGLE,
			NUM_TYPES
		};

		Button( const CFoundation::String& _strName );
		virtual ~Button();

        /// Clears the object
		virtual void                    Clear();

		/// Returns the Type ID
		static const CFoundation::String    GetTypeID();

		/// Returns the Type ID
		virtual const CFoundation::String   GetTypeId() const;

        /// Sets the button type
		virtual void                    SetType( ButtonType eType );

        /// Returns the button type
		virtual ButtonType              GetType() const;

        /// Returns the current background color
		//virtual const CFoundation::Color&   GetBgColor() const;

		virtual void                    SetOutline( Outline eOutline );

		virtual bool                    HasFocus() const;

		void                            SetMask( Unsigned8 u8ButtonState, const CFoundation::String& sMaskFile );
		void                            SetTexture( Unsigned8 u8ButtonState, const CFoundation::String& sTextureFile );

		/// Sets the text of the button
		virtual void                    SetText( const CFoundation::String& _strText );

        /// Sets the text for the passed state
		void                            SetText( State _eState, const CFoundation::String& _strText );

        /// Returns the text of the button
		virtual const CFoundation::String&  GetText() const;

        /// Sets the font of the button
		virtual void                    SetFont( FontCPtr _spFont );

        /// Returns the font of the button
        virtual FontCPtr                GetFont() const;

        /// Sets the text color for the passed state
		void                            SetTextColor( State _eState, const CFoundation::Color& _clTextColor );

        /// Sets the text alignment
		virtual void                    SetTextAlignment( AlignmentType eAlignType, Alignment eAlign );

		bool	                        IsDown() const;

		bool 	                        IsColored() const;
		void                            SetColored( bool bColored );

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Input
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// Called when the pointer left the object
        virtual void                    OnPointerLeft();

        /// Called when the object was pressed
        virtual void                    OnPressed();

        /// Called when the object was released
        virtual void                    OnReleased();

        /// Called when the object was clicked
        virtual void                    OnClicked();

    protected:
		TextBoxPtr                      GetTextBox();
		TextBoxCPtr                     GetTextBox() const;

		const CFoundation::RectF32&	    GetTextBB() const;

	private:
	    friend class ButtonCreator;

		void                            SetBackground( GuiObjectPtr _pBackground );
		GuiObjectPtr                    GetBackground();
		GuiObjectCPtr                   GetBackground() const;

		// Sub Controls
		GuiObjectPtr                    m_spBackground;
		TextBoxPtr                      m_spTextBox;

		// Data
		ButtonType		                m_eType;
		bool			                m_bColored;
	};
    
    /// Button pointer
    typedef SharedPtr< Button > ButtonPtr;
    
    /// Button const pointer
    typedef SharedPtr< const Button > ButtonCPtr;

	class ButtonCreator : public CombinedCtrlCreator
	{
	public:
		virtual const CFoundation::String GetTypeId() const;

		virtual GuiObjectPtr    CreateObject( const CFoundation::String &sName ) const;

		virtual void            InitDefault( GuiObjectPtr _spObject ) const;

        /// Initializes the passed object from a xml element
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

        /// Initializes an object from another
		virtual void            InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const;

        /// Saves the passed object to a xml element
		virtual bool            SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const;
	};
};

#endif
