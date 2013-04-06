#ifndef CGUI_LAYER_H
#define CGUI_LAYER_H

#include "cfoundation/Math/Vector.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Color;
}

namespace CGui
{
    INCLUDE_SHARED_CLASS( GuiObject );
    INCLUDE_SHARED_CLASS( Panel );
    INCLUDE_SHARED_CLASS( RenderElement );

    class RenderParams;

    class Layer
    {
    public:
        //! Constructor
        Layer();

        //! Destructor
        ~Layer();

        /// Deinitialization
        void                    Deinit();

        //! Adds an object as first to this layer
		void                    AddObjectFirst( GuiObjectPtr _spObject );

        //! Adds an object as last to this layer
		void                    AddObjectLast( GuiObjectPtr _spObject );

        //! Adds an object after the passed object to this layer
		void                    AddObjectAfter( GuiObjectPtr _spObject, GuiObjectCPtr _spObjectBefore );

        //! Removes an object from this layer
		void                    RemoveObject( GuiObjectPtr _spObject );

        //! Returns the top object at the passed point
        /*!
         *  Fills clRelativePoint with the relative point
         *  on the found object.
         */
		GuiObjectPtr            GetObjectAtPoint( const CFoundation::Vector2Di& _clPoint, CFoundation::Vector2Di& _clRelativePoint );


        //! Sets the layer size
        void                    SetSize( const CFoundation::Vector2Du& _vSize );

        //! Sets the layer bg color
        void                    SetBgColor( const CFoundation::Color& _clColor );

		/// Per frame update
		void                    Update( Float32 _f32TimeDifference );

		//! Renders the objects of this layer
#ifdef MULTIPLE_RENDER_ELEMENTS
        void                    Render( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements );
#else
        void                    Render( const RenderParams& _Params, std::vector< RenderElement >& _aRenderElements );
#endif

    private:
        //! The root gui element
        PanelPtr                m_spRoot;
    };
}

#endif
