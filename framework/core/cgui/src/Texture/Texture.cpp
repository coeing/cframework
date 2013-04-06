#include "cgui/Texture/Texture.h"
#include "cgui/Texture/TextureLoader.h"

using namespace CGui;

Texture::Texture()
:	m_u32ID( 0 ),
	m_clSize( 0, 0 ),
	m_u16Bpp( 0 ),
	m_eTexType( TYPE_UNKNOWN ),
	m_eColorType( 0 )
{
}

bool Texture::operator==( const Texture& _rhs ) const
{
    return m_u32ID == _rhs.m_u32ID;
}

bool Texture::operator!=( const Texture& _rhs ) const
{
    return !( *this == _rhs );
}

void Texture::Clear()
{
	m_u32ID			= 0;
	m_clSize.Set( 0, 0 );
	m_u16Bpp        = 0;
	m_eTexType		= TYPE_UNKNOWN;
	m_eColorType	= 0;
}

bool Texture::IsValid() const
{
	return m_u32ID != 0;
}

Unsigned32 Texture::GetID() const
{
	return m_u32ID;
}

const CFoundation::String Texture::GetFileName() const
{
	if ( m_spParams != NULL )
	{
		return m_spParams->m_sFileName;
	}
	else
	{
		return "";
	}
}

Unsigned32 Texture::GetWidth() const
{
	return m_clSize.GetX();
}

Unsigned32 Texture::GetHeight() const
{
	return m_clSize.GetY();
}

const CFoundation::Vector2Du& Texture::GetSize() const
{
	return m_clSize;
}
