#ifndef CGUI_DRAWER_H
#define CGUI_DRAWER_H

#include <vector>

#include "cfoundation/Utils/Color.h"
#include "cfoundation/Utils/Singleton.h"
#include "cfoundation/Math/Rect.h"
#include "cfoundation/Math/Vector.h"

namespace CGui
{
	//! A class to draw primitives with
	/*! With an instance of this class simple primitives
     *  can be rendered on the screen, like lines, circles,
     *  rectangles, and so on.
	 *
	 *	\author Christian Oeing
	 *
	 *	\date	27.12.08
	 *
	 */
    class Drawer : public CFoundation::Singleton< Drawer >
    {
    public:
        //! Renders a line
        void    DrawLine( const CFoundation::Vector2Df& _vStart, const CFoundation::Vector2Df& _vEnd, const CFoundation::Color& _clColor, Float32 _f32Thickness ) const;

        //! Renders the passed lines
        void    DrawLines( const std::vector< CFoundation::Vector2Df >& _vPoints, const CFoundation::Color& _clColor, Float32 _f32Thickness ) const;

        //! Renders a line strip
        void    DrawLineStrip( const std::vector< CFoundation::Vector2Df >& _vPoints, const CFoundation::Color& _clColor, Float32 _f32Thickness ) const;

        //! Renders a rectangle
        void    DrawRect( const CFoundation::RectF32& _clRect, const CFoundation::Color& _clColor, Float32 _f32Thickness ) const;

        //! Renders a rectangle
        void    DrawRect( const CFoundation::RectI32& _clRect, const CFoundation::Color& _clColor, Float32 _f32Thickness ) const;

        //! Renders a solid rectangle
        void    DrawSolidRect( const CFoundation::RectF32& _clRect, const CFoundation::Color& _clColor, Float16 _f16Angle = 0.0f ) const;

        //! Renders a solid rectangle
        void    DrawSolidRect( const CFoundation::RectI32& _clRect, const CFoundation::Color& _clColor, Float16 _f16Angle = 0.0f ) const;

        //! Draws an ellipse  
        void    DrawEllipse( const CFoundation::Vector2Di& _clPosition, Float32 _fRadiusX, Float32 _fRadiusY, const CFoundation::Color& _clColor );

    private:
        friend class CFoundation::Singleton< Drawer >;

        //! Constructor
        Drawer();
    };
}

#endif
