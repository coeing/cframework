#include "cctrl/WindowController.h"

#include <algorithm>

#include "cfoundation/Debug/Assert.h"

#include "cgui/Gui/Border.h"
#include "cgui/Gui/Button.h"
#include "cgui/Gui/CheckBox.h"
#include "cgui/Gui/ComboBox.h"
#include "cgui/Gui/EditBox.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/GuiObject.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/Layer.h"
#include "cgui/Gui/ListBox.h"
#include "cgui/Gui/ListBoxColumn.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/ProgressBar.h"
#include "cgui/Gui/ScrollBar.h"
#include "cgui/Gui/ScrollPanel.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Gui/Tree.h"
#include "cgui/Gui/Window.h"

#include "cctrl/Globals.h"
#include "cctrl/ScreenClosedMsg.h"
#include "cctrl/WindowManager.h"

using namespace CCtrl;

#define WINDOW_CONTROLLER_DEFINE_GET_GUI_OBJECT( Type )     CGui::##Type##Ptr WindowController::Get##Type##( const CFoundation::String& _strName ) { \
                                                                return static_pointer_cast< CGui::##Type >( GetGuiObject( _strName ) ); } \
                                                            CGui::##Type##CPtr WindowController::Get##Type##( const CFoundation::String& _strName ) const { \
                                                                return static_pointer_cast< const CGui::##Type >( GetGuiObject( _strName ) ); }

CGui::WindowPtr WindowController::Create( const CFoundation::String& _strXMLFileName )
{
    // Create window
    CGui::WindowPtr pWindow = CGui::GuiMain::GetInstance().CreateFromXML< CGui::Window >( _strXMLFileName );
    CF_WARN_IF( pWindow == NULL, L"WindowController::Create: Could not create window" );
    if ( pWindow != NULL )
    {
        // Add window to gui
        CGui::GuiMain::GetInstance().GetLayer( CGui::LAYER_MAIN ).AddObjectLast( pWindow );
    }
    return pWindow;
}

WindowController::WindowController()
{
}

WindowController::~WindowController()
{
}

void WindowController::InitController( CGui::WindowPtr _spWindow, WindowControllerPtr _spParent )
{
    // Set window pointer
	m_spWindow = _spWindow;

	// Set parent pointer
	m_spParentScreen = _spParent;

    // Listen to window messages
    // m_spWindow->AddListener( *this );
}
		
void WindowController::Update( Float32 /*_f32TimeDifference*/ )
{
}
        
void WindowController::RequestClose( Unsigned32 _u32ReturnValue )
{
    // Request to close this window next frame
    WindowManager::GetInstance().RequestCloseWindow( GetSharedPtr(), _u32ReturnValue );
}

void WindowController::Close( Unsigned32 _u32ReturnValue )
{
	CF_ASSERT( m_spWindow != NULL, L"Controller has no window" );

    WindowControllerPtr spController = GetSharedPtr();

    // Deinitialize window
    DeinitWindow();

	// close screen
	if ( m_spParentScreen != NULL )
	{
		ScreenClosedMsg clScreenClosedMsg( spController );
//		clScreenClosedMsg.SetSender( this );
		clScreenClosedMsg.SetReceiver( m_spParentScreen );
		Send( clScreenClosedMsg );
    }

	// Remove window object from gui
    CGui::GuiMain::GetInstance().Delete( m_spWindow );
    m_spWindow.reset();

    // Inform window manager to destroy this controller
    WindowManager::GetInstance().DestroyWindow( spController );

    // Dispatch event
    DispatchEvent( EVENT_WINDOW_CLOSED, spController, _u32ReturnValue );
}

void WindowController::InitWindow()
{
}
        
void WindowController::DeinitWindow()
{
}
        
void WindowController::ProcessInput( const CSystem::InputState& /*_state*/ )
{
}

CGui::GuiObjectPtr WindowController::GetGuiObject( const CFoundation::String &sName )
{
	if ( m_spWindow->GetName() == sName )
	{        
        //m_spWindow->AddListener( *this );
		return m_spWindow;
	}
	else
	{
        CGui::GuiObjectPtr pObject = m_spWindow->FindDescendant( sName );        
        if ( pObject != NULL )
        {
            //pObject->AddListener( *this );
        }
//        CF_WARN_IF( pObject == NULL, L"WindowController::GetGuiObject: Couldn't find object '%s'", sName );
		return pObject;
	}
}

CGui::GuiObjectCPtr WindowController::GetGuiObject( const CFoundation::String& _sName ) const
{
	WindowController *pTmp = const_cast<WindowController*>( this );
	return pTmp->GetGuiObject( _sName );
}

WINDOW_CONTROLLER_DEFINE_GET_GUI_OBJECT( Panel )
WINDOW_CONTROLLER_DEFINE_GET_GUI_OBJECT( Image )
WINDOW_CONTROLLER_DEFINE_GET_GUI_OBJECT( TextBox )

WINDOW_CONTROLLER_DEFINE_GET_GUI_OBJECT( Button )
WINDOW_CONTROLLER_DEFINE_GET_GUI_OBJECT( CheckBox )
WINDOW_CONTROLLER_DEFINE_GET_GUI_OBJECT( ComboBox )
WINDOW_CONTROLLER_DEFINE_GET_GUI_OBJECT( EditBox )
WINDOW_CONTROLLER_DEFINE_GET_GUI_OBJECT( ListBox )
WINDOW_CONTROLLER_DEFINE_GET_GUI_OBJECT( ProgressBar )
WINDOW_CONTROLLER_DEFINE_GET_GUI_OBJECT( Tree )
WINDOW_CONTROLLER_DEFINE_GET_GUI_OBJECT( Window )

void WindowController::ProcessMessage( const CFoundation::Message &clMessage )
{
    //const CGui::MouseEventMsg* pMouseEventMsg = clMessage.CastTo<CGui::MouseEventMsg>();
    //if ( pMouseEventMsg != NULL )
    //{
    //    if( pMouseEventMsg->GetEvent() == CGui::ME_RELEASED &&
    //        pMouseEventMsg->GetTrigger() == CGui::MOUSE_LEFT )
    //    {
    //        OnLButtonReleased( *pMouseEventMsg );
    //    }
    //    else if ( pMouseEventMsg->GetEvent() == CGui::ME_PRESSED &&
    //              pMouseEventMsg->GetTrigger() == CGui::MOUSE_LEFT )
    //    {
    //        OnLButtonPressed( *pMouseEventMsg );
    //    }
    //    if( pMouseEventMsg->GetEvent() == CGui::ME_RELEASED &&
    //        pMouseEventMsg->GetTrigger() == CGui::MOUSE_RIGHT )
    //    {
    //        OnRButtonReleased( *pMouseEventMsg );
    //    }
    //    else if ( pMouseEventMsg->GetEvent() == CGui::ME_PRESSED &&
    //              pMouseEventMsg->GetTrigger() == CGui::MOUSE_RIGHT )
    //    {
    //        OnRButtonPressed( *pMouseEventMsg );
    //    }
    //    return;
    //}
    //const CGui::KeyboardMsg *pKeyboardMsg = clMessage.CastTo<CGui::KeyboardMsg>();
    //if ( pKeyboardMsg != NULL )
    //{
    //    OnKeyboard( *pKeyboardMsg );
    //    return;
    //}

    //const CGui::KeyboardEventMsg *pKeyboardEventMsg = clMessage.CastTo<CGui::KeyboardEventMsg>();
    //if ( pKeyboardEventMsg != NULL )
    //{
    //    if ( pKeyboardEventMsg->GetEvent() == CGui::KE_DOWN )
    //    {
    //        OnKeyPressed( *pKeyboardEventMsg );
    //    }
    //    else if ( pKeyboardEventMsg->GetEvent() == CGui::KE_UP )
    //    {
    //        OnKeyReleased( *pKeyboardEventMsg );
    //    }
    //    return;
    //}

	const ScreenClosedMsg* pScreenClosedMsg = clMessage.CastTo<ScreenClosedMsg>();
	if( pScreenClosedMsg != NULL )
	{
		OnCloseScreen( *pScreenClosedMsg );
		return;
	}
}
//
//void WindowController::OnLButtonPressed( const CGui::MouseEventMsg& clMessage )
//{
//	UNREFERENCED_PARAMETER( clMessage );
//}
//
//void WindowController::OnLButtonReleased( const CGui::MouseEventMsg& clMessage )
//{
//	UNREFERENCED_PARAMETER( clMessage );
//}
//
//void WindowController::OnRButtonPressed( const CGui::MouseEventMsg& clMessage )
//{
//	UNREFERENCED_PARAMETER( clMessage );
//}
//
//void WindowController::OnRButtonReleased( const CGui::MouseEventMsg& clMessage )
//{
//	UNREFERENCED_PARAMETER( clMessage );
//}
//
//void WindowController::OnKeyboard( const CGui::KeyboardMsg& clMessage )
//{
//	UNREFERENCED_PARAMETER( clMessage );
//}
//
//void WindowController::OnKeyPressed( const CGui::KeyboardEventMsg& clMessage )
//{
//	UNREFERENCED_PARAMETER( clMessage );
//}
//
//void WindowController::OnKeyReleased( const CGui::KeyboardEventMsg& clMessage )
//{
//	UNREFERENCED_PARAMETER( clMessage );
//}
        
void WindowController::SetVisible( bool _bVisible )
{
    SAFE_CALL( m_spWindow )->SetVisible( _bVisible );
}

void WindowController::OnCloseScreen( const ScreenClosedMsg& clMessage )
{
	UNREFERENCED_PARAMETER( clMessage );
}

WindowControllerPtr WindowController::GetSharedPtr()
{
    return static_pointer_cast< WindowController >( MessageObject::GetSharedPtr() );
}

WindowControllerCPtr WindowController::GetSharedPtr() const
{
    return static_pointer_cast< const WindowController >( MessageObject::GetSharedPtr() );
}
       
WindowControllerPtr WindowController::GetParent()
{
    return m_spParentScreen;
}

CGui::WindowCPtr WindowController::GetWindow() const
{
	return m_spWindow;
}

CGui::WindowPtr WindowController::GetWindow()
{
	return m_spWindow;
}
