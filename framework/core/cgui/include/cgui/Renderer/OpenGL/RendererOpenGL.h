#ifndef CGUI_RENDERER_OPENGL_H
#define CGUI_RENDERER_OPENGL_H

#include "cfoundation/Math/Rect.h"

#include "cgui/Renderer/Renderer.h"

namespace CGui
{
#ifdef MULTIPLE_RENDER_ELEMENTS
    INCLUDE_SHARED_CLASS( RenderElementPrimitives );
    INCLUDE_SHARED_CLASS( RenderElementRect );
    INCLUDE_SHARED_CLASS( RenderElementText );
    INCLUDE_SHARED_CLASS( RenderElementTexturedRect );
#endif

    /** RendererOpenGL
     *
     *  Renders the interface elements with OpenGL
	 * 
	 */
    class RendererOpenGL : public Renderer
    {
    public:
        /// Constructor
        RendererOpenGL();

        /// Destructor
        virtual ~RendererOpenGL();

        /// Sets the viewport size
        virtual void    SetViewportSize( Unsigned32 _u32Width, Unsigned32 _u32Height );

        /// Called before rendering each frame
        virtual void    PreRender();

        /// Renders the passed render elements
#ifdef MULTIPLE_RENDER_ELEMENTS
        virtual void    Render( const std::vector< RenderElementPtr >& _aspElements );
#else
        virtual void    Render( const std::vector< RenderElement >& _aElements );
#endif

        /// Called after rendering each frame
        virtual void    PostRender();

    private:
        /// Sets the clipping plane
		void            SetClippingRect( const CFoundation::RectI32& _rectClipping );

        /// Renders an image
        //void            RenderImage( RenderElementImagePtr _spImage );

#ifdef MULTIPLE_RENDER_ELEMENTS
        /// Renders a one-colored rectangle
        void            RenderRect( RenderElementRectPtr _spRect );

        /// Renders a text
        void            RenderText( RenderElementTextPtr _spText );

        /// Renders a textured rect
        void            RenderTexturedRect( RenderElementTexturedRectPtr _spTexturedRect );

        /// Renders primitives
        void            RenderPrimitives( RenderElementPrimitivesPtr _spPrimitives );
#else
        /// Renders a one-colored rectangle
        void            RenderRect( const RenderElement& _Rect );

        /// Renders a text
        void            RenderText( const RenderElement& _Text );

        /// Renders a textured rect
        void            RenderTexturedRect( const RenderElement& _TexturedRect );

        /// Renders primitives
        void            RenderPrimitives( const RenderElement& _Primitives );
#endif
    };
}

#endif
