#ifndef CGUI_RENDER_ELEMENT_PRIMITIVES_H
#define CGUI_RENDER_ELEMENT_PRIMITIVES_H

#include "cfoundation/Math/Vector.h"
#include "cfoundation/Utils/Color.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cgui/Renderer/Elements/RenderElement.h"

namespace CGui
{
#ifdef MULTIPLE_RENDER_ELEMENTS
    /** RenderElementPrimitives
     *
     *  Render element that holds the information to render primitives
	 * 
	 */
    class RenderElementPrimitives : public RenderElement
    {
    public:
        typedef std::vector< CFoundation::Vector2Df > PointVec;

        /// Type
        enum Type
        {
            TYPE_POINTS,
            TYPE_LINES,
            TYPE_TRIANGLES,
            TYPE_QUADS,

            TYPE_END
        };

        /// Constructor
        RenderElementPrimitives( Type _eType = TYPE_END );

        /// Destructor
        virtual ~RenderElementPrimitives();

        /// Sets the type
        void                    SetType( Type _eType );

        /// Returns the type
        Type                    GetType() const;

        /// Clears all points
        void                    ClearPoints();

        /// Adds a point
        void                    AddPoint( const CFoundation::Vector2Df& _vPoint );

        /// Returns all points
        const std::vector< CFoundation::Vector2Df > GetPoints() const;

        /// Sets the thickness of the lines
        void                    SetThickness( Float16 _16Thickness );

        /// Returns the thickness of the lines
        Float16                 GetThickness() const;

        /// Sets the color that should be us
        void                    SetColor( const CFoundation::Color& _Color );

        /// Returns the color that should be used
        const CFoundation::Color&   GetColor() const;

    private:   
        /// Type of primitive
        Type                    m_eType;

        /// Points
        PointVec                m_avPoints;

        /// Thickness of the lines
        Float16                 m_f16Thickness;

        /// Color to use
        CFoundation::Color      m_Color;
    };

    INCLUDE_SHARED_CLASS( RenderElementPrimitives );
#endif
}

#endif
