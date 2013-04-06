#ifndef CETRIS_BLOCK_H
#define CETRIS_BLOCK_H

#include "cgui/Gui/Image.h"

namespace Cetris
{
	class Block
	{
	public:
		static const Unsigned8	PIXEL_WIDTH = 20;
		static const Unsigned8  PIXEL_HEIGHT = 20;

		void SetPosition( Unsigned8 u8Col, Unsigned8 u8Row );
		void SetPosition( const CFoundation::Vector2Du8 &clPosition );
		void MoveDown( Unsigned8 u8Rows );
		Unsigned8 GetCol() const;
		Unsigned8 GetRow() const;

		void SetVisible( bool bVisible );
		bool IsVisible() const;

	private:
		friend class BlockManager;

		Block( CGui::ImagePtr _spImgBlock );

		// Data	
		CFoundation::Vector2Di8	m_clPosition;

		// Controls
		CGui::ImagePtr      m_spImgBlock;
	};
}

#endif
