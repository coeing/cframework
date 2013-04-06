#ifndef CGUI_RENDER_ELEMENT_RECT_H
#define CGUI_RENDER_ELEMENT_RECT_H

#ifdef MULTIPLE_RENDER_ELEMENTS

#include "cfoundation/Math/Vector.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/Color.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cgui/Renderer/Elements/RenderElement.h"
#include "cgui/Texture/Texture.h"

namespace CGui
{
    /** RenderElementRect
     *
     *  Render element that holds the information to render a text
	 * 
	 */
    class RenderElementRect : public RenderElement
    {
    public:
        /// Constructor
        RenderElementRect();

        /// Destructor
        virtual ~RenderElementRect();

        /// Sets the size
        void                            SetSize( const CFoundation::Vector2Du& _vSize );

        /// Returns the size
        const CFoundation::Vector2Du&   GetSize() const;

        /// Sets the angle
        void                            SetAngle( Float16 _f16Angle );

        /// Returns the angle
        Float16                         GetAngle() const;

        /// Sets the mask to use
        void                            SetMask( const Texture& _Mask );

        /// Returns the mask to use
        const Texture&                  GetMask() const;

        /// Sets the color that should be us
        void                            SetColor( CFoundation::Color _Color );

        /// Returns the color that should be used
        CFoundation::Color              GetColor() const;

    private:   
        /// Size
        CFoundation::Vector2Du  m_vSize;

        /// Angle
        Float16                 m_f16Angle;

        /// Mask to use
        Texture                 m_Mask;

        /// Color to use
        CFoundation::Color      m_Color;
    };

    INCLUDE_SHARED_CLASS( RenderElementRect );
}

#endif

#endif
