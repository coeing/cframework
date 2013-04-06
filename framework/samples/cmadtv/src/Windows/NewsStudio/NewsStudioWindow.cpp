#include "cmadtv/Windows/NewsStudio/NewsStudioWindow.h"

#include "cgui/Gui/CheckBox.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/TextBox.h"

#include "cgui/Tooltip/PointerTooltipController.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/News/NewsChannel.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerCompNews.h"
#include "cmadtv/Data/Player/PlayerContext.h"

#include "cmadtv/Logic/PlayerInput.h"
#include "cmadtv/Logic/Commands/News/ClearRecentlySubscribedChannelsCmd.h"
#include "cmadtv/Logic/Commands/News/SubscribeNewsChannelCmd.h"

#include "cmadtv/Windows/NewsStudio/NewsPlanningWindow.h"

using namespace CMadTV;
        
CFoundation::String NewsStudioWindow::GetXMLFileName()
{
    return "data/windows/NewsStudioWindow.xml";
}

NewsStudioWindow::NewsStudioWindow()
{
    m_spDelegateTooltipChannelStarted = CGui::TooltipControllerEventDelegate2Ptr( new CGui::TooltipControllerEventDelegate2( this, &NewsStudioWindow::OnTooltipChannelStarted ) );
    m_spDelegateChannelCheckChanged = CGui::CheckBoxEventDelegate3Ptr( new CGui::CheckBoxEventDelegate3( this, &NewsStudioWindow::OnChannelCheckChanged ) );
    m_spDelegateBoardClicked = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &NewsStudioWindow::OnBoardClicked ) );
}

NewsStudioWindow::~NewsStudioWindow()
{
}

void NewsStudioWindow::InitWindow()
{    
    m_spObjBoard = GetGuiObject( "ObjBoard" );
    if ( m_spObjBoard != NULL )
    {
        m_spObjBoard->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateBoardClicked );
    }

    m_spPnlTooltipChannel = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Panel >( "data/windows/NewsChannelTooltip.xml" );
    m_spTbTooltipChannelName = static_pointer_cast< CGui::TextBox >( m_spPnlTooltipChannel->FindDescendant( "TbName" ) );
    m_spTbTooltipChannelPrice = static_pointer_cast< CGui::TextBox >( m_spPnlTooltipChannel->FindDescendant( "TbPrice" ) );

    CGui::PointerTooltipControllerPtr spTooltipChannel( new CGui::PointerTooltipController() );
    spTooltipChannel->SetTooltip( m_spPnlTooltipChannel );
    spTooltipChannel->RegisterDelegate( CGui::EVENT_TOOLTIP_CONTROLLER_STARTED, m_spDelegateTooltipChannelStarted );

    // Set tooltip for channels
    for ( Unsigned32 u32Idx = 0; u32Idx < MAX_CHANNELS; ++u32Idx )
    {
        CGui::CheckBoxPtr spChkNewsChannel = GetCheckBox( CFoundation::String::Build( "BtNewsChannel%d", u32Idx + 1 ) );
        if ( spChkNewsChannel == NULL )
        {
            continue;
        }

        // Register delegates
        spChkNewsChannel->RegisterDelegate( CGui::EVENT_CHECKBOX_CHECK_CHANGED, m_spDelegateChannelCheckChanged );

        CGui::TooltipSystem::GetInstance().SetTooltipController( spChkNewsChannel, spTooltipChannel );
        m_mapChannelIDs[ spChkNewsChannel ] = u32Idx;
    }

    WindowController::InitWindow();
}
        
void NewsStudioWindow::DeinitWindow()
{
    if ( m_spObjBoard != NULL )
    {
        m_spObjBoard->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateBoardClicked );
    }

    WindowController::DeinitWindow();
}
        
void NewsStudioWindow::OnContextChanged()
{
    SetPlayer( m_spContext != NULL ? m_spContext->GetPlayer() : PlayerPtr() );
}
		
void NewsStudioWindow::Close( Unsigned32 _u32ReturnValue )
{
    if( m_spContext != NULL )
    {
        // Clear recently added channels
        ClearRecentlySubscribedChannelsCmdPtr spCmd( new ClearRecentlySubscribedChannelsCmd( m_spContext ) );
        m_spContext->GetInput()->Execute( spCmd );
    }

    Window::Close( _u32ReturnValue );
}
                
void NewsStudioWindow::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    m_spPlayer = _spPlayer;

    // Get news component
    PlayerCompNewsPtr spCompNews = m_spPlayer != NULL ? m_spPlayer->GetCompNews() : PlayerCompNewsPtr();
    if ( spCompNews != NULL )
    {
        for ( ObjectChannelIDMap::const_iterator it = m_mapChannelIDs.begin(); it != m_mapChannelIDs.end(); ++it )
        {
            Unsigned32 u32NewsChannelID = it->second;
            NewsChannelPtr spChannel = m_spContext->GetDatabase()->GetNewsChannel( u32NewsChannelID );

            // Check if subscribed
            bool bSubscribed = spCompNews->IsSubscribed( spChannel );

            // Set initial checkbox values
            CGui::CheckBoxPtr spCheckBox = it->first;
            spCheckBox->SetChecked( bSubscribed );
        }
    }    
}
       
NewsChannelPtr NewsStudioWindow::FindChannel( CGui::GuiObjectPtr _spObject ) const
{
    // Get channel id
    ObjectChannelIDMap::const_iterator it = m_mapChannelIDs.find( dynamic_pointer_cast< CGui::CheckBox >( _spObject ) );
    if ( it == m_mapChannelIDs.end() )
    {
        return NewsChannelPtr();
    }

    // Get channel
    Unsigned32 u32NewsChannelID = it->second;
    return m_spContext->GetDatabase()->GetNewsChannel( u32NewsChannelID );
}

void NewsStudioWindow::OnTooltipChannelStarted( CGui::TooltipControllerPtr _spTooltipController, CGui::GuiObjectPtr _spObject )
{
    // Get channel
    NewsChannelPtr spChannel = FindChannel( _spObject );
    if ( spChannel == NULL )
    {
        return;
    }

    // Set name and price of news channel
    SAFE_CALL( m_spTbTooltipChannelName )->SetText( spChannel->GetName() );
    SAFE_CALL( m_spTbTooltipChannelPrice )->SetText( CFoundation::String::Build( "$ %.0f", spChannel->GetPrice().GetAmount( CFoundation::CURRENCY_DOLLAR ) ) );
}
        
void NewsStudioWindow::OnChannelCheckChanged( CGui::CheckBoxPtr _spCheckBox, bool _bNewState, bool /*_bOldState*/ )
{
    // Get channel
    NewsChannelPtr spChannel = FindChannel( _spCheckBox );
    if ( spChannel == NULL ||
         m_spContext == NULL )
    {
        return;
    }

    // Send command
    SubscribeNewsChannelCmdPtr spCmd( new SubscribeNewsChannelCmd( m_spContext, spChannel, _bNewState ) );
    m_spContext->GetInput()->Execute( spCmd );
}

void NewsStudioWindow::OnBoardClicked()
{
    // Create news planning window
    NewsPlanningWindowPtr spPlanningWindow = Create< NewsPlanningWindow >( GetSharedPtr() );
    spPlanningWindow->SetContext( m_spContext );

    if( m_spContext != NULL )
    {
        // Clear recently added channels
        ClearRecentlySubscribedChannelsCmdPtr spCmd( new ClearRecentlySubscribedChannelsCmd( m_spContext ) );
        m_spContext->GetInput()->Execute( spCmd );
    }
}
