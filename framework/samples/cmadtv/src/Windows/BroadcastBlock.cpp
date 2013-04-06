#include "cmadtv/Windows/BroadcastBlock.h"

#include "cfoundation/Debug/Makros.h"

#include "cgui/Gui/GuiMain.h"

#include "cmadtv/Data/Broadcast.h"

using namespace CMadTV;

BroadcastBlockPtr BroadcastBlock::Create( BroadcastPtr _spBroadcast )
{
    if ( _spBroadcast == NULL )
    {
        return BroadcastBlockPtr();
    }

    CFoundation::String strXML = "data/windows/BroadcastBlock.xml";

    // Create panel
    CGui::PanelPtr spPnlBlock = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( strXML );
    
    // Set panel height
    Unsigned32 u32NumBlocks = _spBroadcast->GetNumBlocks();
    spPnlBlock->SetCustomHeight( spPnlBlock->GetCustomHeight() * u32NumBlocks );

    // Create broadcast block
    BroadcastBlockPtr spBroadcastBlock( new BroadcastBlock( spPnlBlock, _spBroadcast ) );

    return spBroadcastBlock;
}

BroadcastBlock::BroadcastBlock( CGui::PanelPtr _spPnlRoot, BroadcastPtr _spBroadcast )
:   m_spBroadcast( _spBroadcast )
{
    // Store pointer to root panel
    m_spPnlRoot = _spPnlRoot;

    // Get background
    m_spImgBackground = static_pointer_cast< CGui::Image >( m_spPnlRoot->FindChild( "ImgBackground" ) );

    // Get title textbox
    m_spTbTitle = static_pointer_cast< CGui::TextBox >( m_spPnlRoot->FindChild( "TbTitle" ) );

    // Set title
    m_spTbTitle->SetText( _spBroadcast->GetTitle() );
}

BroadcastBlock::~BroadcastBlock()
{
    CGui::GuiMain::GetInstance().Delete( m_spPnlRoot );
}

BroadcastPtr BroadcastBlock::GetBroadcast() const
{
    return m_spBroadcast;
}

CGui::PanelPtr BroadcastBlock::GetPanel() const
{
    return m_spPnlRoot;
}
        
void BroadcastBlock::SetActive( bool _bActive )
{
    m_spImgBackground->SetActive( _bActive );
}
        
Unsigned32 BroadcastBlock::ComputeBlock( const CFoundation::Vector2Di& _vPosition ) const
{
    // Get y offset
    Integer32 i32PositionY = _vPosition.GetY();

    // Get height 
    Unsigned32 u32Height = m_spPnlRoot->GetSize().GetY();
    if ( i32PositionY < 0 ||
         static_cast< Unsigned32 >( i32PositionY ) > u32Height )
    {
        CF_WARN( "BroadcastBlock::ComputeBlock: Position outside the panel" );
        return m_spBroadcast->GetNumBlocks() - 1;
    }

    Unsigned32 u32NumBlocks = m_spBroadcast->GetNumBlocks();

    Unsigned32 u32Block = static_cast< Unsigned32 >( i32PositionY * 1.0f / u32Height * u32NumBlocks );
    return u32Block;
}

