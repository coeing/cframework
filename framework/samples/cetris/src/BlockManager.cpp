#include "cetris/BlockManager.h"
#include <algorithm>

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Panel.h"

using namespace Cetris;

BlockManager::BlockManager()
{
}
        
void BlockManager::Init( CGui::ImagePtr _spImgBlockTemplate )
{
    m_spImgBlockTemplate = _spImgBlockTemplate;
    m_spPnlBlockContainer = m_spImgBlockTemplate->GetParent();
    m_spImgBlockTemplate->RemoveFromParent();
}
		
void BlockManager::Clear()
{
	m_pImgBlocks.clear();
	m_bFreeImg.clear();
}

void BlockManager::AddBlockImage( CGui::ImagePtr _spImgBlock )
{
	if( _spImgBlock != NULL )
	{
		_spImgBlock->SetVisible( false );
		_spImgBlock->SetBgVisible( false );
		m_pImgBlocks.insert( _spImgBlock );
		m_bFreeImg[ _spImgBlock ] = true;
	}
}

CGui::ImagePtr BlockManager::GetFreeImage()
{
	for( BlockImgSet::const_iterator it = m_pImgBlocks.begin(); it != m_pImgBlocks.end(); ++it )
	{
		if( m_bFreeImg[ *it ] )
		{
			m_bFreeImg[ *it ] = false;
			return *it;
		}
	}
    
    CGui::ImagePtr spImage = CGui::GuiMain::GetInstance().CreateFromTemplate( "ImgBlock", m_spImgBlockTemplate );
    m_spPnlBlockContainer->AddChildLast( spImage );
    return spImage;
}
	
void	BlockManager::FreeImage( CGui::ImagePtr _spImgBlock )
{
	if( _spImgBlock != NULL )
	{
		_spImgBlock->SetVisible( false );
		m_bFreeImg[ _spImgBlock ] = true;
	}
}

Block*	BlockManager::CreateBlock()
{
	Block *pBlock = NULL;

	CGui::ImagePtr spImgBlock = GetFreeImage();
	if( spImgBlock != NULL )
	{
		spImgBlock->SetVisible( true );
        spImgBlock->SetCustomSize( CFoundation::Vector2Du( Block::PIXEL_WIDTH, Block::PIXEL_HEIGHT ) );
		pBlock = new Block( spImgBlock );
		m_pBlocks.push_back( pBlock );
	}

	return pBlock;	
}

void    BlockManager::FreeBlock( Block *pBlock )
{
	BlockVec::iterator it = std::find( m_pBlocks.begin(), m_pBlocks.end(), pBlock );
	if( it != m_pBlocks.end() )
	{
		FreeImage( pBlock->m_spImgBlock );
		m_pBlocks.erase( it );
	}
	delete pBlock;
}

Figure*	BlockManager::CreateFigure( Figure::Type eType )
{
	Figure *pFigure = NULL;
	if( eType >= 0 &&
		eType < Figure::NUM_TYPES )
	{
		// Get 4 blocks
		Block *pBlocks[ 4 ];
		for( Unsigned8 u8Idx = 0; u8Idx < 4; ++u8Idx )
		{
			pBlocks[ u8Idx ] = CreateBlock();
		}

		pFigure = new Figure();
		pFigure->Init( pBlocks, eType );

		m_pFigures.push_back( pFigure );
	}

	return pFigure;
}


void    BlockManager::FreeFigure( Figure *pFigure )
{
	FigureVec::iterator it = std::find( m_pFigures.begin(), m_pFigures.end(), pFigure );
	if( it != m_pFigures.end() )
	{
		m_pFigures.erase( it );
	}
	delete pFigure;
}