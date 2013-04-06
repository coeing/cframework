#ifndef CETRIS_BLOCKMANAGER_H
#define CETRIS_BLOCKMANAGER_H

#include <list>
#include <set>
#include <vector>

#include "cfoundation/Utils/Singleton.h"

#include "cgui/Gui/Image.h"

#include "cetris/Block.h"
#include "cetris/Figure.h"

namespace Cetris
{
	class BlockManager : public CFoundation::Singleton<BlockManager>
	{
	public:
		BlockManager();

        /// Initialization
        void        Init( CGui::ImagePtr _spImgBlockTemplate );

		void	Clear();

		void	AddBlockImage( CGui::ImagePtr _spImgBlock );

		void    FreeBlock( Block *pBlock );

		Figure*	CreateFigure( Figure::Type	eType );
		void    FreeFigure( Figure *pFigure );
		
	private:
		typedef std::set< CGui::ImagePtr > BlockImgSet;
		typedef std::map< CGui::ImagePtr, bool > ImageBoolMap;
		typedef std::list<Block*>			BlockVec;
		typedef std::list<Figure*>			FigureVec;

		CGui::ImagePtr  GetFreeImage();
		void	        FreeImage( CGui::ImagePtr _spImgBlock );

		Block*	CreateBlock();

        /// Template for a block object
        CGui::ImagePtr  m_spImgBlockTemplate;

        /// Block container
        CGui::PanelPtr  m_spPnlBlockContainer;

		BlockImgSet		m_pImgBlocks;

		ImageBoolMap	m_bFreeImg;

		BlockVec		m_pBlocks;

		FigureVec		m_pFigures;
	};
}

#endif
