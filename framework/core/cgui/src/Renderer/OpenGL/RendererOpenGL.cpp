#include "cgui/Renderer/OpenGL/RendererOpenGL.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Debug/Profiling.h"

#include "cgui/Font/Font.h"
#include "cgui/GUI/Drawer.h"
#include "cgui/GUI/OpenGL.h"
#include "cgui/Renderer/Elements/RenderElementPrimitives.h"
#include "cgui/Renderer/Elements/RenderElementRect.h"
#include "cgui/Renderer/Elements/RenderElementText.h"
#include "cgui/Renderer/Elements/RenderElementTexturedRect.h"

using namespace CGui;

///////////////////////////////////////////////////////////////////////////////
///
/// RendererOpenGL
///
///////////////////////////////////////////////////////////////////////////////

RendererOpenGL::RendererOpenGL()
{
}

RendererOpenGL::~RendererOpenGL()
{
}

void RendererOpenGL::SetViewportSize( Unsigned32 _u32Width, Unsigned32 _u32Height )
{
    // Setup viewport
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
    glViewport( 0, 0, _u32Width, _u32Height );
    //gluPerspective( 45.0f, _u32Width * 1.0f / _u32Height, 0.1f, 100.0f );
	glOrtho( 0, _u32Width, _u32Height, 0, 0, 1 );

	glMatrixMode( GL_MODELVIEW );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_FRONT );
	glLoadIdentity();

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	// Enable alpha blending
	glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glDisable( GL_ALPHA_TEST );
	glDisable( GL_DEPTH_TEST );		// Turn Depth Testing Off
    glDisable( GL_DITHER );

	// Textures
	/*glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );*/

	// Clipping on
	glEnable( GL_CLIP_PLANE0 );
	glEnable( GL_CLIP_PLANE1 );
	glEnable( GL_CLIP_PLANE2 );
	glEnable( GL_CLIP_PLANE3 );
}
        
void RendererOpenGL::PreRender()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
        
#ifdef MULTIPLE_RENDER_ELEMENTS
void RendererOpenGL::Render( const std::vector< RenderElementPtr >& _aspElements )
#else
void RendererOpenGL::Render( const std::vector< RenderElement >& _aspElements )
#endif
{
    Unsigned32 u32NumElements = _aspElements.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumElements; ++u32Idx )
    {
#ifdef MULTIPLE_RENDER_ELEMENTS
        RenderElementPtr spElement = _aspElements[ u32Idx ];
#else
        const RenderElement& spElement = _aspElements[ u32Idx ];
#endif

        // Set clipping rect
#ifdef MULTIPLE_RENDER_ELEMENTS
        const CFoundation::RectI32& rectClipping = spElement->GetClippingPlane();
#else
        const CFoundation::RectI32& rectClipping = spElement.GetClippingPlane();
#endif
        if ( rectClipping.GetWidth() == 0 ||
            rectClipping.GetHeight() == 0 )
        {
            continue;
        }
        SetClippingRect( rectClipping );
        
#ifdef MULTIPLE_RENDER_ELEMENTS
        // Check if text
        RenderElementTextPtr spElementText = dynamic_pointer_cast< RenderElementText >( spElement );
        if ( spElementText != NULL )
        {
            RenderText( spElementText );
            continue;
        }

        // Check if textured rect
        RenderElementTexturedRectPtr spElementTexturedRect = dynamic_pointer_cast< RenderElementTexturedRect >( spElement );
        if ( spElementTexturedRect != NULL )
        {
            RenderTexturedRect( spElementTexturedRect );
            continue;
        }

        // Check if normal rect
        RenderElementRectPtr spElementRect = dynamic_pointer_cast< RenderElementRect >( spElement );
        if ( spElementRect != NULL )
        {
            RenderRect( spElementRect );
            continue;
        }

        // Check if normal rect
        RenderElementPrimitivesPtr spPrimitives = dynamic_pointer_cast< RenderElementPrimitives >( spElement );
        if ( spPrimitives != NULL )
        {
            RenderPrimitives( spPrimitives );
            continue;
        }
#else
        switch ( spElement.GetType() )
        {
        case RenderElement::TYPE_RECT:
            {
                RenderRect( spElement );
            }
            break;
        case RenderElement::TYPE_TEXTURED_RECT:
            {
                RenderTexturedRect( spElement );
            }
            break;
        case RenderElement::TYPE_TEXT:
            {
                RenderText( spElement );
            }
            break;
        case RenderElement::TYPE_POINTS:
        case RenderElement::TYPE_LINES:
        case RenderElement::TYPE_TRIANGLES:
        case RenderElement::TYPE_QUADS:
            {
                RenderPrimitives( spElement );
            }
            break;
        }
#endif
    }
}

void RendererOpenGL::PostRender()
{
}

void RendererOpenGL::SetClippingRect( const CFoundation::RectI32& _rectClipping )
{
    glLoadIdentity();

	Float32 fWidth = _rectClipping.GetWidth();
	Float32 fHeight = _rectClipping.GetHeight();

	Float32 f32PlaneLeft[]		= { 1.0, 0.0, 0.0, -_rectClipping.GetX() };
	Float32 f32PlaneRight[]		= { -1.0, 0.0, 0.0, fWidth + _rectClipping.GetX() };
	Float32 f32PlaneTop[]		= { 0.0, 1.0, 0.0, -_rectClipping.GetY() };
	Float32 f32PlaneBottom[]	= { 0.0, -1.0, 0.0, fHeight + _rectClipping.GetY() };

	glClipPlane( GL_CLIP_PLANE0, f32PlaneLeft );
	glClipPlane( GL_CLIP_PLANE1, f32PlaneRight );
	glClipPlane( GL_CLIP_PLANE2, f32PlaneTop );
	glClipPlane( GL_CLIP_PLANE3, f32PlaneBottom );
}

#ifdef MULTIPLE_RENDER_ELEMENTS
void RendererOpenGL::RenderRect( RenderElementRectPtr _spRect )
#else
void RendererOpenGL::RenderRect( const RenderElement& _Rect )
#endif
{
    glLoadIdentity();
    glDisable( GL_TEXTURE_2D );
#ifdef MULTIPLE_RENDER_ELEMENTS
    Drawer::GetInstance().DrawSolidRect( CFoundation::RectI32( _spRect->GetPosition(), _spRect->GetSize() ), _spRect->GetColor(), _spRect->GetAngle() );
#else
    Drawer::GetInstance().DrawSolidRect( CFoundation::RectI32( _Rect.GetPosition(), _Rect.GetSize() ), _Rect.GetColor(), _Rect.GetAngle() );
#endif
}
        
#ifdef MULTIPLE_RENDER_ELEMENTS
void RendererOpenGL::RenderText( RenderElementTextPtr _spText )
#else
void RendererOpenGL::RenderText( const RenderElement& _Text )
#endif
{
#ifdef MULTIPLE_RENDER_ELEMENTS
    FontCPtr spFont = _spText->GetFont();
#else
    FontCPtr spFont = _Text.GetFont();
#endif
    if ( spFont == NULL )
    {
        CF_WARN( "No font set to render text." );
        return;
    }

#ifdef MULTIPLE_RENDER_ELEMENTS
    const CFoundation::String& strText = _spText->GetText();
#else
    const CFoundation::String& strText = _Text.GetText();
#endif
    if ( strText.GetLength() == 0 )
    {
        return;
    }

#ifdef MULTIPLE_RENDER_ELEMENTS
    const CFoundation::Vector2Di& vPosition = _spText->GetPosition();
    const CFoundation::Color& color = _spText->GetColor();
#else
    const CFoundation::Vector2Di& vPosition = _Text.GetPosition();
    const CFoundation::Color& color = _Text.GetColor();
#endif

    // Set text color
    Float16 f16R, f16G, f16B, f16A;
    color.ToRGBA( f16R, f16G, f16B, f16A );
    glColor4f( f16R, f16G, f16B, f16A );

    spFont->Activate();

    // Move to right position
    glLoadIdentity();
    glTranslated( vPosition.GetX(),
			      vPosition.GetY(),
			      0.0 );

    // Draw text
    glCallLists( strText.GetLength(), GL_UNSIGNED_SHORT, strText.wc_str() );
    spFont->Deactivate();
}

#ifdef MULTIPLE_RENDER_ELEMENTS
void RendererOpenGL::RenderTexturedRect( RenderElementTexturedRectPtr _spTexturedRect )
#else
void RendererOpenGL::RenderTexturedRect( const RenderElement& _TexturedRect )
#endif
{
    PROFILING( "Render Texture" );
    
    // Get settings
#ifdef MULTIPLE_RENDER_ELEMENTS
    const CFoundation::Vector2Di& vPosition = _spTexturedRect->GetPosition();
    const Texture& mask = _spTexturedRect->GetMask();
    const Texture& texture = _spTexturedRect->GetTexture();
    const CFoundation::RectF16& rectTexture = _spTexturedRect->GetRelativeTextureRect();
    const CFoundation::Vector2Du& vSize = _spTexturedRect->GetSize();
    bool bBgVisible = _spTexturedRect->IsBgVisible();
    const CFoundation::Color& bgColor = _spTexturedRect->GetBgColor();
    Float16 f16Angle = _spTexturedRect->GetAngle();
#else
    const CFoundation::Vector2Di& vPosition = _TexturedRect.GetPosition();
    const Texture& mask = _TexturedRect.GetMask();
    const Texture& texture = _TexturedRect.GetTexture();
    const CFoundation::RectF16& rectTexture = _TexturedRect.GetRelativeTextureRect();
    const CFoundation::Vector2Du& vSize = _TexturedRect.GetSize();
    bool bBgVisible = _TexturedRect.IsBgVisible();
    const CFoundation::Color& bgColor = _TexturedRect.GetBgColor();
    Float16 f16Angle = _TexturedRect.GetAngle();
#endif

    // Compute size
    Float16 f16HalfWidth = vSize.GetX() * 0.5f;
    Float16 f16HalfHeight = vSize.GetY() * 0.5f;
    Unsigned32 u32Width = vSize.GetX();
	Unsigned32 u32Height = vSize.GetY();

    // Render mask
    if ( mask.IsValid() )
    {
        // Enable stencil buffer
	    glClear( GL_STENCIL_BUFFER_BIT );
	    glEnable( GL_STENCIL_TEST );
	    glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE ); 
	    glStencilFunc( GL_ALWAYS, 1, 1 );

        // Enable mask
	    glEnable( GL_TEXTURE_2D );
	    glBindTexture( GL_TEXTURE_2D, mask.GetID() );

        // Draw only non-transparent pixels into the stencil buffer
	    glEnable( GL_ALPHA_TEST );
	    glAlphaFunc( GL_GREATER, 0.01f );
        
        // Draw nothing on the screen
	    glColorMask( 0, 0, 0, 0 );

        // Move to right position
	    glLoadIdentity();
	    glTranslated( vPosition.GetX(), vPosition.GetY(), 0 );   

        // Draw mask into stencil buffer
	    glBegin( GL_QUADS );

		    glTexCoord2f( rectTexture.GetLeft(), rectTexture.GetBottom() );     glVertex3d( 0, 0, 0 );
		    glTexCoord2f( rectTexture.GetRight(), rectTexture.GetBottom() );    glVertex3d( u32Width, 0, 0 );
		    glTexCoord2f( rectTexture.GetRight(), rectTexture.GetTop() );       glVertex3d( u32Width, u32Height, 0 );
		    glTexCoord2f( rectTexture.GetLeft(), rectTexture.GetTop() );        glVertex3d( 0, u32Height, 0 );

	    glEnd();

        // Draw again on the screen
	    glColorMask( 1, 1, 1, 1 );

        // Setup stencil test for background/texture
	    glStencilFunc( GL_EQUAL, 1, 1 );
	    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
    }

    // Render background
    if ( bBgVisible )
    {
        glLoadIdentity();
        glDisable( GL_TEXTURE_2D );
        Drawer::GetInstance().DrawSolidRect( CFoundation::RectI32( vPosition, vSize ), bgColor, f16Angle );
    }

    // Render texture	
	glLoadIdentity();
    glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, texture.GetID() );

    // Set color
    Float16 f16R, f16G, f16B, f16A;
    bgColor.ToRGBA( f16R, f16G, f16B, f16A );
    glColor4f( f16R, f16G, f16B, f16A );
	
    // Translation
    //glTranslated( vPosition.GetX(), vPosition.GetY(), 0.0f );
    glTranslatef( vPosition.GetX() + f16HalfWidth, vPosition.GetY() + f16HalfHeight, 0.0f );

    // Rotate
    glRotatef( f16Angle, 0.0f, 0.0f, 1.0f );

    // Render texture
	glBegin( GL_QUADS );

		glTexCoord2f( rectTexture.GetLeft(), rectTexture.GetBottom() );     glVertex3f( -f16HalfWidth,  -f16HalfHeight, 0 );
		glTexCoord2f( rectTexture.GetRight(), rectTexture.GetBottom() );    glVertex3f(  f16HalfWidth,  -f16HalfHeight, 0 );
		glTexCoord2f( rectTexture.GetRight(), rectTexture.GetTop() );       glVertex3f(  f16HalfWidth,   f16HalfHeight, 0 );
		glTexCoord2f( rectTexture.GetLeft(), rectTexture.GetTop() );        glVertex3f( -f16HalfWidth,   f16HalfHeight, 0 );

	glEnd();
    
    if ( mask.IsValid() )
    {
	    glClear( GL_STENCIL_BUFFER_BIT );
	    glDisable( GL_STENCIL_TEST );	
    }
}
      
#ifdef MULTIPLE_RENDER_ELEMENTS  
void RendererOpenGL::RenderPrimitives( RenderElementPrimitivesPtr _spPrimitives )
#else
void RendererOpenGL::RenderPrimitives( const RenderElement& _Primitives )
#endif
{
    PROFILING( "Render Primitives" );

#ifdef MULTIPLE_RENDER_ELEMENTS  
    RenderElementPrimitives::Type eType = _spPrimitives->GetType();
    const CFoundation::Color& color = _spPrimitives->GetColor();
    Float16 f16Thickness = _spPrimitives->GetThickness();
    const RenderElementPrimitives::PointVec& avPoints = _spPrimitives->GetPoints();
#else
    RenderElement::Type eType = _Primitives.GetType();
    const CFoundation::Color& color = _Primitives.GetColor();
    Float16 f16Thickness = _Primitives.GetThickness();
    const RenderElement::PointVec& avPoints = _Primitives.GetPoints();
#endif

    // Get opengl type
    Unsigned32 u32Type = 0;
    switch ( eType )
    {
#ifdef MULTIPLE_RENDER_ELEMENTS 
    case RenderElementPrimitives::TYPE_POINTS:
#else
    case RenderElement::TYPE_POINTS:
#endif
        {
            u32Type = GL_POINTS;
        }
        break;
#ifdef MULTIPLE_RENDER_ELEMENTS 
    case RenderElementPrimitives::TYPE_LINES:
#else
    case RenderElement::TYPE_LINES:
#endif
        {
            u32Type = GL_LINES;
        }
        break;
#ifdef MULTIPLE_RENDER_ELEMENTS 
    case RenderElementPrimitives::TYPE_TRIANGLES:
#else
    case RenderElement::TYPE_TRIANGLES:
#endif
        {
            u32Type = GL_TRIANGLES;
        }
        break;
#ifdef MULTIPLE_RENDER_ELEMENTS 
    case RenderElementPrimitives::TYPE_QUADS:
#else
    case RenderElement::TYPE_QUADS:
#endif
        {
            u32Type = GL_QUADS;
        }
        break;
    }

    // Set color
    Float16 f16R, f16G, f16B, f16A;
    color.ToRGBA( f16R, f16G, f16B, f16A );
    glColor4f( f16R, f16G, f16B, f16A );

    // Set line thickness
    glLineWidth( f16Thickness );

    // Render primitives
    glLoadIdentity();
    glDisable( GL_TEXTURE_2D );

	glBegin( u32Type );

    Unsigned32 u32NumPoints = avPoints.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPoints; ++u32Idx )
    {
        const CFoundation::Vector2Df& vPoint = avPoints[ u32Idx ];
        glVertex3d( vPoint.GetX(),  vPoint.GetY(), 0 );
    }

	glEnd();
}
