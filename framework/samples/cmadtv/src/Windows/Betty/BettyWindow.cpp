#include "cmadtv/Windows/Betty/BettyWindow.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/ProgressBar.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Betty/Present.h"
#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Game/Sammy/Sammy.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerCompBetty.h"
#include "cmadtv/Data/Player/PlayerContext.h"

#include "cmadtv/DragDrop/PresentDragController.h"
#include "cmadtv/DragDrop/PresentDropController.h"
#include "cmadtv/Tooltip/SammyTooltipController.h"
#include "cmadtv/Windows/Supermarket/PresentObject.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BettyWindow
///
////////////////////////////////////////////////////////////////////////////////////////////
        
CFoundation::String BettyWindow::GetXMLFileName()
{
    return "data/windows/BettyWindow.xml";
}

BettyWindow::BettyWindow()
:   m_i32InitialPictureOffset( 0 )
{
    // Create delegates
    m_spDelegateBettyPresentChanged = PlayerCompBettyEventPresentDelegate1Ptr( new PlayerCompBettyEventPresentDelegate1( this, &BettyWindow::OnBettyPresentChanged ) );
    m_spDelegateBettyLoveChanged = PlayerCompBettyEventU8U8Delegate0Ptr( new PlayerCompBettyEventU8U8Delegate0( this, &BettyWindow::OnBettyLoveChanged ) );
    m_spDelegateSammyPointsChanged = SammyEventPlayerDelegate0Ptr( new SammyEventPlayerDelegate0( this, &BettyWindow::OnSammyPointsChanged ) );
    
    m_spBettyDropController = PresentDropControllerPtr( new PresentDropController( PresentDropController::TYPE_BETTY ) );
    m_spSammyTooltipController = SammyTooltipControllerPtr( new SammyTooltipController() );
}

BettyWindow::~BettyWindow()
{
}

void BettyWindow::InitWindow()
{
    // Init bag
    CGui::PanelPtr spPnlBag = GetPanel( "PnlBag" );
    m_spObjBag = spPnlBag;
    m_spBagPresentObject = PresentObjectPtr( new PresentObject() );
    m_spBagPresentObject->Init( static_pointer_cast< CGui::Panel >( spPnlBag->FindChild( "ObjPresent" ) ) );
    m_spBagPresentObject->SetVisibleDuringDrag( false );

    // Init sammy
    m_spPnlSammy = GetPanel( "PnlSammy" );
    m_spPnlSammy->SetVisible( false );
    CGui::TooltipSystem::GetInstance().SetTooltipController( m_spPnlSammy, m_spSammyTooltipController );

    // Init betty
    CGui::GuiObjectPtr spObjBetty = GetGuiObject( "ObjBetty" );
    CGui::DragDropSystem::GetInstance().SetDropController( spObjBetty, m_spBettyDropController );

    for ( Unsigned32 u32Idx = 0; u32Idx < MAX_PLAYERS; ++u32Idx )
    {
        // Get pictures
        CGui::PanelPtr spPnlPicture = GetPanel( CFoundation::String::Build( "PnlPicture%d", u32Idx + 1 ) );
        m_aspPictures.push_back( spPnlPicture );

        // Get initial picture offset
        if ( spPnlPicture != NULL )
        {
            m_i32InitialPictureOffset = spPnlPicture->GetOffset().GetY();
        }

        // Get sammy point bars
        CGui::ProgressBarPtr spPbSammyPoints = GetProgressBar( CFoundation::String::Build( "PbPoints%d", u32Idx + 1 ) );
        m_aspPbSammyPoints.push_back( spPbSammyPoints );
    }

    WindowController::InitWindow();
}
		
void BettyWindow::DeinitWindow()
{
    // Deinit betty
    CGui::GuiObjectPtr spObjBetty = GetGuiObject( "ObjBetty" );
    CGui::DragDropSystem::GetInstance().RemoveDropController( spObjBetty );

    WindowController::DeinitWindow();
}
        
void BettyWindow::OnContextChanged()
{
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );
    SetGame( m_spContext != NULL ? m_spContext->GetGame() : GamePtr() );
}
        
void BettyWindow::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    if ( m_spPlayer != NULL )
    {
        // Remove delegate
        PlayerCompBettyPtr spCompBetty = m_spPlayer->GetCompBetty();
        spCompBetty->RemoveDelegate( EVENT_BETTY_PRESENT_CHANGED, m_spDelegateBettyPresentChanged );

        SetBagPresent( PresentPtr() );
    }

    m_spPlayer = _spPlayer;

    // Setup drop controllers
    m_spBettyDropController->SetPlayer( m_spPlayer );

    m_spBagPresentObject->SetPlayer( m_spPlayer );

    if ( m_spPlayer != NULL )
    {
        // Register delegate
        PlayerCompBettyPtr spCompBetty = m_spPlayer->GetCompBetty();
        spCompBetty->RegisterDelegate( EVENT_BETTY_PRESENT_CHANGED, m_spDelegateBettyPresentChanged );

        // Init bag
        SetBagPresent( spCompBetty->GetPresent() );
    }
}

void BettyWindow::SetGame( GamePtr _spGame )
{
    if ( _spGame == m_spGame )
    {
        return;
    }

    if ( m_spGame != NULL )
    {
        Unsigned32 u32NumPlayers = m_spGame->GetNumPlayers();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
        {
            PlayerPtr spPlayer = m_spGame->GetPlayer( u32Idx );

            // Register delegate
            PlayerCompBettyPtr spCompBetty = spPlayer->GetCompBetty();
            spCompBetty->RemoveDelegate( EVENT_BETTY_LOVE_CHANGED, m_spDelegateBettyLoveChanged );
        }
    }

    m_spGame = _spGame;

    if ( m_spGame != NULL )
    {
        Unsigned32 u32NumPlayers = m_spGame->GetNumPlayers();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPlayers; ++u32Idx )
        {
            PlayerPtr spPlayer = m_spGame->GetPlayer( u32Idx );

            // Register delegate
            PlayerCompBettyPtr spCompBetty = spPlayer->GetCompBetty();
            spCompBetty->RegisterDelegate( EVENT_BETTY_LOVE_CHANGED, m_spDelegateBettyLoveChanged );
        }
    }

    // Update pictures
    UpdatePictures();

    // Set sammy
    SetSammy( m_spGame != NULL ? m_spGame->GetSammy() : SammyPtr() );
}

void BettyWindow::SetSammy( SammyPtr _spSammy )
{
    if ( _spSammy == m_spSammy )
    {
        return;
    }

    if ( m_spSammy != NULL )
    {
        // Remove delegate
        m_spSammy->RemoveDelegate( EVENT_SAMMY_POINTS_CHANGED, m_spDelegateSammyPointsChanged );
    }

    m_spSammy = _spSammy;

    // Setup controllers
    m_spSammyTooltipController->SetSammy( m_spSammy );

    if ( m_spSammy != NULL )
    {
        // Register delegate
        m_spSammy->RegisterDelegate( EVENT_SAMMY_POINTS_CHANGED, m_spDelegateSammyPointsChanged );
    }

    // Update sammy
    UpdateSammy();
}
        
void BettyWindow::UpdateSammy()
{
    // Set sammy visible
    m_spPnlSammy->SetVisible( m_spSammy != NULL );
    
    if ( m_spSammy == NULL )
    {
        return;
    }

    // Get summed points
    Unsigned32 u32SummedPoints = m_spSammy->GetSummedPoints();

    // Setup point bars
    for ( Unsigned32 u32Idx = 0; u32Idx < MAX_PLAYERS; ++u32Idx )
    {
        CGui::ProgressBarPtr spPbPoints = m_aspPbSammyPoints[ u32Idx ];
        if ( spPbPoints == NULL )
        {
            continue;
        }

        // Get points
        Unsigned32 u32Points = m_spSammy->GetPoints( u32Idx );

        // Set bar ratio
        spPbPoints->SetRatio( u32Points * 1.0f / u32SummedPoints );
    }
}
              
void BettyWindow::SetBagPresent( PresentPtr _spPresent )
{
    m_spObjBag->SetVisible( _spPresent != NULL );
    m_spBagPresentObject->SetPresent( _spPresent );
}
      
void BettyWindow::UpdatePictures()
{
    for ( Unsigned32 u32Idx = 0; u32Idx < MAX_PLAYERS; ++u32Idx )
    {
        CGui::PanelPtr spPnlPicture = m_aspPictures[ u32Idx ];
        if ( spPnlPicture == NULL )
        {
            continue;
        }

        // Get player
        PlayerPtr spPlayer = m_spGame != NULL ? m_spGame->GetPlayer( u32Idx ) : PlayerPtr();
        spPnlPicture->SetVisible( spPlayer != NULL );
        if ( spPlayer != NULL )
        {
            // Get love
            PlayerCompBettyPtr spCompBetty = spPlayer->GetCompBetty();
            Unsigned8 u8Love = spCompBetty->GetLove();

            CGui::GuiObjectPtr spImgPicture = spPnlPicture->FindChild( "ImgPicture" );
            SAFE_CALL( spImgPicture )->SetVisible( u8Love != 0 );

            // Set y offset
            spPnlPicture->SetOffsetY( m_i32InitialPictureOffset - u8Love );
        }
    }
}
        
void BettyWindow::OnBettyPresentChanged( PresentPtr _spPresent )
{
    SetBagPresent( _spPresent );
}
       
void BettyWindow::OnBettyLoveChanged()
{
    // Update pictures
    UpdatePictures();
}
      
void BettyWindow::OnSammyPointsChanged()
{
    // Update sammy point bars
    UpdateSammy();
}
 