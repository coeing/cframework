#include "cgui/GUI/Drawer.h"

#include <algorithm>

#include "cfoundation/Debug/Assert.h"

#include "cgui/GUI/OpenGL.h"

using namespace CGui;

Drawer::Drawer()
{
}

void Drawer::DrawLine( const CFoundation::Vector2Df& _vStart, const CFoundation::Vector2Df& _vEnd, const CFoundation::Color& _clColor, Float32 _f32Thickness ) const
{
    Float16 f16R, f16G, f16B, f16A;
    _clColor.ToRGBA( f16R, f16G, f16B, f16A );
	glColor4f( f16R, f16G, f16B, f16A );

    glLineWidth( static_cast< GLfloat >( _f32Thickness ) );

	glBegin( GL_LINE_STRIP );
        glVertex3d( _vStart.GetX(), _vStart.GetY(), 0.0f );
        glVertex3d( _vEnd.GetX(), _vEnd.GetY(), 0.0f );
    glEnd();
}

void Drawer::DrawLines( const std::vector< CFoundation::Vector2Df >& _avPoints, const CFoundation::Color& _clColor, Float32 _f32Thickness ) const
{
    Float16 f16R, f16G, f16B, f16A;
    _clColor.ToRGBA( f16R, f16G, f16B, f16A );
	glColor4f( f16R, f16G, f16B, f16A );

    glLineWidth( static_cast< GLfloat >( _f32Thickness ) );

    CF_WARN_IF( _avPoints.size() % 2 != 0, "Drawer::DrawLines: Passed points are no multiple of two. Last point will not be drawn." );
	glBegin( GL_LINES );
    for ( std::vector< CFoundation::Vector2Df >::const_iterator it = _avPoints.begin(); it != _avPoints.end(); ++it )
    {
        glVertex3d( (*it).GetX(), (*it).GetY(), 0.0 );
    }
    glEnd();
}

void Drawer::DrawLineStrip( const std::vector< CFoundation::Vector2Df >& _avPoints, const CFoundation::Color& _clColor, Float32 _f32Thickness ) const
{
    Float16 f16R, f16G, f16B, f16A;
    _clColor.ToRGBA( f16R, f16G, f16B, f16A );
	glColor4f( f16R, f16G, f16B, f16A );

    glLineWidth( static_cast< GLfloat >( _f32Thickness ) );

    glBegin( GL_LINE_STRIP );
    for ( std::vector< CFoundation::Vector2Df >::const_iterator it = _avPoints.begin(); it != _avPoints.end(); ++it )
    {
        glVertex3d( (*it).GetX(), (*it).GetY(), 0.0 );
    }
    glEnd();
}

void Drawer::DrawRect( const CFoundation::RectI32& _clRect, const CFoundation::Color& _clColor, Float32 _f32Thickness ) const
{
    CFoundation::RectF32 clRect = _clRect;
    DrawRect( clRect, _clColor, _f32Thickness );
}

void Drawer::DrawRect( const CFoundation::RectF32& _clRect, const CFoundation::Color& _clColor, Float32 _f32Thickness ) const
{
    Float32 f32Thickness1 = std::max( 0.0, _f32Thickness / 2.0 + 0.5f );
    Float32 f32Thickness2 = std::max( 0.0, _f32Thickness / 2.0 + 0.5f );

    std::vector< CFoundation::Vector2Df > avPoints;

    // Top
    avPoints.push_back( CFoundation::Vector2Df( _clRect.GetLeft(), _clRect.GetTop() + f32Thickness1 ) );
    avPoints.push_back( CFoundation::Vector2Df( _clRect.GetRight(), _clRect.GetTop() + f32Thickness1 ) );
    // Right
    avPoints.push_back( CFoundation::Vector2Df( _clRect.GetRight() - f32Thickness2, _clRect.GetTop() ) );
    avPoints.push_back( CFoundation::Vector2Df( _clRect.GetRight() - f32Thickness2, _clRect.GetBottom() ) );
    // Bottom
    avPoints.push_back( CFoundation::Vector2Df( _clRect.GetRight(), _clRect.GetBottom() ) );
    avPoints.push_back( CFoundation::Vector2Df( _clRect.GetLeft(), _clRect.GetBottom() ) );
    // Left
    avPoints.push_back( CFoundation::Vector2Df( _clRect.GetLeft(), _clRect.GetBottom() ) );
    avPoints.push_back( CFoundation::Vector2Df( _clRect.GetLeft(), _clRect.GetTop() ) );

    DrawLines( avPoints, _clColor, _f32Thickness );
}
        
void Drawer::DrawSolidRect( const CFoundation::RectI32& _clRect, const CFoundation::Color& _clColor, Float16 _f16Angle ) const
{
    CFoundation::RectF32 clRect = _clRect;
    DrawSolidRect( clRect, _clColor, _f16Angle );
}

void Drawer::DrawSolidRect( const CFoundation::RectF32& _clRect, const CFoundation::Color& _clColor, Float16 _f16Angle ) const
{
    // Get position
    const CFoundation::Vector2Di& vPosition = _clRect.GetPosition();

    // Compute size
    const CFoundation::Vector2Du& vSize = _clRect.GetSize();
    Float16 f16HalfWidth = vSize.GetX() * 0.5f;
    Float16 f16HalfHeight = vSize.GetY() * 0.5f;

    // Translation
    glTranslatef( vPosition.GetX() + f16HalfWidth, vPosition.GetY() + f16HalfHeight, 0.0f );

    Float16 f16R, f16G, f16B, f16A;
    _clColor.ToRGBA( f16R, f16G, f16B, f16A );
	glColor4f( f16R, f16G, f16B, f16A );
    
    // Rotate
    glRotatef( _f16Angle, 0.0f, 0.0f, 1.0f );

    glBegin( GL_QUADS );
        glVertex3d( -f16HalfWidth, -f16HalfHeight, 0 );
	    glVertex3d(  f16HalfWidth, -f16HalfHeight, 0 );
	    glVertex3d(  f16HalfWidth,  f16HalfHeight, 0 );
	    glVertex3d( -f16HalfWidth,  f16HalfHeight, 0 );
    glEnd();
}

const Float32 DEG2RAD = PI / 180.0f;

void Drawer::DrawEllipse( const CFoundation::Vector2Di& _clPosition, Float32 _fRadiusX, Float32 _fRadiusY, const CFoundation::Color& _clColor )
{
    Float16 f16R, f16G, f16B, f16A;
    _clColor.ToRGBA( f16R, f16G, f16B, f16A );
    glColor4f( f16R, f16G, f16B, f16A );

    glBegin( GL_POLYGON );

    for ( Unsigned32 i = 0; i < 360; ++i )
    {
      //convert degrees into radians
      Float32 f32DegInRad = i * DEG2RAD;
      Float32 fX = _clPosition.GetX() + cos( f32DegInRad ) * _fRadiusX + _fRadiusX;
      Float32 fY = _clPosition.GetY() + sin( f32DegInRad ) * _fRadiusY + _fRadiusY;
      glVertex2d( fX, fY );
    }

    glEnd();
}