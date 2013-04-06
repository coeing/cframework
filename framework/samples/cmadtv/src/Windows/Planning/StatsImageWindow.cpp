#include "cmadtv/Windows/Planning/StatsImageWindow.h"

#include "csystem/Input/Input.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/TextBox.h"

#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Player/Budget.h"

#include "cmadtv/Windows/Planning/StatsRatingsWindow.h"

using namespace CMadTV;
        
CFoundation::String StatsImageWindow::GetXMLFileName()
{
    return "data/windows/StatsImageWindow.xml";
}

StatsImageWindow::StatsImageWindow()
:   m_u32MaxBarHeight( 0 )
{
    m_spDelegateRatingsPushed = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &StatsImageWindow::OnRatingsPushed ) );
}

StatsImageWindow::~StatsImageWindow()
{
}

void StatsImageWindow::InitWindow()
{        
    // Get initial bar height
    CGui::PanelPtr spPnlBarPanel = GetPanel( "PnlBar1" );
    CGui::GuiObjectPtr spPnlBar = spPnlBarPanel != NULL ? spPnlBarPanel->FindChild( "PnlBar" ) : CGui::GuiObjectPtr();
    if ( spPnlBar != NULL )
    {
        m_u32MaxBarHeight = spPnlBar->GetHeight();
    }

    // Get player bars
    static const Unsigned32 MAX_BARS = 3;
    for ( Unsigned32 u32Idx = 0; u32Idx < MAX_BARS; ++u32Idx )
    {
        m_mapPlayerBars[ u32Idx ] = GetPanel( CFoundation::String::Build( "PnlBar%d", u32Idx + 1 ) );
    }

    m_spBtRatings = GetButton( "BtRatings" );
    m_spBtRatings->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateRatingsPushed );

    WindowController::InitWindow();
}
        
void StatsImageWindow::DeinitWindow()
{
    // Remove delegates
    m_spBtRatings->RemoveDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateRatingsPushed );

    WindowController::DeinitWindow();
}

void StatsImageWindow::ProcessInput( const CSystem::InputState& _State )
{
    if ( _State.WasPressed( CSystem::MOUSE_RIGHT ) ||
         _State.WasPressed( CSystem::KEY_ESC ) )
    {
        Close();
    }
}
        
void StatsImageWindow::OnContextChanged()
{
    SetGame( m_spContext != NULL ? m_spContext->GetGame() : GamePtr() );
}
                
void StatsImageWindow::SetGame( GamePtr _spGame )
{
    if ( _spGame == m_spGame )
    {
        return;
    }
    
    m_spGame = _spGame;

    UpdateStats();
}
        
void StatsImageWindow::UpdateStats()
{
    if ( m_spGame == NULL )
    {
        return;
    }

    // Set image values
    Unsigned32 u32NumBars = m_mapPlayerBars.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumBars; ++u32Idx )
    {
        PlayerPtr spPlayer = m_spGame->GetPlayer( u32Idx );
    
        // Get image
        Unsigned8 u8Image = spPlayer != NULL ? spPlayer->GetImage() : 0;
            
        // Get image panel
        CGui::PanelPtr spPnlBarPanel = m_mapPlayerBars[ u32Idx ];
        if ( spPnlBarPanel == NULL )
        {
            continue;
        }

        // Show bar
        spPnlBarPanel->SetVisible( u8Image > 0 );

        // Set bar and shadow height
        CGui::GuiObjectPtr spPnlBar = spPnlBarPanel->FindChild( "PnlBar" );
        spPnlBar->SetCustomHeight( m_u32MaxBarHeight * u8Image / 100 );
        CGui::GuiObjectPtr spImgShadow = spPnlBarPanel->FindChild( "ImgShadow" );
        spImgShadow->SetCustomHeight( m_u32MaxBarHeight * u8Image / 100 );

        // Set text
        CGui::TextBoxPtr spTbImage = static_pointer_cast< CGui::TextBox >( spPnlBarPanel->FindChild( "TbImage" ) );
        spTbImage->SetText( CFoundation::String::Build( "%d %%", u8Image ) );
    }
}
               
void StatsImageWindow::OnRatingsPushed()
{
    StatsRatingsWindowPtr spStatsRatings = Create< StatsRatingsWindow >( GetParent() );
    spStatsRatings->SetContext( m_spContext );

    Close();
}
     