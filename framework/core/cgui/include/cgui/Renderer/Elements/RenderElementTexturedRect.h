#ifndef CGUI_RENDER_ELEMENT_TEXTURED_RECT_H
#define CGUI_RENDER_ELEMENT_TEXTURED_RECT_H

#ifdef MULTIPLE_RENDER_ELEMENTS

#include "cfoundation/Math/Vector.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/Color.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cgui/Renderer/Elements/RenderElementRect.h"
#include "cgui/Texture/Texture.h"

namespace CGui
{
    /** RenderElementTexturedRect
     *
     *  Render element that holds the information to render a text
	 * 
	 */
    class RenderElementTexturedRect : public RenderElementRect
    {
    public:
        /// Constructor
        RenderElementTexturedRect();

        /// Destructor
        virtual ~RenderElementTexturedRect();

        /// Sets the mask to use
        void                            SetMask( const Texture& _Mask );

        /// Returns the mask to use
        const Texture&                  GetMask() const;

        /// Sets if the background should be visible
        void                            SetBgVisible( bool _bBgVisible );

        /// Indicates if the background should be visible
        bool                            IsBgVisible() const;

        /// Sets the color that should be us
        void                            SetBgColor( CFoundation::Color _Color );

        /// Returns the color that should be used
        CFoundation::Color              GetBgColor() const;

        /// Sets the texture to use
        void                            SetTexture( const Texture& _Texture );

        /// Returns the texture to use
        const Texture&                  GetTexture() const;

        /// Sets the texture rectangle
        void                            SetTextureRect( const CFoundation::RectF16& _rectTexture );

        /// Returns the texture rectangle
        const CFoundation::RectF16&     GetTextureRect() const;

        /// Returns the texture rectangle in relative coordinates
        const CFoundation::RectF16      GetRelativeTextureRect() const;

    private:   
        /// Mask to use
        Texture                 m_Mask;

        /// Indicates if the background should be visible
        bool                    m_bBgVisible;

        /// Color to use
        CFoundation::Color      m_Color;

        /// Texture to use
        Texture                 m_Texture;

        /// Texture rect
        CFoundation::RectF16    m_rectTexture;
    };

    INCLUDE_SHARED_CLASS( RenderElementTexturedRect );
}

#endif

#endif
