#ifndef CGUI_RENDERER_H
#define CGUI_RENDERER_H

#include <vector>

#include "cfoundation/Base/Types.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CGui
{
#ifdef MULTIPLE_RENDER_ELEMENTS
    INCLUDE_SHARED_CLASS( RenderElement );
#else
    class RenderElement;
#endif

    /** Renderer
     *
     *  Interface to render the interface elements.
	 * 
	 */
	class Renderer
    {
    public:
        /// Constructor
        Renderer();

        /// Destructor
        virtual ~Renderer();

        /// Sets the viewport size
        virtual void    SetViewportSize( Unsigned32 _u32Width, Unsigned32 _u32Height ) = 0;

        /// Called before rendering each frame
        virtual void    PreRender();

        /// Renders the passed render elements
#ifdef MULTIPLE_RENDER_ELEMENTS
        virtual void    Render( const std::vector< RenderElementPtr >& _aspElements ) = 0;
#else
        virtual void    Render( const std::vector< RenderElement >& _aElements ) = 0;
#endif

        /// Called after rendering each frame
        virtual void    PostRender();
    };
}

#endif
