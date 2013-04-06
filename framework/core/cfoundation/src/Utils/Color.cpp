#include "cfoundation/Utils/Color.h"

#include <algorithm>

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Serialization/Serializer.h"

using namespace CFoundation;

Color::Color()
:	m_u32Color( 0 )
{
}

Color::Color( Unsigned32 u32HexColor )
:	m_u32Color( u32HexColor )
{
}

Color::Color( Float16 f16R, Float16 f16G, Float16 f16B, Float16 f16A )
:	m_u32Color( 0 )
{
	SetHex( RGBAToHex(	static_cast<Unsigned8>( f16R * MAX_U8 ),
						static_cast<Unsigned8>( f16G * MAX_U8 ),
						static_cast<Unsigned8>( f16B * MAX_U8 ),
						static_cast<Unsigned8>( f16A * MAX_U8 ) ) );
}

Color::Color( Unsigned8 u8R, Unsigned8 u8G, Unsigned8 u8B, Unsigned8 u8A )
:	m_u32Color( 0 )
{
	SetHex( RGBAToHex( u8R, u8G, u8B, u8A ) );
}

Color::Color( const Vector3Df &clColor )
:	m_u32Color( 0 )
{
	SetHex( RGBAToHex(	static_cast<Unsigned8>( clColor.GetX() * MAX_U8 ),
						static_cast<Unsigned8>( clColor.GetY() * MAX_U8 ),
						static_cast<Unsigned8>( clColor.GetZ() * MAX_U8 ),
						MAX_U8 ) );
}

Color::Color( const String& _sHexString )
:   m_u32Color( 0 )
{
    Unsigned32 u32Digits = 0;
    m_u32Color = _sHexString.HexToInteger( u32Digits );

    // Check if alpha channel is set (last two digits)
    if ( u32Digits <= 6 )
    {
        m_u32Color = ( m_u32Color << ( 8 - u32Digits ) * 4 ) + 0xff;
    }
    else if ( u32Digits == 7 )
    {
        m_u32Color = ( m_u32Color << 4 );
    }
}
       
void Color::Serialize( CFoundation::Serializer& _Serializer )
{
    _Serializer.Serialize( m_u32Color );
}

void Color::SetU8R( Unsigned8 _u8R )
{
    Unsigned8 u8R, u8G, u8B, u8A;
    ToRGBA( u8R, u8G, u8B, u8A );
    SetRGBA( _u8R, u8G, u8B, u8A );
}

void Color::SetU8G( Unsigned8 _u8G )
{
    Unsigned8 u8R, u8G, u8B, u8A;
    ToRGBA( u8R, u8G, u8B, u8A );
    SetRGBA( u8R, _u8G, u8B, u8A );
}

void Color::SetU8B( Unsigned8 _u8B )
{
    Unsigned8 u8R, u8G, u8B, u8A;
    ToRGBA( u8R, u8G, u8B, u8A );
    SetRGBA( u8R, u8G, _u8B, u8A );
}

void Color::SetU8A( Unsigned8 _u8A )
{
    Unsigned8 u8R, u8G, u8B, u8A;
    ToRGBA( u8R, u8G, u8B, u8A );
    SetRGBA( u8R, u8G, u8B, _u8A );
}
        
void Color::SetF16R( Float16 _f16R )
{
    Float16 f16R, f16G, f16B, f16A;
    ToRGBA( f16R, f16G, f16B, f16A );
    SetRGBA( _f16R, f16G, f16B, f16A );
}
        
void Color::SetF16G( Float16 _f16G )
{
    Float16 f16R, f16G, f16B, f16A;
    ToRGBA( f16R, f16G, f16B, f16A );
    SetRGBA( f16R, _f16G, f16B, f16A );
}
        
void Color::SetF16B( Float16 _f16B )
{
    Float16 f16R, f16G, f16B, f16A;
    ToRGBA( f16R, f16G, f16B, f16A );
    SetRGBA( f16R, f16G, _f16B, f16A );
}
        
void Color::SetF16A( Float16 _f16A )
{
    Float16 f16R, f16G, f16B, f16A;
    ToRGBA( f16R, f16G, f16B, f16A );
    SetRGBA( f16R, f16G, f16B, _f16A );
}

void Color::SetHex( Unsigned32 u32Color )
{
	m_u32Color = u32Color;
}

void Color::SetRGB( Float16 f16R, Float16 f16G, Float16 f16B )
{
	SetRGBA( f16R, f16G, f16B, GetF16A() );
}

void Color::SetRGB( Unsigned8 u8R, Unsigned8 u8G, Unsigned8 u8B )
{
    SetRGBA( u8R, u8G, u8B, GetU8A() );
}

void Color::SetRGBA( Float16 f16R, Float16 f16G, Float16 f16B, Float16 f16A )
{
	SetHex( RGBAToHex(	static_cast<Unsigned8>( f16R * MAX_U8 ),
						static_cast<Unsigned8>( f16G * MAX_U8 ),
						static_cast<Unsigned8>( f16B * MAX_U8 ),
						static_cast<Unsigned8>( f16A * MAX_U8 ) ) );
}

void Color::SetRGBA( Unsigned8 u8R, Unsigned8 u8G, Unsigned8 u8B, Unsigned8 u8A )
{
	SetHex( RGBAToHex( u8R, u8G, u8B, u8A ) );
}
        
void Color::SetHSV( Unsigned8 _u8H, Unsigned8 _u8S, Unsigned8 _u8V )
{
    // Make h in (0,6)
    Float16 f16Hi = _u8H / 60.0f;
    Unsigned8 u8Hi = static_cast< Unsigned8 >( f16Hi );
    // Get fractional
    Float16 f = f16Hi - u8Hi;
    
    // Compute helper variables
    Float16 v = _u8V / 100.0f;
    Float16 s = _u8S / 100.0f;
    Float16 p = v * (1 - s);
    Float16 q = v * (1 - (s * f));
    Float16 t = v * (1 - (s * (1 - f)));
    
    Float16 f16R = 0.0f, f16G = 0.0f, f16B = 0.0f;
    switch ( u8Hi )
	{
    case 0:
		f16R = v;
		f16G = t;
		f16B = p;
		break;
    case 1:
		f16R = q;
		f16G = v;
		f16B = p;
		break;
    case 2:
		f16R = p;
		f16G = v;
		f16B = t;
		break;
    case 3:
		f16R = p;
		f16G = q;
		f16B = v;
		break;
    case 4:
		f16R = t;
		f16G = p;
		f16B = v;
		break;
    case 5:
		f16R = v;
		f16G = p;
		f16B = q;
		break;
    }
    SetRGB( f16R, f16G, f16B );
}

void Color::ToRGB( Unsigned8 &u8R, Unsigned8 &u8G, Unsigned8 &u8B ) const
{
	Unsigned8 u8A = 0;
	ToRGBA( u8R, u8G, u8B, u8A );
}

void Color::ToRGB( Float16 &f16R, Float16 &f16G, Float16 &f16B ) const
{
	Float16 f16A = 0.0f;
	ToRGBA( f16R, f16G, f16B, f16A );
}

void Color::ToRGBA( Unsigned8 &u8R, Unsigned8 &u8G, Unsigned8 &u8B, Unsigned8& u8A ) const
{
	u8R = static_cast<Unsigned8>( ( m_u32Color & 0xff000000 ) >> 24 );
	u8G = static_cast<Unsigned8>( ( m_u32Color & 0x00ff0000 ) >> 16 );
	u8B = static_cast<Unsigned8>( ( m_u32Color & 0x0000ff00 ) >> 8 );
	u8A = static_cast<Unsigned8>( ( m_u32Color & 0x000000ff ) );
}

void Color::ToRGBA( Float16 &f16R, Float16 &f16G, Float16 &f16B, Float16& f16A ) const
{
	Unsigned8 u8R, u8G, u8B, u8A;
	ToRGBA( u8R, u8G, u8B, u8A );
	f16R = u8R * 1.0f / MAX_U8;
	f16G = u8G * 1.0f / MAX_U8;
	f16B = u8B * 1.0f / MAX_U8;
	f16A = u8A * 1.0f / MAX_U8;
}

Unsigned32 Color::ToHex() const
{
	return m_u32Color;
}
		
void Color::ToHSV( Unsigned8& _u8H, Unsigned8& _u8S, Unsigned8& _u8V ) const
{
	Unsigned8 u8R, u8G, u8B, u8A;
	ToRGBA( u8R, u8G, u8B, u8A );
    Unsigned8 u8Max = std::max( u8R, std::max( u8G, u8B ) );
    Unsigned8 u8Min = std::min( u8R, std::min( u8G, u8B ) );
    Unsigned8 u8Diff = u8Max - u8Min;

    // Compute H
    Float16 f16H = 0.0f;
    if ( u8Min == u8Max )
    {
        f16H = 0;
    }
    else if ( u8R == u8Max )
    {
        f16H = 60 * ( 0 + ( u8G - u8B ) * 1.0f / u8Diff );
    }
    else if ( u8G == u8Max )
    {
        f16H = 60 * ( 2 + ( u8B - u8R ) * 1.0f / u8Diff );
    }
    else if ( u8B == u8Max )
    {
        f16H = 60 * ( 4 + ( u8R - u8G ) * 1.0f / u8Diff );
    }
    if ( f16H < 0.0f )
    {
        f16H += 360.0f;
    }
    _u8H = static_cast< Unsigned8 >( f16H );

    // Compute S
    if ( u8Max == 0 )
    {
        _u8S = 0;
    }
    else
    {
        _u8S = u8Diff * 100.0f / u8Max;
    }

    // Compute V
    _u8V = u8Max * 100 / 255;
}

Unsigned32 Color::GetHex() const
{
	return m_u32Color;
}

const CFoundation::String Color::GetHexString() const
{
    CFoundation::String sTmp = CFoundation::String::ToHexString( m_u32Color );

	// add leading nils
	while( sTmp.GetLength() < 8 )
	{
		sTmp = "0" + sTmp;
	}

	return sTmp;
}

Unsigned8 Color::GetU8R() const
{
	Unsigned8 u8R, u8G, u8B;
	ToRGB( u8R, u8G, u8B );
	return u8R;
}

Unsigned8 Color::GetU8G() const
{
	Unsigned8 u8R, u8G, u8B;
	ToRGB( u8R, u8G, u8B );
	return u8G;
}

Unsigned8 Color::GetU8B() const
{
	Unsigned8 u8R, u8G, u8B;
	ToRGB( u8R, u8G, u8B );
	return u8B;
}

Unsigned8 Color::GetU8A() const
{
	Unsigned8 u8R, u8G, u8B, u8A;
	ToRGBA( u8R, u8G, u8B, u8A );
	return u8A;
}

Float16 Color::GetF16R() const
{
	Float16 f16R, f16G, f16B;
	ToRGB( f16R, f16G, f16B );
	return f16R;
}

Float16 Color::GetF16G() const
{
	Float16 f16R, f16G, f16B;
	ToRGB( f16R, f16G, f16B );
	return f16G;
}

Float16 Color::GetF16B() const
{
	Float16 f16R, f16G, f16B;
	ToRGB( f16R, f16G, f16B );
	return f16B;
}

Float16 Color::GetF16A() const
{
	Float16 f16R, f16G, f16B, f16A;
	ToRGBA( f16R, f16G, f16B, f16A );
	return f16A;
}

bool Color::operator==( const Color &rhs ) const
{
	return m_u32Color == rhs.m_u32Color;
}

bool Color::operator!=( const Color &rhs ) const
{
	return !( *this == rhs );
}

Color& Color::operator+=( const Color &rhs )
{
	Float16 f16R1, f16G1, f16B1;
	ToRGB( f16R1, f16G1, f16B1 );
	Float16 f16R2, f16G2, f16B2;
	rhs.ToRGB( f16R2, f16G2, f16B2 );

	SetRGB( std::min<Float16>( f16R1 + f16R2, 1.0f ),
			std::min<Float16>( f16G1 + f16G2, 1.0f ),
			std::min<Float16>( f16B1 + f16B2, 1.0f ) );

	return *this;
}

Color& Color::operator-=( const Color &rhs )
{
	Float16 f16R1, f16G1, f16B1;
	ToRGB( f16R1, f16G1, f16B1 );
	Float16 f16R2, f16G2, f16B2;
	rhs.ToRGB( f16R2, f16G2, f16B2 );

	SetRGB( std::max<Float16>( f16R1 - f16R2, 0.0f ),
			std::max<Float16>( f16G1 - f16G2, 0.0f ),
			std::max<Float16>( f16B1 - f16B2, 0.0f ) );

	return *this;
}

Color& Color::operator*=( Float16 d )
{
	Float16 f16R1, f16G1, f16B1;
	ToRGB( f16R1, f16G1, f16B1 );

	SetRGB( std::min<Float16>( f16R1 * d, 1.0f ),
			std::min<Float16>( f16G1 * d, 1.0f ),
			std::min<Float16>( f16B1 * d, 1.0f ) );

	return *this;
}

Color& Color::operator*=( const Color &rhs )
{
	Float16 f16R1, f16G1, f16B1;
	ToRGB( f16R1, f16G1, f16B1 );
	Float16 f16R2, f16G2, f16B2;
	rhs.ToRGB( f16R2, f16G2, f16B2 );

	SetRGB( std::min<Float16>( f16R1 * f16R2, 1.0f ),
			std::min<Float16>( f16G1 * f16G2, 1.0f ),
			std::min<Float16>( f16B1 * f16B2, 1.0f ) );

	return *this;
}

Color& Color::operator*=( const Vector3Df &rhs )
{
	Float16 f16R1, f16G1, f16B1;
	ToRGB( f16R1, f16G1, f16B1 );

	SetRGB( std::min<Float16>( f16R1 * static_cast<Float16>( rhs.GetX() ), 1.0f ),
			std::min<Float16>( f16G1 * static_cast<Float16>( rhs.GetY() ), 1.0f ),
			std::min<Float16>( f16B1 * static_cast<Float16>( rhs.GetZ() ), 1.0f ) );

	return *this;
}

Unsigned32 Color::RGBAToHex( Unsigned8 u8R, Unsigned8 u8G, Unsigned8 u8B, Unsigned8 u8A )
{
	return ( u8R << 24 ) + ( u8G << 16 ) + ( u8B << 8 ) + ( u8A );
}

void Color::Mix( const Color &clColor )
{
	Float16 f16R1, f16G1, f16B1;
	ToRGB( f16R1, f16G1, f16B1 );
	Float16 f16R2, f16G2, f16B2;
	clColor.ToRGB( f16R2, f16G2, f16B2 );

	SetRGB( f16R1 * 0.5f + f16R2 * 0.5f,
			f16G1 * 0.5f + f16G2 * 0.5f,
			f16B1 * 0.5f + f16B2 * 0.5f );
}

namespace CFoundation
{
	Color operator+( const Color &lhs, const Color &rhs )
	{
		return Color( lhs ) += rhs;
	}

	Color operator-( const Color &lhs, const Color &rhs )
	{
		return Color( lhs ) -= rhs;
	}

	Color operator*( const Color &lhs, Float16 d )
	{
		return Color( lhs ) *= d;
	}

	Color operator*( Float16 d, const Color &rhs )
	{
		return Color( rhs ) *= d;
	}

	Color operator*( const Color &lhs, const Color &rhs )
	{
		return Color( lhs ) *= rhs;
	}
}
