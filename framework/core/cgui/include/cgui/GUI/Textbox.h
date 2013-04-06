#ifndef CGUI_TEXTBOX_H
#define CGUI_TEXTBOX_H

#include "cfoundation/Utils/Singleton.h"

#include "cgui/Font/Font.h"
#include "cgui/GUI/AlignmentType.h"
#include "cgui/GUI/GuiObject.h"
#include "cgui/GUI/GuiObjectCreator.h"

namespace CGui
{
	class TextBoxView;

	class TextBox : public GuiObject
	{
	public:
		enum CutType
		{
			CUT_ABBR,
			CUT_CONTINUE,
            CUT_LEFT,
            CUT_WRAP,
            CUT_WORD_WRAP,
			CUT_TYPE_END
		};

        /// Destructor
		virtual ~TextBox();

		static const CFoundation::String        GetTypeID();
		virtual const CFoundation::String       GetTypeId() const;

        /// Finishes initializing the object
        virtual void                        EndInit();

		virtual Integer32                   GetInnerWidth() const;
		virtual Integer32                   GetInnerHeight() const;

        /// Sets the text
		virtual void                        SetText( const CFoundation::String& _strText );

        /// Returns the text
		virtual const CFoundation::String&  GetText() const;

        /// Sets the text for the passed state
		virtual void                        SetText( State _eState, const CFoundation::String& _strText );

        /// Returns the text for the passed state
		virtual const CFoundation::String&  GetText( State _eState ) const;

        /// Sets the font
		virtual void                        SetFont( const CFoundation::String& _strFontName, Unsigned8 _u8FontSize );

        /// Sets the font
		virtual void                        SetFont( FontCPtr _spFont );

        /// Returns the font
        virtual FontCPtr                    GetFont() const;

        /// Sets the text color for the passed state
		virtual void                        SetTextColor( State _eState, const CFoundation::Color& _clColor );

        /// Returns the text color for the passed state
		virtual const CFoundation::Color&   GetTextColor( State _eState ) const;

        /// Sets the text alignment
		virtual void                        SetTextAlignment( AlignmentType eAlignType, Alignment eAlign );

        /// Returns the text alignment
		virtual Alignment                   GetTextAlignment( AlignmentType eAlignType ) const;

        /// Makes the text underline for the passed state
        virtual void                        SetUnderlined( State _eState, bool _bUnderlined );

        /// Indicates if the text is underlined for the passed state
        virtual bool                        IsUnderlined( State _eState ) const;

        /// Sets the padding for each side
        virtual void                        SetPadding( Integer32 _i32PaddingLeft, Integer32 _i32PaddingTop, Integer32 _i32PaddingRight, Integer32 _i32PaddingBottom );

        /// Sets the left padding
        virtual void                        SetPaddingLeft( Integer32 _i32Padding );

        /// Returns the left padding
        virtual Integer32                   GetPaddingLeft() const;

        /// Sets the top padding
        virtual void                        SetPaddingTop( Integer32 _i32Padding );

        /// Returns the top padding
        virtual Integer32                   GetPaddingTop() const;

        /// Sets the right padding
        virtual void                        SetPaddingRight( Integer32 _i32Padding );

        /// Returns the right padding
        virtual Integer32                   GetPaddingRight() const;

        /// Sets the bottom padding
        virtual void                        SetPaddingBottom( Integer32 _i32Padding );

        /// Returns the bottom padding
        virtual Integer32                   GetPaddingBottom() const;

        /// Sets the cut type
		virtual void                        SetCutType( CutType eType );

        /// Sets the cut offset
        virtual void                        SetCutOffset( const CFoundation::Vector2Df& _vCutOffset );

        /// Returns the cut offset
        virtual const CFoundation::Vector2Df&   GetCutOffset() const;

		virtual const CFoundation::RectF32&	    GetTextBB() const;

    protected:
        /// Changes the current state
        virtual void                        SetState( State _eState );

        /// Changes the size of the object
        virtual void                        SetSize( const CFoundation::Vector2Du& _vSize );

        /// Computes the size of the object
        /*! Depending on the size mode the real size
         *  of the object is computed.
         */
        virtual CFoundation::Vector2Du      ComputeSize();

	private:
        typedef std::map< State, CFoundation::Color > StateColorMap;
        typedef std::map< State, bool > StateBoolMap;
        typedef std::map< State, CFoundation::String > StateStringMap;

		friend class TextBoxCreator;
		TextBox( const CFoundation::String &rName );

        /// Sets the output text
        void                                SetOutputText( const CFoundation::String& _strText );

		void UpdateOutputText();
		void UpdateTextPosition();

	    TextBoxView*            m_pTextBoxView;

        /// Text
		CFoundation::String		m_strText;

		/// Bounding box of the shown text
		CFoundation::RectF32	m_clOutputTextBB;

        /// Text alignment
		Alignment			    m_eAlign[ ALIGNMENT_TYPE_END ];

        /// Left padding
        Integer32               m_i32PaddingLeft;
        
        /// Top padding
        Integer32               m_i32PaddingTop;
        
        /// Right padding
        Integer32               m_i32PaddingRight;
        
        /// Bottom padding
        Integer32               m_i32PaddingBottom;

        /// Method to use when text is too long
		CutType				    m_eCutType;

        /// Cut offset
        CFoundation::Vector2Df  m_vCutOffset;

        /// Text colors for each state
		StateColorMap		    m_clTextColors;

        /// Underline flag for each state
        StateBoolMap            m_mapUnderlined;

        /// Text for each state
        StateStringMap          m_mapTexts;

        /// Font
        FontCPtr                m_spFont;
	};

    /// Textbox pointer
    typedef SharedPtr< TextBox > TextBoxPtr;

    /// Textbox const pointer
    typedef SharedPtr< const TextBox > TextBoxCPtr;

	class TextBoxCreator : public GuiObjectCreator, public CFoundation::Singleton<TextBoxCreator>
	{
	public:
		virtual const CFoundation::String GetTypeId() const;

		virtual GuiObjectPtr    CreateObject( const CFoundation::String &sName ) const;

		virtual void            InitDefault( GuiObjectPtr _spObject ) const;
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;
        
        //! Initializes an object from another
		virtual void            InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const;

		//virtual void            AssignStyle( GuiObjectPtr _spObject, const CGui::CtrlStyle& clStyle ) const;

        /// Saves the passed object to a xml element
		virtual bool            SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const;
	};
};

#endif
