#include "cetris/Block.h"

using namespace Cetris;

Block::Block( CGui::ImagePtr _spImgBlock )
:	m_spImgBlock( _spImgBlock )
{
}

void Block::SetPosition( Unsigned8 u8Col, Unsigned8 u8Row )
{
	m_clPosition.Set( u8Col, u8Row );
	if( m_spImgBlock != NULL )
	{
        m_spImgBlock->SetOffset( CFoundation::Vector2Di( u8Col * PIXEL_WIDTH, u8Row * PIXEL_HEIGHT ) );
	}
}
		
void Block::SetPosition( const CFoundation::Vector2Du8 &clPosition )
{
	SetPosition( clPosition.GetX(), clPosition.GetY() );
}
		
void Block::MoveDown( Unsigned8 u8Rows )
{
	m_clPosition.AddY( u8Rows );
	SetPosition( m_clPosition.GetX(), m_clPosition.GetY() );
}

Unsigned8 Block::GetCol() const
{
	return m_clPosition.GetX();
}

Unsigned8 Block::GetRow() const
{
	return m_clPosition.GetY();
}
		
void Block::SetVisible( bool bVisible )
{
	if ( m_spImgBlock != NULL )
	{
		m_spImgBlock->SetVisible( bVisible );
	}
}
		
bool Block::IsVisible() const
{
	if ( m_spImgBlock != NULL )
	{
		return m_spImgBlock->IsVisible();
	}
	else
	{
		return false;
	}
}