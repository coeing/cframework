#ifndef CLIB_TEXTBOXVIEW_H
#define CLIB_TEXTBOXVIEW_H

#include "cgui/Font/Font.h"
#include "cgui/GUI/GuiObjectView.h"

namespace CGui
{
	class TextBoxView : public GuiObjectView
	{
		static Unsigned32 sID;

	public:
		TextBoxView();

        /// Collects the render elements
#ifdef MULTIPLE_RENDER_ELEMENTS
		virtual void Render( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements );
#else
		virtual void Render( const RenderParams& _Params, std::vector< RenderElement >& _aRenderElements );
#endif

        /// Sets the font
		void    SetFont( FontCPtr _spFont );

        /// Sets the text of the passed line
		void    SetText( const CFoundation::String& _strText, Unsigned32 _u32Line = 0 );

        /// Removes the text in the passed line
        void    RemoveText( Unsigned32 _u32Line );

        /// Removes all texts
        void    RemoveTexts();

        /// Sets the text color
		void    SetTextColor( const CFoundation::Color& _clColor );

		void    SetTextPosition( const CFoundation::Vector2Di &clPosition );

        /// Makes the text underline
        void    SetUnderlined( bool _bUnderlined );

        /// Indicates if the text is underlined
        bool    IsUnderlined() const;

	private:
        typedef std::map< Unsigned32, CFoundation::String > LineTextMap;

        /// Text lines
		LineTextMap		        m_strTexts;

        /// Text position
		CFoundation::Vector2Di	m_clTextPosition;

        /// Font
		FontCPtr                m_spFont;

        /// Text color
		CFoundation::Color		m_clTextColor;

        /// Indicates if the text is underlined
        bool                    m_bUnderlined;
	};
}

#endif
