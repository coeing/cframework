#include "ced/Windows/SettingsWindow.h"

#include "cgui/Gui/Button.h"

using namespace Ced;

const CFoundation::String SettingsWindow::GetXMLFileName()
{
	return "data/screens/SettingsWindow.xml";
}

SettingsWindow::SettingsWindow()
{
    // Create delegates
    m_spDelegateClickedOk = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &SettingsWindow::OnOk ) );
    m_spDelegateClickedCancel = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &SettingsWindow::OnCancel ) );
}

void SettingsWindow::InitWindow()
{
    m_spBtOk = GetButton( "BtOk" );
    if ( m_spBtOk != NULL )
    {
        m_spBtOk->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedOk );
    }

    m_spBtCancel = GetButton( "BtCancel" );
    if ( m_spBtCancel != NULL )
    {
        m_spBtCancel->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedCancel );
    }

    // Make modal
    WindowManager::GetInstance().SetModalWindow( GetSharedPtr() );

    CCtrl::WindowController::InitWindow();
}
        
void SettingsWindow::DeinitWindow()
{
    // Make unmodal
    WindowManager::GetInstance().SetModalWindow( WindowControllerPtr() );

    if ( m_spBtOk != NULL )
    {
        m_spBtOk->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedOk );
    }

    if ( m_spBtCancel != NULL )
    {
        m_spBtCancel->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedCancel );
    }

    CCtrl::WindowController::DeinitWindow();
}
       
void SettingsWindow::OnOk()
{
    Close( WINDOW_RETURN_VALUE_OK );
}
        
void SettingsWindow::OnCancel()
{
    Close( WINDOW_RETURN_VALUE_CANCEL );
}
