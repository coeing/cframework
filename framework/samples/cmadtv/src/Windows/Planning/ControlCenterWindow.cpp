#include "cmadtv/Windows/Planning/ControlCenterWindow.h"

#include <math.h>

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Enum/EnumIncrement.h"
#include "cfoundation/Math/Utils.h"

#include "csystem/Input/Input.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Tooltip/TooltipSystem.h"

#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/DayStat.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerCompSpectators.h"
#include "cmadtv/Data/Player/PlayerCompStats.h"
#include "cmadtv/Data/Player/PlayerContext.h"

#include "cmadtv/Tooltip/StatsRatingTooltipController.h"
#include "cmadtv/Windows/Planning/StatsImageWindow.h"

using namespace CMadTV;
        
CFoundation::String ControlCenterWindow::GetXMLFileName()
{
    return "data/windows/ControlCenterWindow.xml";
}

ControlCenterWindow::ControlCenterWindow()
{
    m_spDelegateFamilyActivated = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &ControlCenterWindow::OnFamilyActivated ) );
    m_spDelegateFamilyDeactivated = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &ControlCenterWindow::OnFamilyDeactivated ) );
    m_spDelegatePictureActivated = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &ControlCenterWindow::OnPictureActivated ) );
    m_spDelegatePictureDeactivated = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &ControlCenterWindow::OnPictureDeactivated ) );
    m_spDelegateMusicActivated = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &ControlCenterWindow::OnMusicActivated ) );
    m_spDelegateMusicDeactivated = CGui::StateEventDelegate0Ptr( new CGui::StateEventDelegate0( this, &ControlCenterWindow::OnMusicDeactivated ) );
    m_spDelegateChangedPlayerSelection = CCtrl::ButtonGroupButtonEventDelegate3Ptr( new CCtrl::ButtonGroupButtonEventDelegate3( this, &ControlCenterWindow::OnChangedSpeedSelection ) );
}

ControlCenterWindow::~ControlCenterWindow()
{
}

void ControlCenterWindow::InitWindow()
{      
    // Create button group
    m_spSpeedButtons = CCtrl::ButtonGroupPtr( new CCtrl::ButtonGroup() );

    // Register delegate
    m_spSpeedButtons->RegisterDelegate( CCtrl::BUTTON_GROUP_EVENT_SELECTION_CHANGED, m_spDelegateChangedPlayerSelection );

    // Get player buttons
    CGui::ButtonPtr spBtSpeed1 = GetButton( "BtSpeed1" );
    m_mapButtonSpeeds[ spBtSpeed1 ] = 60.0f;
    CGui::ButtonPtr spBtSpeed2 = GetButton( "BtSpeed2" );
    m_mapButtonSpeeds[ spBtSpeed2 ] = 120.0f;
    CGui::ButtonPtr spBtSpeed3 = GetButton( "BtSpeed3" );
    m_mapButtonSpeeds[ spBtSpeed3 ] = 180.0f;

    // Add to button group
    m_spSpeedButtons->AddButton( spBtSpeed1 );
    m_spSpeedButtons->AddButton( spBtSpeed2 );
    m_spSpeedButtons->AddButton( spBtSpeed3 );

    m_spBtFamily = GetButton( "BtFamily" );
    m_spBtFamily->RegisterDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateFamilyActivated );
    m_spBtFamily->RegisterDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateFamilyDeactivated );

    m_spBtPicture = GetButton( "BtPicture" );
    m_spBtPicture->RegisterDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegatePictureActivated );
    m_spBtPicture->RegisterDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegatePictureDeactivated );

    m_spBtMusic = GetButton( "BtMusic" );
    m_spBtMusic->RegisterDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateMusicActivated );
    m_spBtMusic->RegisterDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateMusicDeactivated );

    WindowController::InitWindow();
}
        
void ControlCenterWindow::DeinitWindow()
{
    // Remove delegates
    m_spSpeedButtons->RemoveDelegate( CCtrl::BUTTON_GROUP_EVENT_SELECTION_CHANGED, m_spDelegateChangedPlayerSelection );
    m_spBtFamily->RemoveDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateFamilyActivated );
    m_spBtFamily->RemoveDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateFamilyDeactivated );
    m_spBtPicture->RemoveDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegatePictureActivated );
    m_spBtPicture->RemoveDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegatePictureDeactivated );
    m_spBtMusic->RemoveDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateMusicActivated );
    m_spBtMusic->RemoveDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateMusicDeactivated );

    WindowController::DeinitWindow();
}

void ControlCenterWindow::ProcessInput( const CSystem::InputState& _State )
{
    if ( _State.WasPressed( CSystem::MOUSE_RIGHT ) ||
         _State.WasPressed( CSystem::KEY_ESC ) )
    {
        Close();
    }
}
        
void ControlCenterWindow::OnContextChanged()
{
    SetGame( m_spContext != NULL ? m_spContext->GetGame() : GamePtr() );   
}
        
void ControlCenterWindow::SetGame( GamePtr _spGame )
{
    if ( _spGame == m_spGame )
    {
        return;
    }

    m_spGame = _spGame;

    if ( m_spGame != NULL )
    {
        // Get current speed
        Float32 f32Speed = m_spGame->GetSpeed();

        // Get closest speed button to select
        CGui::ButtonPtr spBtSpeed;
        Float32 f32BestDiff = MAX_F32;
        for ( ButtonSpeedMap::const_iterator it = m_mapButtonSpeeds.begin(); it != m_mapButtonSpeeds.end(); ++it )
        {
            Float32 f32ButtonSpeed = it->second;
            Float32 f32Diff = fabs( f32ButtonSpeed - f32Speed );
            if ( f32Diff < f32BestDiff )
            {
                f32BestDiff = f32Diff;
                spBtSpeed = it->first;
            }
        }

        // Select best speed button
        m_spSpeedButtons->SetActiveButton( spBtSpeed );
    }
}
        
void ControlCenterWindow::OnFamilyActivated()
{
}
        
void ControlCenterWindow::OnFamilyDeactivated()
{
}
        
void ControlCenterWindow::OnPictureActivated()
{
}
        
void ControlCenterWindow::OnPictureDeactivated()
{
}
        
void ControlCenterWindow::OnMusicActivated()
{
}
        
void ControlCenterWindow::OnMusicDeactivated()
{
}
       
void ControlCenterWindow::OnChangedSpeedSelection( CCtrl::ButtonGroupPtr _spButtonGroup, CGui::ButtonPtr _spNewActiveButton, CGui::ButtonPtr _spOldActiveButton )
{
    // TODO: Send command
    /*
    // Set new speed
    if ( m_spGame != NULL )
    {
        m_spGame->SetSpeed( m_mapButtonSpeeds[ _spNewActiveButton ] );
    }*/
}
     