#include "cetris/Figure.h"

#include "cfoundation/Base/Defines.h"

#include "cetris/Block.h"

using namespace Cetris;

Figure::Figure()
:	m_eType( Figure::TYPE_I ),
	m_u8Dir( DIR_UP )
{
	for( Unsigned8 u8Idx = 0; u8Idx < 8; ++u8Idx )
	{
		m_pBlocks[ u8Idx ] = NULL;
	}
}

void Figure::Init( Block *pBlocks[ 4 ], Figure::Type eType )
{
	m_eType = eType;

	// clear old blocks
	for( Unsigned8 u8Idx = 0; u8Idx < 8; ++u8Idx )
	{
		m_pBlocks[ u8Idx ] = NULL;
	}

	// Set the 4 blocks to the right position
	switch( m_eType )
	{
	case TYPE_I:
		{
			m_pBlocks[ 4 ] = pBlocks[ 0 ];
			m_pBlocks[ 5 ] = pBlocks[ 1 ];
			m_pBlocks[ 6 ] = pBlocks[ 2 ];
			m_pBlocks[ 7 ] = pBlocks[ 3 ];
		}
		break;
	case TYPE_J:
		{
			m_pBlocks[ 0 ] = pBlocks[ 0 ];
			m_pBlocks[ 4 ] = pBlocks[ 1 ];
			m_pBlocks[ 5 ] = pBlocks[ 2 ];
			m_pBlocks[ 6 ] = pBlocks[ 3 ];
		}
		break;
	case TYPE_L:
		{
			m_pBlocks[ 2 ] = pBlocks[ 0 ];
			m_pBlocks[ 4 ] = pBlocks[ 1 ];
			m_pBlocks[ 5 ] = pBlocks[ 2 ];
			m_pBlocks[ 6 ] = pBlocks[ 3 ];
		}
		break;
	case TYPE_O:
		{
			m_pBlocks[ 1 ] = pBlocks[ 0 ];
			m_pBlocks[ 2 ] = pBlocks[ 1 ];
			m_pBlocks[ 5 ] = pBlocks[ 2 ];
			m_pBlocks[ 6 ] = pBlocks[ 3 ];
		}
		break;
	case TYPE_S:
		{
			m_pBlocks[ 1 ] = pBlocks[ 0 ];
			m_pBlocks[ 2 ] = pBlocks[ 1 ];
			m_pBlocks[ 4 ] = pBlocks[ 2 ];
			m_pBlocks[ 5 ] = pBlocks[ 3 ];
		}
		break;
	case TYPE_T:
		{
			m_pBlocks[ 1 ] = pBlocks[ 0 ];
			m_pBlocks[ 4 ] = pBlocks[ 1 ];
			m_pBlocks[ 5 ] = pBlocks[ 2 ];
			m_pBlocks[ 6 ] = pBlocks[ 3 ];
		}
		break;
	case TYPE_Z:
		{
			m_pBlocks[ 0 ] = pBlocks[ 0 ];
			m_pBlocks[ 1 ] = pBlocks[ 1 ];
			m_pBlocks[ 5 ] = pBlocks[ 2 ];
			m_pBlocks[ 6 ] = pBlocks[ 3 ];
		}
		break;
	}

	// set axis
	switch( m_eType )
	{
	case TYPE_O:
		{
			m_clAxis.Set( 2.0f, 1.0f );
		}
		break;
	case TYPE_I:
	case TYPE_J:
	case TYPE_L:
	case TYPE_S:
	case TYPE_T:
	case TYPE_Z:
		{
			m_clAxis.Set( 1.5f, 1.5f );
		}
		break;
	}
}

void Figure::SetPosition( Unsigned8 u8PosX, Unsigned8 u8PosY )
{
	m_clPosition.Set( u8PosX, u8PosY );

	UpdateBlockPositions();
}

void Figure::SetPosition( const CFoundation::Vector2Du8 &clPos )
{
	SetPosition( clPos.GetX(), clPos.GetY() );
}
		
void Figure::Move( Figure::Direction eDir, Unsigned8 u8Value )
{
	switch( eDir )
	{
	case DIR_UP:
		{
			m_clPosition.SubY( u8Value );
		}
		break;
	case DIR_RIGHT:
		{
			m_clPosition.AddX( u8Value );
		}
		break;
	case DIR_DOWN:
		{
			m_clPosition.AddY( u8Value );
		}
		break;
	case DIR_LEFT:
		{
			m_clPosition.SubX( u8Value );
		}
		break;
	}
	UpdateBlockPositions();
}
		
Unsigned8 Figure::GetRow() const
{
	return m_clPosition.GetY();
}

Unsigned8 Figure::GetCol() const
{
	return m_clPosition.GetX();
}

Unsigned8 Figure::GetNumRows() const
{
	Integer8 i8MaxRow = -1;
	for( Unsigned8 u8Idx = 0; u8Idx < 8; ++u8Idx )
	{
		Block *pBlock = m_pBlocks[ u8Idx ];
		if( pBlock != NULL &&
			( u8Idx / 4 ) > i8MaxRow )
		{
			i8MaxRow = ( u8Idx / 4 );
		}
	}
	
	return static_cast<Unsigned8>( i8MaxRow + 1 );
}

Unsigned8 Figure::GetNumCols() const
{
	Integer8 i8MaxCol = -1;
	for( Unsigned8 u8Idx = 0; u8Idx < 8; ++u8Idx )
	{
		Block *pBlock = m_pBlocks[ u8Idx ];
		if( pBlock != NULL &&
			( u8Idx % 4 ) > i8MaxCol )
		{
			i8MaxCol = ( u8Idx % 4 );
		}
	}
	
	return static_cast<Unsigned8>( i8MaxCol + 1 );
}

Block* Figure::GetBlock( Unsigned8 u8BlockNo )
{
	Unsigned8 u8FoundBlocks = 0;
	for( Unsigned8 u8Idx = 0; u8Idx < 8; ++u8Idx )
	{
		Block *pBlock = m_pBlocks[ u8Idx ];
		if( pBlock != NULL )
		{
			if( u8FoundBlocks == u8BlockNo )
			{
				return pBlock;
			}
			++u8FoundBlocks;
		}
	}

	return NULL;
}

const Block* Figure::GetBlock( Unsigned8 u8BlockNo ) const
{
	Figure *pNotConstThis = const_cast<Figure*>( this );
	Block *pBlock = pNotConstThis->GetBlock( u8BlockNo );
	return pBlock;
}

void Figure::RotateCW()
{
	if( m_u8Dir < NUM_DIRS - 1 )
	{
		++m_u8Dir;
	}
	else
	{
		m_u8Dir = 0;
	}
	UpdateBlockPositions();
}

void Figure::RotateCCW()
{
	if( m_u8Dir > 0 )
	{
		--m_u8Dir;
	}
	else
	{ 
		m_u8Dir = NUM_DIRS - 1;
	}
	UpdateBlockPositions();
}
		
void Figure::UpdateBlockPositions()
{
	Integer8 i8PosX = m_clPosition.GetX();
	Integer8 i8PosY = m_clPosition.GetY();

	// set rotation
	Integer8 i8Sin = 0;
	Integer8 i8Cos = 0;
	switch( m_u8Dir )
	{
	case DIR_UP:
		{
			i8Sin = 0;
			i8Cos = 1;
		}
		break;
	case DIR_RIGHT:
		{
			i8Sin = 1;
			i8Cos = 0;
		}
		break;
	case DIR_DOWN:
		{
			i8Sin = 0;
			i8Cos = -1;
		}
		break;
	case DIR_LEFT:
		{
			i8Sin = -1;
			i8Cos = 0;
		}
		break;
	}

	// set block positions
	for( Unsigned8 u8Idx = 0; u8Idx < 8; ++u8Idx )
	{
		Block *pBlock = m_pBlocks[ u8Idx ];
		if( pBlock != NULL )
		{
			CFoundation::Vector2Df clTmpPos( u8Idx % 4 - m_clAxis.GetX() + 0.5f, u8Idx / 4 - m_clAxis.GetY() + 0.5f );

			// Do rotation
			CFoundation::Vector2Df clPosition;
			clPosition.SetX( clTmpPos.GetX() * i8Cos + clTmpPos.GetY() * -i8Sin );
			clPosition.SetY( clTmpPos.GetX() * i8Sin + clTmpPos.GetY() * i8Cos );

            Unsigned8 u8Col = static_cast< Unsigned8 >( i8PosX + clPosition.GetX() + m_clAxis.GetX() - 0.5f );
            Unsigned8 u8Row = static_cast< Unsigned8 >( i8PosY + clPosition.GetY() + m_clAxis.GetX() - 0.5f );
			pBlock->SetPosition( u8Col, u8Row );
		}
	}
}