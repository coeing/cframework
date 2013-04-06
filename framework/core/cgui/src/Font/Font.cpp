#include "cgui/Font/Font.h"

#include "cgui/Font/CharMetrics.h"
#include "cgui/Gui/OpenGL.h"

using namespace CGui;

Font::Font()
:	m_sName( "" ),
	m_u8Size( 20 ),
	m_i8Height( -1 ),
	m_i8Depth( -1 ),
	m_u32FontBase( 0 )
{
}

Font::Font( const CFoundation::String &sFontName, Unsigned8 u8FontSize )
:	m_sName( sFontName ),
	m_u8Size( u8FontSize ),
	m_i8Height( -1 ),
	m_i8Depth( -1 ),
	m_u32FontBase( 0 )
{
}

Font::~Font()
{
}

const CFoundation::String& Font::GetName() const
{
	return m_sName;
}

Unsigned8 Font::GetSize() const
{
	return m_u8Size;
}

Unsigned8 Font::GetHeight() const
{
	if( m_i8Height == -1 )
	{
		CalculateHeightDepth();
	}

	return m_i8Height;
}

Unsigned8 Font::GetDepth() const
{
	if( m_i8Depth == -1 )
	{
		CalculateHeightDepth();
	}

	return m_i8Depth;
}

/// A fairly straight forward function that pushes
/// a projection matrix that will make object world
/// coordinates identical to window coordinates.
inline void pushScreenCoordinateMatrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint	viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	//glLoadIdentity();
	//gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
	glPopAttrib();
}

/// Pops the projection matrix without changing the current
/// MatrixMode.
inline void pop_projection_matrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

void Font::Activate() const
{
    /*
	// We want a coordinate system where things coresponding to window pixels.
	pushScreenCoordinateMatrix();

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
*/
	glEnable( GL_TEXTURE_2D );
	glListBase( m_u32FontBase ); 	// Sets The Base Character to 32
}

void Font::Deactivate() const
{

	//glPopAttrib();

	//pop_projection_matrix();
}

CFoundation::RectF32 Font::GetBoundingBox( const CFoundation::String& _strText ) const
{
	CFoundation::RectF32 clBoundingBox;
	clBoundingBox.SetPosition( 0, 0 );
	clBoundingBox.SetSize( GetSize( _strText ) );

	return clBoundingBox;
}
       
CFoundation::Vector2Df Font::GetSize( const CFoundation::String& _strText ) const
{
	Float32 f32Width = 0;
	Unsigned32 u32Height = GetHeight() + GetDepth();
    Unsigned32 u32NumChars = _strText.GetLength();
	for( Unsigned32 u32Idx = 0; u32Idx < u32NumChars; ++u32Idx )
	{
		wchar_t ch = _strText[ u32Idx ];
		/*if( ch < 0 ||
			ch >= NUM_CHARS )
		{
			continue;
		}*/

		const CharMetrics &clMetrics = m_clCharMetrics[ ch ];
		const CFoundation::RectF32 &clCharBB = clMetrics.GetBoundingBox();
		f32Width += clCharBB.GetWidth();
	}

    return CFoundation::Vector2Df( f32Width, u32Height );
}
       
Float32 Font::GetPosition( const CFoundation::String& _strText, Float32 _f32Offset ) const
{
    if ( _f32Offset <= 0.0f )
    {
        return 0;
    }

	Float32 f32Offset = 0;
    Unsigned32 u32NumChars = _strText.GetLength();
	for( Unsigned32 u32Idx = 0; u32Idx < u32NumChars; ++u32Idx )
	{
		wchar_t ch = _strText[ u32Idx ];
		/*if( ch < 0 ||
			ch >= NUM_CHARS )
		{
			continue;
		}*/

		const CharMetrics& clMetrics = m_clCharMetrics[ ch ];
		const CFoundation::RectF32& rectCharBB = clMetrics.GetBoundingBox();
        Float32 f32CharWidth = rectCharBB.GetWidth();

        // Check if reached offset
        if ( _f32Offset < f32Offset + f32CharWidth )
        {
            // Compute character position
            Float32 f32CharPosition = ( _f32Offset - f32Offset ) / f32CharWidth;
            return u32Idx + f32CharPosition;
        }

		f32Offset += f32CharWidth;
	}

    return u32NumChars;
}

void Font::CalculateHeightDepth() const
{
	m_i8Height = -1;
	m_i8Depth = -1;
	for( Unsigned32 u32Idx = 0; u32Idx < NUM_CHARS; ++u32Idx )
	{
		const CharMetrics &clMetrics = m_clCharMetrics[ u32Idx ];
		const CFoundation::RectF32 &clCharBB = clMetrics.GetBoundingBox();
        Unsigned8 u8HoriBearingY = static_cast< Unsigned8 >( abs( clMetrics.GetHoriBearingY() ) );
		Unsigned8 u8Depth = static_cast< Unsigned8 >( clCharBB.GetHeight() - u8HoriBearingY );
		if( u8HoriBearingY > m_i8Height )
		{
			m_i8Height = u8HoriBearingY;
		}
		if( u8Depth > m_i8Depth )
		{
			m_i8Depth = u8Depth;
		}
	}
}
