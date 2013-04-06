#include "cgui/Font/FontLoader.h"

#include <algorithm>

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "cfoundation/Debug/Assert.h"

#include "cgui/Font/Font.h"
#include "cgui/Gui/OpenGL.h"

using namespace CGui;

namespace CGui
{
	FT_Library gFtLibrary;

	// This Function Gets The First Power Of 2 >= The
	// Int That We Pass It.
	inline int next_p2 (int a )
	{
		int rval=1;
		// rval<<=1 Is A Prettier Way Of Writing rval*=2;
		while(rval<a) rval<<=1;
		return rval;
	}

	class FindFont
	{
	public:
		FindFont( const CFoundation::String &sFontName, Unsigned8 u8FontSize )
		:	m_sName( sFontName ),
			m_u8Size( u8FontSize )
		{
		}

		bool operator()( FontCPtr _spFont ) const
		{
			return _spFont->GetName() == m_sName && _spFont->GetSize() == m_u8Size;
		}

	private:
		CFoundation::String		m_sName;
		Unsigned8			m_u8Size;
	};
}

FontLoader::FontLoader()
{
	Integer32 i32Error = FT_Init_FreeType( &gFtLibrary );
	if( i32Error != 0 )
	{
		CF_WARN( L"FreeType Library could not be initialised." );
	}
}

FontLoader::~FontLoader()
{
	FT_Done_FreeType( gFtLibrary );
}

FontPtr FontLoader::LoadFontFromFile( const CFoundation::String &sFontName, Unsigned8 u8FontSize )
{
    FontPtr spFont;

	if( sFontName == "" ||
		u8FontSize == 0 )
	{
		return spFont;
	}

	// Check if this font was already created
	FontVec::iterator it = std::find_if( m_pCreatedFonts.begin(), m_pCreatedFonts.end(), FindFont( sFontName, u8FontSize ) );
	if ( it != m_pCreatedFonts.end() )
	{
		return *it;
	}

	FT_Face clFace;

	const char *szTmp = sFontName.c_str();
	Integer32 i32Error = FT_New_Face( gFtLibrary, szTmp, 0, &clFace );
	if( i32Error == FT_Err_Unknown_File_Format )
	{
		CF_WARN( L"FontLoader::LoadFontFromFile: Unknown File Format" );
		return spFont;
	}
	else if( i32Error )
	{
		CF_WARN( L"FontLoader::LoadFontFromFile: Error loading font %s with size %d", sFontName.wc_str(), u8FontSize );
		return spFont;
	}

	FT_Set_Pixel_Sizes( clFace, /* handle to face object */
						0, /* pixel_width */
						u8FontSize ); /* pixel_height */
	FT_Set_Char_Size( clFace, u8FontSize << 6, u8FontSize << 6, 96, 96);

	spFont = FontPtr( new Font( sFontName, u8FontSize ) );

	spFont->m_u32FontBase = glGenLists( Font::NUM_CHARS ); 		// Storage For 96 Characters ( NEW )

	glGenTextures( Font::NUM_CHARS, spFont->m_u32Textures );

	// This Is Where We Actually Create Each Of The Fonts Display Lists.
	for( wchar_t ch = 0; ch < Font::NUM_CHARS; ++ch )
	{
		CreateCharacter( clFace, ch, *spFont );
	}

	// We Don't Need The Face Information Now That The Display
	// Lists Have Been Created, So We Free The Assosiated Resources.
	FT_Done_Face( clFace );

	m_pCreatedFonts.push_back( spFont );

	return spFont;
}

void FontLoader::CreateCharacter( FT_FaceRec_ *pFace, wchar_t ch, Font &clFont )
{
	// The First Thing We Do Is Get FreeType To Render Our Character
	// Into A Bitmap.  This Actually Requires A Couple Of FreeType Commands:

	// Load The Glyph For Our Character.
	if( FT_Load_Glyph( pFace, FT_Get_Char_Index( pFace, ch ), FT_LOAD_RENDER ) )
	{
		//throw std::runtime_error("FT_Load_Glyph failed");
		return;
	}

	// Move The Face's Glyph Into A Glyph Object.
	FT_Glyph glyph;
	if( FT_Get_Glyph( pFace->glyph, &glyph ) )
	{
		//throw std::runtime_error("FT_Get_Glyph failed");
		return;
	}

	// Convert The Glyph To A Bitmap.
	FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_NORMAL, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	// This Reference Will Make Accessing The Bitmap Easier.
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;

	// Save char metrics
	FT_Glyph_Metrics *pMetrics = &pFace->glyph->metrics;
    CharMetrics& clMetrics = clFont.m_clCharMetrics[ ch ];
	CFoundation::RectF32 clBoundingBox( pMetrics->horiBearingX/64, -pMetrics->horiBearingY/64, pMetrics->horiAdvance/64, pMetrics->height/64 );
	clMetrics.SetBoundingBox( clBoundingBox );
    clMetrics.SetHoriBearingX( pMetrics->horiBearingX / 64.0f );
    clMetrics.SetHoriBearingY( pMetrics->horiBearingY / 64.0f );
    clMetrics.SetHoriAdvance( pMetrics->horiAdvance / 64.0f );
    clMetrics.SetVertBearingX( pMetrics->vertBearingX / 64.0f );
    clMetrics.SetVertBearingY( pMetrics->vertBearingY / 64.0f );
    clMetrics.SetVertAdvance( pMetrics->vertAdvance / 64.0f );

	// Use Our Helper Function To Get The Widths Of
	// The Bitmap Data That We Will Need In Order To Create
	// Our Texture.
	int width = next_p2( bitmap.width );
	int height = next_p2( bitmap.rows );

	// Allocate Memory For The Texture Data.
	GLubyte* expanded_data = new GLubyte[ 2 * width * height];

	// Here We Fill In The Data For The Expanded Bitmap.
	// Notice That We Are Using A Two Channel Bitmap (One For
	// Channel Luminosity And One For Alpha), But We Assign
	// Both Luminosity And Alpha To The Value That We
	// Find In The FreeType Bitmap.
	// We Use The ?: Operator To Say That Value Which We Use
	// Will Be 0 If We Are In The Padding Zone, And Whatever
	// Is The FreeType Bitmap Otherwise.
	for(int j=0; j <height;j++)
	{
		for(int i=0; i < width; i++)
		{
			expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] =
				(i>=bitmap.width || j>=bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width*j];
		}
	}

	// Now We Just Setup Some Texture Parameters.
	glBindTexture( GL_TEXTURE_2D, clFont.m_u32Textures[ ch ] );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	// Here We Actually Create The Texture Itself, Notice
	// That We Are Using GL_LUMINANCE_ALPHA To Indicate That
	// We Are Using 2 Channel Data.
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );

	// With The Texture Created, We Don't Need The Expanded Data Anymore.
	delete [] expanded_data;

	// Now We Create The Display List
	glNewList( clFont.m_u32FontBase + ch, GL_COMPILE );

	glBindTexture( GL_TEXTURE_2D, clFont.m_u32Textures[ ch ] );

	glPushMatrix();

	// First We Need To Move Over A Little So That
	// The Character Has The Right Amount Of Space
	// Between It And The One Before It.
	glTranslatef( static_cast<GLfloat>( pMetrics->horiBearingX/64.0 ), 0, 0 );

	// Now We Move Down A Little In The Case That The
	// Bitmap Extends Past The Bottom Of The Line
	// This Is Only True For Characters Like 'g' Or 'y'.
	glTranslatef( 0, static_cast<GLfloat>( -pMetrics->horiBearingY/64.0 ), 0 );

	// Now We Need To Account For The Fact That Many Of
	// Our Textures Are Filled With Empty Padding Space.
	// We Figure What Portion Of The Texture Is Used By
	// The Actual Character And Store That Information In
	// The x And y Variables, Then When We Draw The
	// Quad, We Will Only Reference The Parts Of The Texture
	// That Contains The Character Itself.
	Float32 f32X = pMetrics->width / 64.0f * 1.0f / width;
	Float32 f32Y = pMetrics->height / 64.0f * 1.0f / height;

	// Here We Draw The Texturemapped Quads.
	// The Bitmap That We Got From FreeType Was Not
	// Oriented Quite Like We Would Like It To Be,
	// But We Link The Texture To The Quad
	// In Such A Way That The Result Will Be Properly Aligned.
	glBegin( GL_QUADS );
	glTexCoord2d( 0, f32Y ); glVertex2f( 0, static_cast<GLfloat>( pMetrics->height / 64.0f ) );
	glTexCoord2d( 0, 0 ); glVertex2f( 0, 0 );
	glTexCoord2d( f32X, 0 ); glVertex2f( static_cast<GLfloat>( pMetrics->width / 64.0f ), 0 );
	glTexCoord2d( f32X, f32Y ); glVertex2f( static_cast<GLfloat>( pMetrics->width / 64.0f ), static_cast<GLfloat>( pMetrics->height / 64.0f ) );
	glEnd();

	glPopMatrix();
	glTranslatef( static_cast<GLfloat>( pMetrics->horiAdvance / 64.0f ), 0, 0 );

	// Finish The Display List
	glEndList();
}
