#include "cmadtv/DragDrop/PresentDropController.h"

#include "cmadtv/Data/Betty/Present.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/Player.h"
#include "cmadtv/Data/Player/PlayerCompBetty.h"

#include "cmadtv/DragDrop/PresentDragController.h"

using namespace CMadTV;

PresentDropController::PresentDropController( Type _eType )
:   m_eType( _eType )
{
}

PresentDropController::~PresentDropController()
{
}

bool PresentDropController::Execute( CGui::GuiObjectPtr _spDropObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& /*_vDropPosition*/, const CFoundation::Vector2Di& /*_vDropPosition*/ ) 
{
    if ( m_spPlayer == NULL )
    {
        return false;
    }

    // Present drag data expected
    PresentDragDataPtr spDragData = dynamic_pointer_cast< PresentDragData >( _spDragData );
    if ( spDragData == NULL )
    {
        return false;
    }

    switch ( m_eType )
    {
    case TYPE_BAG:
        return ExecuteBagDrop( spDragData );
    case TYPE_PRESENTS:
        return ExecutePresentsDrop( spDragData );
    case TYPE_BETTY:
        return ExecuteBettyDrop( spDragData );
    default:
        return false;
    }
}
        
void PresentDropController::SetPlayer( PlayerPtr _spPlayer )
{
    m_spPlayer = _spPlayer;
}
        
bool PresentDropController::ExecuteBagDrop( PresentDragDataPtr _spDragData )
{
    // Check if present came from supermarket or player
    if ( _spDragData->m_spPlayer != NULL )
    {
        return false;
    }
       
    // Check if there's already a present    
    PlayerCompBettyPtr spCompBetty = m_spPlayer->GetCompBetty();
    if ( spCompBetty->GetPresent() != NULL )
    {
        // TODO Dispatch event
        return false;
    }

    // Check if enough cash to buy present
    PresentPtr spPresent = _spDragData->m_spPresent;
    if ( spPresent->GetPrice() > m_spPlayer->GetBudget().GetCash() )
    {
        // TODO Dispatch event
        return false;
    }

    // Buy present
    m_spPlayer->GetBudget().Book( ACCOUNT_CASH, ACCOUNT_SUNDRY_COSTS, spPresent->GetPrice(), m_spPlayer->GetGame()->GetDateTime() );

    // Set present
    spCompBetty->SetPresent( spPresent );
    spCompBetty->EnablePresentReturn( true );    

    return true;
}
        
bool PresentDropController::ExecutePresentsDrop( PresentDragDataPtr _spDragData )
{
    // Check if present came from supermarket or player
    if ( _spDragData->m_spPlayer == NULL )
    {
        return false;
    }

    PlayerCompBettyPtr spCompBetty = m_spPlayer->GetCompBetty();
    PresentPtr spPresent = _spDragData->m_spPresent;

    CF_WARN_IF( spPresent != spCompBetty->GetPresent(), "Present isn't the same in drag data and in player component." );

    // Check if it's possible to return the present
    if ( spCompBetty->IsPresentReturnEnabled() )
    {
        // Get back money
        m_spPlayer->GetBudget().Book( ACCOUNT_SUNDRY_COSTS, ACCOUNT_CASH, spPresent->GetPrice(), m_spPlayer->GetGame()->GetDateTime() );

        spCompBetty->EnablePresentReturn( false );
    }

    // Remove present
    spCompBetty->SetPresent( PresentPtr() );

    return true;
}

bool PresentDropController::ExecuteBettyDrop( PresentDragDataPtr _spDragData )
{
    // Check if present came from player
    if ( _spDragData->m_spPlayer == NULL )
    {
        return false;
    }

    // Security check
    PlayerCompBettyPtr spCompBetty = m_spPlayer->GetCompBetty();
    PresentPtr spPresent = _spDragData->m_spPresent;
    CF_WARN_IF( spPresent != spCompBetty->GetPresent(), "Present isn't the same in drag data and in player component." );
       
    // Give present to betty
    GamePtr spGame = m_spPlayer->GetGame();

    // Compute love increase
    Unsigned8 u8LoveIncrease = spGame->ComputePresentLoveIncrease( spPresent );

    // Add love
    spCompBetty->AddLove( u8LoveIncrease );

    // Remove present
    spCompBetty->SetPresent( PresentPtr() );

    // Increase present counter
    spGame->IncreasePresentCounter( spPresent );

    return true;
}
