#ifndef CGUI_RENDER_ELEMENT_TEXT_H
#define CGUI_RENDER_ELEMENT_TEXT_H

#ifdef MULTIPLE_RENDER_ELEMENTS

#include "cfoundation/Math/Vector.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/Color.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cgui/Renderer/Elements/RenderElement.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( Font );

    /** RenderElementText
     *
     *  Render element that holds the information to render a text
	 * 
	 */
    class RenderElementText : public RenderElement
    {
    public:
        /// Constructor
        RenderElementText();

        /// Destructor
        virtual ~RenderElementText();

        /// Sets the text that should be rendered
        void                            SetText( const CFoundation::String& _strText );

        /// Returns the text that should be rendered
        const CFoundation::String&      GetText() const;

        /// Sets the font that should be us
        void                            SetFont( FontCPtr _spFont );

        /// Returns the font that should be used
        FontCPtr                        GetFont() const;

        /// Sets the color that should be us
        void                            SetColor( CFoundation::Color _Color );

        /// Returns the color that should be used
        CFoundation::Color              GetColor() const;

        /// Makes the text underline
        //void                            SetUnderlined( bool _bUnderlined );

        /// Indicates if the text is underlined
        //bool                            IsUnderlined() const;

    private:   
        /// Text to render
        CFoundation::String     m_strText;

        /// Position
        CFoundation::Vector2Di  m_vPosition;

        /// Font to use
        FontCPtr                m_spFont;

        /// Color to use
        CFoundation::Color      m_Color;

        /// Indicates if the text is underlined
        //bool                    m_bUnderlined;
    };

    INCLUDE_SHARED_CLASS( RenderElementText );
}

#endif

#endif
