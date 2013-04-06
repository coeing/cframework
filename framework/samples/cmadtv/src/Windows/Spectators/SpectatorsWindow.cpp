#include "cmadtv/Windows/Spectators/SpectatorsWindow.h"

#include "cfoundation/Math/Utils.h"

#include "csystem/Input/Input.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/CheckBox.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/TextBox.h"

#include "cgui/Input/Cursor.h"
#include "cgui/Input/Input.h"

#include "cgui/Texture/TextureLoader.h"

#include "cgui/Tooltip/PointerTooltipController.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerCompSpectators.h"
#include "cmadtv/Data/Player/PlayerContext.h"
#include "cmadtv/Data/Spectators/Map.h"
#include "cmadtv/Data/Spectators/Transmitter.h"

#include "cmadtv/Logic/PlayerInput.h"
#include "cmadtv/Logic/Commands/Spectators/BuyAntennaCmd.h"
#include "cmadtv/Logic/Commands/Spectators/TradeTransmitterCmd.h"

using namespace CMadTV;
        
CFoundation::String SpectatorsWindow::GetXMLFileName()
{
    return "data/windows/SpectatorsWindow.xml";
}

SpectatorsWindow::SpectatorsWindow()
:   m_eMode( MODE_NONE ),
    m_u32Player( MAX_U32 ),
    m_u32SelectedTransmitter( 0 )
{
    m_spDelegateSatelliteEntered = CGui::ObjectInputEventDelegate1Ptr( new CGui::ObjectInputEventDelegate1( this, &SpectatorsWindow::OnSatelliteEntered ) );
    m_spDelegateSatelliteLeft = CGui::ObjectInputEventDelegate1Ptr( new CGui::ObjectInputEventDelegate1( this, &SpectatorsWindow::OnSatelliteLeft ) );
    m_spDelegateSatelliteClicked = CGui::ObjectInputEventDelegate1Ptr( new CGui::ObjectInputEventDelegate1( this, &SpectatorsWindow::OnSatelliteClicked ) );
    m_spDelegateBuyActivated = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &SpectatorsWindow::OnBuyActivated ) );
    m_spDelegateBuyDeactivated = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &SpectatorsWindow::OnBuyDeactivated ) );
    m_spDelegateSellActivated = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &SpectatorsWindow::OnSellActivated ) );
    m_spDelegateSellDeactivated = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &SpectatorsWindow::OnSellDeactivated ) );
    m_spDelegatePrevTransmitterClicked = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &SpectatorsWindow::OnPrevTransmitterClicked ) );
    m_spDelegateNextTransmitterClicked = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &SpectatorsWindow::OnNextTransmitterClicked ) );
    m_spDelegateSellOkClicked = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &SpectatorsWindow::OnSellOkClicked ) );
    m_spDelegateChangedPlayerSelection = CCtrl::ButtonGroupButtonEventDelegate3Ptr( new CCtrl::ButtonGroupButtonEventDelegate3( this, &SpectatorsWindow::OnChangedPlayerSelection ) );
}

SpectatorsWindow::~SpectatorsWindow()
{
}

void SpectatorsWindow::InitWindow()
{    
    m_spPnlMap = GetPanel( "PnlMap" );

    for ( Unsigned32 u32Idx = 0; u32Idx < MAX_NUM_PLAYERS; ++u32Idx )
    {
        CGui::ImagePtr spImgTplTransmitter = GetImage( CFoundation::String::Build( "ImgAntenna%d", u32Idx + 1 ) );
        spImgTplTransmitter->RemoveFromParent();
        m_mapPlayerImgTplTransmitter[ u32Idx ] = spImgTplTransmitter;
    }

    // Get satellite images
    for ( Unsigned32 u32Idx = 0; u32Idx < MAX_NUM_SATELLITES; ++u32Idx )
    {
        CGui::ImagePtr spImgSatellite = GetImage( CFoundation::String::Build( "ImgSatellite%d", u32Idx + 1 ) );

        // Register delegates
        spImgSatellite->RegisterDelegate( CGui::INPUT_EVENT_POINTER_ENTERED, m_spDelegateSatelliteEntered );
        spImgSatellite->RegisterDelegate( CGui::INPUT_EVENT_POINTER_LEFT, m_spDelegateSatelliteLeft );
        spImgSatellite->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateSatelliteClicked );

        m_aspImgSatellites.push_back( spImgSatellite );
    }
    
    m_spBtBuy = GetButton( "BtBuy" );
    m_spBtBuy->RegisterDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateBuyActivated );
    m_spBtBuy->RegisterDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateBuyDeactivated );
    m_spBtSell = GetButton( "BtSell" );
    m_spBtSell->RegisterDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateSellActivated );
    m_spBtSell->RegisterDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateSellDeactivated );
    m_spBtPrevTransmitter = GetButton( "BtPrevTransmitter" );
    m_spBtPrevTransmitter->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegatePrevTransmitterClicked );
    m_spBtPrevTransmitter->SetVisible( false );
    m_spBtNextTransmitter = GetButton( "BtNextTransmitter" );
    m_spBtNextTransmitter->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateNextTransmitterClicked );
    m_spBtNextTransmitter->SetVisible( false );
    m_spBtSellOk = GetButton( "BtSellOk" );
    m_spBtSellOk->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateSellOkClicked );
    m_spBtSellOk->SetVisible( false );

    // Create button group
    m_spPlayerButtons = CCtrl::ButtonGroupPtr( new CCtrl::ButtonGroup() );

    // Register delegate
    m_spPlayerButtons->RegisterDelegate( CCtrl::BUTTON_GROUP_EVENT_SELECTION_CHANGED, m_spDelegateChangedPlayerSelection );

    for ( Unsigned32 u32Idx = 0; u32Idx < MAX_NUM_PLAYERS; ++u32Idx )
    {
        // Get player button
        CGui::ButtonPtr spBtPlayer = GetButton( CFoundation::String::Build( "BtPlayer%d", u32Idx + 1 ) );

        // Add to button group
        if ( m_spPlayerButtons->AddButton( spBtPlayer ) )
        {
            m_mapButtonPlayers[ spBtPlayer ] = m_spPlayerButtons->GetNumButtons() - 1;
        }
    }

    // Get tooltips
    m_spPnlTooltipBuy = GetPanel( "PnlTooltipBuy" );
    m_spPnlTooltipBuy->SetVisible( false );
    m_spTbTooltipBuySpectators = static_pointer_cast< CGui::TextBox >( m_spPnlTooltipBuy->FindChild( "TbSpectators" ) );
    m_spTbTooltipBuyPrice = static_pointer_cast< CGui::TextBox >( m_spPnlTooltipBuy->FindChild( "TbPrice" ) );

    m_spPnlTooltipSell = GetPanel( "PnlTooltipSell" );
    m_spPnlTooltipSell->SetVisible( false );
    m_spTbTooltipSellSpectators = static_pointer_cast< CGui::TextBox >( m_spPnlTooltipSell->FindChild( "TbSpectators" ) );
    m_spTbTooltipSellPrice = static_pointer_cast< CGui::TextBox >( m_spPnlTooltipSell->FindChild( "TbPrice" ) );

    m_spPnlTooltipSatellite = GetPanel( "PnlTooltipSatellite" );
    m_spPnlTooltipSatellite->SetVisible( false );
    m_spPnlTooltipSatelliteOwn = GetPanel( "PnlTooltipSatelliteOwn" );
    m_spPnlTooltipSatelliteOwn->SetVisible( false );

    WindowController::InitWindow();
}
        
void SpectatorsWindow::DeinitWindow()
{
    // Remove delegates
    for ( Unsigned32 u32Idx = 0; u32Idx < MAX_NUM_SATELLITES; ++u32Idx )
    {
        CGui::ImagePtr spImgSatellite = m_aspImgSatellites[ u32Idx ];
        spImgSatellite->RemoveDelegate( CGui::INPUT_EVENT_POINTER_ENTERED, m_spDelegateSatelliteEntered );
        spImgSatellite->RemoveDelegate( CGui::INPUT_EVENT_POINTER_LEFT, m_spDelegateSatelliteLeft );
        spImgSatellite->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateSatelliteClicked );
    }

    m_spBtBuy->RemoveDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateBuyActivated );
    m_spBtBuy->RemoveDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateBuyDeactivated );
    m_spBtSell->RemoveDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateSellActivated );
    m_spBtSell->RemoveDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateSellDeactivated );
    m_spBtPrevTransmitter->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegatePrevTransmitterClicked );
    m_spBtNextTransmitter->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateNextTransmitterClicked );
    m_spBtSellOk->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateSellOkClicked );
    m_spPlayerButtons->RemoveDelegate( CCtrl::BUTTON_GROUP_EVENT_SELECTION_CHANGED, m_spDelegateChangedPlayerSelection );

    // Delete player antenna templates
    for ( Unsigned32 u32Idx = 0; u32Idx < MAX_NUM_PLAYERS; ++u32Idx )
    {
        CGui::ImagePtr spImgTplTransmitter = m_mapPlayerImgTplTransmitter[ u32Idx ];
        CGui::GuiMain::GetInstance().Delete( spImgTplTransmitter );
    }

    WindowController::DeinitWindow();
}

void SpectatorsWindow::ProcessInput( const CSystem::InputState& _State )
{
    if ( m_eMode == MODE_BUY )
    {
        if ( _State.WasPressed( CSystem::MOUSE_RIGHT ) ||
             _State.WasPressed( CSystem::KEY_ESC ) )
        {
            EndBuyMode( false );
        }
        else if ( _State.WasPressed( CSystem::MOUSE_LEFT ) )
        {
            EndBuyMode( true );
        }
        else if ( !_State.GetMouseMovement().IsZero() )
        {
            // Update spectators
            Unsigned32 u32NumSpectators = 0;
            Unsigned32 u32NumNewSpectators = ComputeNumNewSpectators( u32NumSpectators );
            m_spTbTooltipBuySpectators->SetText( CFoundation::String::MakeMillionString( u32NumNewSpectators ) );
            
            // Update price
            CFoundation::Money priceAntenna = m_spContext->GetDatabase()->ComputeAntennaPrice( u32NumSpectators );
            m_spTbTooltipBuyPrice->SetText( priceAntenna.ToString( CFoundation::CURRENCY_DOLLAR ) );
        }
    }
    else if ( m_eMode == MODE_SELL )
    {
        if ( _State.WasPressed( CSystem::MOUSE_RIGHT ) ||
             _State.WasPressed( CSystem::KEY_ESC ) )
        {
            EndSellMode( false );
        }
    }
}

void SpectatorsWindow::OnContextChanged()
{
    SetGame( m_spContext != NULL ? m_spContext->GetGame() : GamePtr() );

    // Set initial player
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayerIdx() : MAX_U32 );
}
       
void SpectatorsWindow::SetGame( GamePtr _spGame )
{
    if ( _spGame == m_spGame )
    {
        return;
    }

    m_spGame = _spGame;

    if ( m_spGame != NULL )
    {
        for ( Unsigned32 u32Idx = 0; u32Idx < MAX_NUM_PLAYERS; ++u32Idx )
        {
            CGui::ButtonPtr spButton = m_spPlayerButtons->GetButton( u32Idx );

            // Set button text
            PlayerPtr spPlayer = m_spGame->GetPlayer( u32Idx );
            spButton->SetVisible( spPlayer != NULL );
            if ( spPlayer != NULL )
            {
                spButton->SetText( spPlayer->GetName() );
            }
        }
    }

    // Update player
    UpdatePlayer();
}
        
void SpectatorsWindow::SetPlayer( Unsigned32 _u32Player )
{
    if ( _u32Player == m_u32Player )
    {
        return;
    }
    
    m_u32Player = _u32Player;

    // Activate correct player button
    m_spPlayerButtons->SetActiveButton( m_u32Player );

    // Update player
    UpdatePlayer();
}
      
void SpectatorsWindow::UpdatePlayer()
{
    SetPlayer( m_spGame != NULL ? m_spGame->GetPlayer( m_u32Player ) : PlayerPtr() );
}
        
void SpectatorsWindow::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    if ( m_spPlayer != NULL )
    {
        // Remove delegates
        DISCONNECT_DELEGATE_CONNECTION( m_spDelegateTransmitterAdded );
        DISCONNECT_DELEGATE_CONNECTION( m_spDelegateTransmitterRemoved );
    }

    // Get player pointer
    m_spPlayer = _spPlayer; 

    if ( m_spPlayer != NULL )
    {
        // Register delegates
        PlayerCompSpectatorsPtr spCompSpectators = m_spPlayer->GetCompSpectators();
        m_spDelegateTransmitterAdded = spCompSpectators->RegisterDelegate( EVENT_TRANSMITTER_ADDED, CreateDelegate( this, &SpectatorsWindow::OnTransmitterAdded ) );
        m_spDelegateTransmitterRemoved = spCompSpectators->RegisterDelegate( EVENT_TRANSMITTER_REMOVED, CreateDelegate( this, &SpectatorsWindow::OnTransmitterRemoved ) );
    }

    // Update transmitters
    ShowAllTransmitters();
}
       
void SpectatorsWindow::SetMode( Mode _eMode )
{
    if ( _eMode == m_eMode )
    {
        return;
    }

    switch ( m_eMode )
    {
    case MODE_BUY:
        {
            EndBuyMode( false );
        }
        break;
    case MODE_SELL:
        {
            EndSellMode( false );
        }
        break;
    }

    m_eMode = _eMode;
}

void SpectatorsWindow::StartBuyMode()
{
    SetMode( MODE_BUY );

    // Set active player
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayerIdx() : MAX_U32 );

    // Set custom cursor
    CGui::Input::GetInstance().SetCustomCursor( m_mapPlayerImgTplTransmitter[ m_u32Player ], CFoundation::Vector2Di32( 0, 0 ) );

    // Clip cursor
    CGui::GuiMain::GetInstance().GetCursor()->ClipCursor( CFoundation::RectI32( m_spPnlMap->GetAbsolutePosition(), m_spPnlMap->GetSize() ) );

    // Show tooltip panel
    m_spPnlTooltipBuy->SetVisible( true );
}

void SpectatorsWindow::BuyAntenna( const CFoundation::Vector2Di& _vPosition )
{    
    // Try to buy antenna
    BuyAntennaCmdPtr spCmd( new BuyAntennaCmd( m_spContext, _vPosition ) );
    Unsigned32 u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
}

void SpectatorsWindow::BuyTransmitter( TransmitterPtr _spTransmitter )
{
    if ( m_spContext == NULL )
    {
        return;
    }

    // Try to buy transmitter
    TradeTransmitterCmdPtr spCmd( new TradeTransmitterCmd( m_spContext, TradeTransmitterCmd::MODE_BUY, _spTransmitter ) );
    Unsigned32 u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
    if ( u32ReturnValue == 0 )
    {
        // Show transmitter
        ShowTransmitter( _spTransmitter );
    }
}

void SpectatorsWindow::EndBuyMode( bool _bBuyTransmitter )
{
    if ( _bBuyTransmitter &&
         m_spPnlMap != NULL )
    {
        // Get current relative mouse position
        const CSystem::InputState& inputState = CSystem::Input::GetInstance().GetState();
        const CFoundation::Vector2Di& vMousePosition = inputState.GetMousePosition();
        CFoundation::Vector2Di vRelMousePosition = vMousePosition - m_spPnlMap->GetAbsolutePosition();  

        // Try to buy antenna
        BuyAntenna( vRelMousePosition );
    }

    // Deactivate buy button
    m_spBtBuy->SetActive( false );

    // Hide tooltip panel
    m_spPnlTooltipBuy->SetVisible( false );

    // Unclip cursor
    CGui::GuiMain::GetInstance().GetCursor()->UnclipCursor();

    // Remove custom cursor
    CGui::Input::GetInstance().RemoveCustomCursor();

    m_eMode = MODE_NONE;
}
       
void SpectatorsWindow::StartSellMode()
{
    SetMode( MODE_SELL );

    // Set active player
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayerIdx() : MAX_U32 );

    // Show controls
    m_spBtPrevTransmitter->SetVisible( true );
    m_spBtNextTransmitter->SetVisible( true );
    m_spBtSellOk->SetVisible( true );

    // Show tooltip panel
    m_spPnlTooltipSell->SetVisible( true );

    m_u32SelectedTransmitter = 0;

    // Update selected transmitter
    UpdateSelectedTransmitter();
}

void SpectatorsWindow::SelectPrevTransmitter()
{
    --m_u32SelectedTransmitter;
    UpdateSelectedTransmitter();
}

void SpectatorsWindow::SelectNextTransmitter()
{
    ++m_u32SelectedTransmitter;
    UpdateSelectedTransmitter();
}
        
void SpectatorsWindow::UpdateSelectedTransmitter()
{
    // Remove old transmitter icons
    RemoveTransmitters();

    if ( m_spPlayer == NULL )
    {
        return;
    }

    // Get spectators component
    PlayerCompSpectatorsPtr spCompSpectators = m_spPlayer->GetCompSpectators();

    // Get transmitters
    const std::vector< TransmitterPtr >& aspTransmitters = spCompSpectators->GetTransmitters();
    Unsigned32 u32NumTransmitters = aspTransmitters.size();

    // Clamp selected transmitter
    if ( m_u32SelectedTransmitter == MAX_U32 )
    {
        m_u32SelectedTransmitter = u32NumTransmitters > 0 ? u32NumTransmitters - 1 : 0;
    }
    else if ( m_u32SelectedTransmitter >= u32NumTransmitters )
    {
        m_u32SelectedTransmitter = 0;
    }

    if ( m_u32SelectedTransmitter < u32NumTransmitters )
    {
        TransmitterPtr spTransmitter = aspTransmitters[ m_u32SelectedTransmitter ]; 

        // Get map
        MapPtr spMap = m_spContext->GetDatabase()->GetMap();

        // Update tooltip
        Unsigned32 u32NumSpectators = spMap->ComputeNumSpectators( spTransmitter );
        m_spTbTooltipSellSpectators->SetText( CFoundation::String::MakeMillionString( u32NumSpectators ) );
        
        // Update price
        CFoundation::Money priceTransmitter = m_spContext->GetDatabase()->ComputeTransmitterPrice( spTransmitter );
        m_spTbTooltipSellPrice->SetText( priceTransmitter.ToString( CFoundation::CURRENCY_DOLLAR ) );
        
        ShowTransmitter( spTransmitter );
    }
}
       
void SpectatorsWindow::SellTransmitter()
{
    PlayerCompSpectatorsPtr spCompSpectators = m_spContext->GetPlayer()->GetCompSpectators();
    const std::vector< TransmitterPtr >& aspTransmitters = spCompSpectators->GetTransmitters();

    if ( m_u32SelectedTransmitter >= aspTransmitters.size() )
    {
        return;
    }

    TransmitterPtr spTransmitter = aspTransmitters[ m_u32SelectedTransmitter ];

    // Try to sell transmitter
    TradeTransmitterCmdPtr spCmd( new TradeTransmitterCmd( m_spContext, TradeTransmitterCmd::MODE_SELL, spTransmitter ) );
    Unsigned32 u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );
}
       
void SpectatorsWindow::EndSellMode( bool _bSell )
{
    if ( _bSell )
    {
        SellTransmitter();
    }

    // Deactivate sell button
    m_spBtSell->SetActive( false );

    // Hide tooltip panel
    m_spPnlTooltipSell->SetVisible( false );

    // Hide controls
    m_spBtPrevTransmitter->SetVisible( false );
    m_spBtNextTransmitter->SetVisible( false );
    m_spBtSellOk->SetVisible( false );

    // Show all transmitters
    ShowAllTransmitters();

    m_eMode = MODE_NONE;
}
        
Unsigned32 SpectatorsWindow::ComputeNumNewSpectators( Unsigned32& _u32NumSpectators ) const
{
    if ( m_spPnlMap == NULL )
    {
        return 0;
    }

    // Get current mouse position
    const CSystem::InputState& inputState = CSystem::Input::GetInstance().GetState();
    const CFoundation::Vector2Di& vMousePosition = inputState.GetMousePosition();

    // Compute relative mouse position
    CFoundation::Vector2Di vRelMousePosition = vMousePosition - m_spPnlMap->GetAbsolutePosition();    

    // Check if outside borders
    if ( vRelMousePosition.GetX() < 0 ||
         vRelMousePosition.GetY() < 0 )
    {
        return 0;
    }

    // Get map
    MapPtr spMap = m_spContext->GetDatabase()->GetMap();

    // Compute spectators
    return spMap->ComputeNumNewSpectators( vRelMousePosition.GetX(), vRelMousePosition.GetY(), m_spPlayer->GetCompSpectators()->GetTransmitters(), _u32NumSpectators );
}
       
void SpectatorsWindow::ShowAllTransmitters()
{
    // Remove old transmitter icons
    RemoveTransmitters();

    if ( m_spPlayer == NULL )
    {
        return;
    }

    // Get spectators component
    PlayerCompSpectatorsPtr spCompSpectators = m_spPlayer->GetCompSpectators();

    // Get transmitters
    const std::vector< TransmitterPtr >& aspTransmitters = spCompSpectators->GetTransmitters();
    Unsigned32 u32NumTransmitters = aspTransmitters.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumTransmitters; ++u32Idx )
    {
        TransmitterPtr spTransmitter = aspTransmitters[ u32Idx ];

        // Show transmitter
        ShowTransmitter( spTransmitter );
    }
}
        
void SpectatorsWindow::ShowTransmitter( TransmitterPtr _spTransmitter )
{
    // Check if antenna
    if ( _spTransmitter == NULL )
    {
        return;
    }

    switch ( _spTransmitter->GetType() )
    {
    case Transmitter::TYPE_ANTENNA:
        {
            // Create gui object
            CGui::ImagePtr spTransmitterIcon = CGui::GuiMain::GetInstance().CreateFromTemplate( "Transmitter", m_mapPlayerImgTplTransmitter[ m_u32Player ] );

            // Set offset
            spTransmitterIcon->SetOffset( CFoundation::Vector2Di( _spTransmitter->GetX(), _spTransmitter->GetY() ) );

            // Add to map
            m_spPnlMap->AddChildLast( spTransmitterIcon );

            // Store icon
            m_aspImgAntennaIcons.push_back( spTransmitterIcon );
        }
        break;
    case Transmitter::TYPE_SATELLITE:
        {
            // Get satellite index
            Unsigned32 u32Satellite = m_spContext->GetDatabase()->GetSatelliteIdx( _spTransmitter );
            if ( u32Satellite < m_aspImgSatellites.size() )
            {
                m_aspImgSatellites[ u32Satellite ]->SetActive( true );
            }
        }
        break;
    }
}
        
void SpectatorsWindow::RemoveTransmitters()
{
    // Remove old transmitter icons
    Unsigned32 u32NumIcons = m_aspImgAntennaIcons.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumIcons; ++u32Idx )
    {
        m_spPnlMap->RemoveChild( m_aspImgAntennaIcons[ u32Idx ] );
    }
    m_aspImgAntennaIcons.clear();

    // Hide satellites
    Unsigned32 u32NumSatellites = m_aspImgSatellites.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumSatellites; ++u32Idx )
    {
        m_aspImgSatellites[ u32Idx ]->SetActive( false );
    }
}
       
void SpectatorsWindow::SetTransmitterTooltip( bool _bVisible, TransmitterPtr _spTransmitter )
{
    if ( _spTransmitter == NULL ||
         m_spPlayer == NULL )
    {
        return;
    }

    // Get spectators component
    PlayerCompSpectatorsPtr spCompSpectators = m_spPlayer->GetCompSpectators();

    // Get tooltip to use
    CGui::PanelPtr spPnlTooltip;
    switch ( _spTransmitter->GetType() )
    {
    case Transmitter::TYPE_SATELLITE:
        {
            // Check if satellite is owned
            bool bOwned = spCompSpectators->HasTransmitter( _spTransmitter );
            spPnlTooltip = bOwned ? m_spPnlTooltipSatelliteOwn : m_spPnlTooltipSatellite;
        }
        break;
    case Transmitter::TYPE_ANTENNA:
        {
        }
        break;
    }

    if ( spPnlTooltip == NULL )
    {
        return;
    }

    // Check if to show
    spPnlTooltip->SetVisible( _bVisible );
    if ( _bVisible )
    {
        // Get map
        MapPtr spMap = m_spContext->GetDatabase()->GetMap();

        // Update new spectators
        Unsigned32 u32NumSpectators = 0;
        CGui::TextBoxPtr spTbNewSpectators = static_pointer_cast< CGui::TextBox >( spPnlTooltip->FindChild( "TbNewSpectators" ) );
        if ( spTbNewSpectators != NULL )
        {
            // Get spectators component
            PlayerCompSpectatorsPtr spCompSpectators = m_spPlayer->GetCompSpectators();

            // Get transmitters
            const std::vector< TransmitterPtr >& aspTransmitters = spCompSpectators->GetTransmitters();

            Unsigned32 u32NumNewSpectators = spMap->ComputeNumNewSpectators( _spTransmitter, aspTransmitters, u32NumSpectators );
            spTbNewSpectators->SetText( CFoundation::String::MakeMillionString( u32NumNewSpectators ) );
        }
        else
        {
            u32NumSpectators = spMap->ComputeNumSpectators( _spTransmitter );;
        }

        // Update spectators
        CGui::TextBoxPtr spTbSpectators = static_pointer_cast< CGui::TextBox >( spPnlTooltip->FindChild( "TbSpectators" ) );
        spTbSpectators->SetText( CFoundation::String::MakeMillionString( u32NumSpectators ) );
        
        // Update price
        CGui::TextBoxPtr spTbPrice = static_pointer_cast< CGui::TextBox >( spPnlTooltip->FindChild( "TbPrice" ) );
        CFoundation::Money priceTransmitter = m_spContext->GetDatabase()->ComputeTransmitterPrice( _spTransmitter );
        spTbPrice->SetText( priceTransmitter.ToString( CFoundation::CURRENCY_DOLLAR ) );
    }
}

void SpectatorsWindow::OnTransmitterAdded( TransmitterPtr _spTransmitter )
{
    if ( m_eMode != MODE_SELL )
    {
        // Show new transmitter
        ShowTransmitter( _spTransmitter );
    }
}
        
void SpectatorsWindow::OnTransmitterRemoved( TransmitterPtr _spTransmitter )
{
    if ( m_eMode == MODE_SELL )
    {
        // Update selected transmitter
        UpdateSelectedTransmitter();
    }
    else
    {
        ShowAllTransmitters();
    }
}
        
void SpectatorsWindow::OnSatelliteEntered( CGui::GuiObjectPtr _spObject )
{
    if ( m_eMode != MODE_NONE )
    {
        return;
    }

    // Get satellite index
    std::vector< CGui::ImagePtr >::const_iterator it = std::find( m_aspImgSatellites.begin(), m_aspImgSatellites.end(), _spObject );
    if ( it == m_aspImgSatellites.end() )
    {
        return;
    }

    Unsigned32 u32Satellite = it - m_aspImgSatellites.begin();
    SetTransmitterTooltip( true, m_spContext->GetDatabase()->GetSatellite( u32Satellite ) );
}
        
void SpectatorsWindow::OnSatelliteLeft( CGui::GuiObjectPtr _spObject )
{
    if ( m_eMode != MODE_NONE )
    {
        return;
    }

    // Get satellite index
    std::vector< CGui::ImagePtr >::const_iterator it = std::find( m_aspImgSatellites.begin(), m_aspImgSatellites.end(), _spObject );
    if ( it == m_aspImgSatellites.end() )
    {
        return;
    }

    Unsigned32 u32Satellite = it - m_aspImgSatellites.begin();
    SetTransmitterTooltip( false, m_spContext->GetDatabase()->GetSatellite( u32Satellite ) );
}
        
void SpectatorsWindow::OnSatelliteClicked( CGui::GuiObjectPtr _spObject )
{
    if ( m_eMode != MODE_NONE )
    {
        return;
    }

    // Get satellite index
    std::vector< CGui::ImagePtr >::const_iterator it = std::find( m_aspImgSatellites.begin(), m_aspImgSatellites.end(), _spObject );
    if ( it == m_aspImgSatellites.end() )
    {
        return;
    }

    // Get satellite
    Unsigned32 u32Satellite = it - m_aspImgSatellites.begin();
    TransmitterPtr spTransmitter = m_spContext->GetDatabase()->GetSatellite( u32Satellite );

    /*
    // Try to buy satellite
    BuySatelliteCmdPtr spCmd( new BuySatelliteCmd( m_spContext, spTransmitter ) );
    Unsigned32 u32ReturnValue = m_spContext->GetInput()->Execute( spCmd );

    // Get spectators component
    PlayerCompSpectatorsPtr spCompSpectators = m_spContext->GetPlayer()->GetCompSpectators();

    // Check if satellite is already owned
    bool bOwned = spCompSpectators->HasTransmitter( spTransmitter );
    if ( bOwned )
    {
        return;
    }

    // Hide tooltip
    SetTransmitterTooltip( false, spTransmitter );

    // Buy satellite
    BuyTransmitter( spTransmitter );

    // Show tooltip
    SetTransmitterTooltip( true, spTransmitter );*/
}
        
void SpectatorsWindow::OnBuyActivated()
{
    StartBuyMode();
}
        
void SpectatorsWindow::OnBuyDeactivated()
{
    EndBuyMode( false );
}
        
void SpectatorsWindow::OnSellActivated()
{
    StartSellMode();
}
        
void SpectatorsWindow::OnSellDeactivated()
{
    EndSellMode( false );
}
       
void SpectatorsWindow::OnPrevTransmitterClicked()
{
    SelectPrevTransmitter();
}
        
void SpectatorsWindow::OnNextTransmitterClicked()
{
    SelectNextTransmitter();
}
       
void SpectatorsWindow::OnSellOkClicked()
{
    SellTransmitter();
}
        
void SpectatorsWindow::OnChangedPlayerSelection( CCtrl::ButtonGroupPtr _spButtonGroup, CGui::ButtonPtr _spNewActiveButton, CGui::ButtonPtr _spOldActiveButton )
{
    // Set new player
    SetPlayer( m_mapButtonPlayers[ _spNewActiveButton ] );
}
