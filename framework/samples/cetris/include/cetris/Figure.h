#ifndef CETRIS_FIGURE_H
#define CETRIS_FIGURE_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Math/Vector.h"

namespace Cetris
{
	class Block;
	class Figure
	{
	public:
		enum Type
		{
			TYPE_I,
			TYPE_J,
			TYPE_L,
			TYPE_O,
			TYPE_S,
			TYPE_T,
			TYPE_Z,
			NUM_TYPES
		};

		enum Direction
		{
			DIR_UP,
			DIR_RIGHT,
			DIR_DOWN,
			DIR_LEFT,
			NUM_DIRS
		};

		void RotateCW();
		void RotateCCW();

		void SetPosition( Unsigned8 u8PosX, Unsigned8 u8PosY );
		void SetPosition( const CFoundation::Vector2Du8 &clPos );

		void Move( Direction eDir, Unsigned8 u8Value );

		Unsigned8 GetRow() const;
		Unsigned8 GetCol() const;

		Unsigned8 GetNumRows() const;
		Unsigned8 GetNumCols() const;

		Block* GetBlock( Unsigned8 u8BlockNo );
		const Block* GetBlock( Unsigned8 u8BlockNo ) const;

	private:
		friend class BlockManager;

		Figure();
		void Init( Block *pBlocks[ 4 ], Type eType = TYPE_I );
		
		void UpdateBlockPositions();

		Type				m_eType;
		CFoundation::Vector2Df	m_clAxis;
		Block				*m_pBlocks[ 8 ];

		CFoundation::Vector2Di8	m_clPosition;
		Unsigned8			m_u8Dir;
	};
}

#endif
