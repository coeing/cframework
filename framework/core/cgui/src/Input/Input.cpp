#include "cgui/Input/Input.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <vector>

#include "cfoundation/Debug/Logger.h"
#include "cfoundation/Debug/Makros.h"
#include "cfoundation/Events/MessageSystem.h"
#include "cfoundation/DateTime/Timer.h"

#include "csystem/Input/Input.h"

#include "cgui/DragDrop/DragDropSystem.h"

#include "cgui/Input/Cursor.h"
#include "cgui/Input/Mouse.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Layer.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/RenderWindow.h"

using namespace CGui;

CFoundation::Logger gInputLogger( "input.log" );

Input::Input()
:   m_bCursorVisible( true ),
    m_bCursorClipped( false )
{
    // Create delegates
    m_spDelegateDoubleClickedLeft = CSystem::InputEventDelegatePtr( new CSystem::InputEventDelegate( this, &Input::OnDoubleClickedLeft ) );
    m_spDelegateDoubleClickedRight = CSystem::InputEventDelegatePtr( new CSystem::InputEventDelegate( this, &Input::OnDoubleClickedRight ) );

    // Register delegates
    CSystem::Input::GetInstance().RegisterDelegate( CSystem::INPUT_EVENT_LMB_DOUBLE_CLICKED, m_spDelegateDoubleClickedLeft );
    CSystem::Input::GetInstance().RegisterDelegate( CSystem::INPUT_EVENT_RMB_DOUBLE_CLICKED, m_spDelegateDoubleClickedRight );
}

Input::~Input()
{
    CFoundation::MessageSystem::DestroyInstance();
}
       
void Input::Update( Float32 /*_f32TimeDifference*/ )
{
    // Maximum time to process messages (in one frame)
    Unsigned64 u64Time = 30;

	// Timer => don't process all messages
    CFoundation::Timer clTimer;
	clTimer.Start();

    // Get input state
    const CSystem::InputState& inputState = CSystem::Input::GetInstance().GetState();

    // Check events
    if ( inputState.m_u32Events & CSystem::InputState::EVENT_WINDOW_ACTIVATED )
    {
        CGui::CursorPtr spCursor = GuiMain::GetInstance().GetCursor();

        // Restore cursor settings
        spCursor->SetVisible( m_bCursorVisible );
        if ( m_bCursorClipped )
        {
            spCursor->ClipCursor( m_rectCursorClipRect, true );
        }
    }
    else if ( inputState.m_u32Events & CSystem::InputState::EVENT_WINDOW_DEACTIVATED )
    {
        CGui::CursorPtr spCursor = GuiMain::GetInstance().GetCursor();

        // Store cursor settings (to restore when window is activated again)
        m_bCursorVisible = spCursor->IsVisible();
        m_bCursorClipped = spCursor->IsClipped( m_rectCursorClipRect );

        // Release cursor
        spCursor->UnclipCursor();
        spCursor->SetVisible( true );
    }

    // Handle mouse input
    HandleMouseInput();

	while ( clTimer.GetElapsedMs() < u64Time )
	{
        // Handle internal message
        bool bHandled = CFoundation::MessageSystem::GetInstance().ProcessMessage();

        // If no message handled there are no
        if ( !bHandled )
        {
            break;
        }
	}

#ifndef FINAL
    if ( CFoundation::MessageSystem::GetInstance().GetNumMessages() > 0 )
    {
        CFoundation::String sLog;
        sLog.Format( "MessageSystem::ProcessMessages: Left messages after %ldms: %d", u64Time, CFoundation::MessageSystem::GetInstance().GetNumMessages() );
        gInputLogger.Log( sLog );
    }
#endif
}

void Input::SetInputRoot( GuiObjectPtr _pInputRoot )
{
    m_spInputRoot = _pInputRoot;
}

GuiObjectPtr Input::GetInputRoot() const
{
    return m_spInputRoot;
}

GuiObjectPtr Input::GetFocusObject() const
{
	return m_spFocusObject;
}

void Input::HandleMouseInput()
{
    // Get input state
    const CSystem::InputState& inputState = CSystem::Input::GetInstance().GetState();

    // Get current and last mouse position
    const CFoundation::Vector2Di& vMousePosition = inputState.m_vMousePosition;    

    // Set custom cursor position
    if ( m_spCustomCursor != NULL )
    {
        m_spCustomCursor->SetOffset( vMousePosition - m_vCustomCursorHotSpot );
    }

    // Search input receiver
	CFoundation::Vector2Di vRelativePoint = vMousePosition;
    GuiObjectPtr spReceiver = GuiMain::GetInstance().GetLayer( LAYER_FG ).GetObjectAtPoint( vMousePosition, vRelativePoint );
    if ( spReceiver == NULL )
    {
        vRelativePoint = vMousePosition;
        spReceiver = GuiMain::GetInstance().GetLayer( LAYER_MAIN ).GetObjectAtPoint( vMousePosition, vRelativePoint );
    }

    // Check if input receiver under input root
    if ( m_spInputRoot != NULL &&
         !m_spInputRoot->IsDescendant( spReceiver ) )
    {
        spReceiver.reset();
    }

    if( DragDropSystem::GetInstance().ProcessInput( inputState, spReceiver, vRelativePoint ) )
	{
        spReceiver.reset();
	}

    // Set pointed object
    SetPointedObject( spReceiver );

    // Get independent receiver
    //GuiObjectPtr spIndependentReceiver = m_spPointedObject;
    //while ( spIndependentReceiver != NULL &&
    //        !spIndependentReceiver->IsIndependent() )
    //{
    //    spIndependentReceiver = spIndependentReceiver->GetParent();
    //}

    // Check if object was pressed
    if ( inputState.m_eMouseButtonStates[ CSystem::MOUSE_LEFT ] == CSystem::MBS_FIRST_DOWN )
    {
        m_spPressedObject = m_spPointedObject;
        if ( m_spPointedObject == NULL ||
             !m_spPointedObject->IsDisabled() )
        {
            SetFocusObject( m_spPointedObject );
        }
    }
    // Check if released
    else if ( inputState.m_eMouseButtonStates[ CSystem::MOUSE_LEFT ] == CSystem::MBS_FIRST_UP )
    {
        // Check if mouse was released over pressed object
        if ( m_spPointedObject == m_spPressedObject &&
             m_spPressedObject != NULL )
        {
            m_spPressedObject->OnClicked();
            m_spPressedObject.reset();
        }
    }

    // Let focused object process input
    if ( m_spFocusObject != NULL )
    {
        // Check if under input root
        if ( m_spInputRoot == NULL ||
             m_spInputRoot->IsDescendant( m_spFocusObject ) )
        {
            bool bProcessed = false;
            GuiObjectPtr spFocusObject = m_spFocusObject;
            while ( spFocusObject != NULL )
            {
                // Check if input handled
                bProcessed = spFocusObject->ProcessInputFocused( inputState );
                if ( bProcessed )
                {
                    break;
                }

                // Stop if object is independent
                if ( spFocusObject->IsIndependent() )
                {
                    break;
                }
                
                spFocusObject = spFocusObject->GetParent();
            }

            /*
            while ( spFocusObject != NULL &&
                    !spFocusObject->IsIndependent() )
            {
                spFocusObject = spFocusObject->GetParent();
            }
            if ( spFocusObject != NULL )
            {
                spFocusObject->ProcessInputFocused( inputState );
            }*/
        }
    }

    // Let pointed object process input
    if ( m_spPointedObject != NULL )
    {
        // Check if under input root
        if ( m_spInputRoot == NULL ||
             m_spInputRoot->IsDescendant( m_spPointedObject ) )
        {
            GuiObjectPtr spObject = m_spPointedObject;
            while ( spObject != NULL )
            {
                // Check if input handled
                if ( spObject->ProcessInputPointed( inputState ) )
                {
                    break;
                }

                // Stop if object is independent
                if ( spObject->IsIndependent() )
                {
                    break;
                }
                
                spObject = spObject->GetParent();
            }
            //m_spPointedObject->ProcessInputPointed( inputState );
        }
    }
}

void Input::SetFocusObject( GuiObjectPtr _spFocusObject )
{
	if ( _spFocusObject == m_spFocusObject )
	{
        return;
    }

	if ( m_spFocusObject != NULL )
	{
        m_spFocusObject->OnLostFocus();
	}

    GuiObjectPtr spOldFocusedObject = m_spFocusObject;
	m_spFocusObject = _spFocusObject;

	if ( m_spFocusObject != NULL )
	{
        m_spFocusObject->OnGainedFocus();
	}

    // Dispatch event
    DispatchEvent( INPUT_EVENT_FOCUSED_OBJECT_CHANGED, spOldFocusedObject, m_spFocusObject );
}

void Input::SetPointedObject( GuiObjectPtr _spPointedObject )
{
	if ( _spPointedObject == m_spPointedObject )
	{
        return;
    }
    
	// Mouse entering and exiting
    typedef std::vector< GuiObjectPtr > GuiObjectPointerVec;
    GuiObjectPointerVec aspEnteredObjects;
    GuiObjectPointerVec aspExitedObjects;

    // Search shared parent
    GuiObjectPtr spSharedParent = m_spPointedObject;
    while ( spSharedParent != NULL )
    {
        aspEnteredObjects.clear();
        GuiObjectPtr spObject = _spPointedObject;
        while ( spObject != NULL &&
                spObject != spSharedParent )
        {
            aspEnteredObjects.push_back( spObject );
            spObject = spObject->GetParent();
        }

        // Found shared parent
        if ( spObject == spSharedParent )
        {
            break;
        }

        aspExitedObjects.push_back( spSharedParent );
        spSharedParent = spSharedParent->GetParent();
    }

    for ( GuiObjectPointerVec::iterator it = aspExitedObjects.begin(); it != aspExitedObjects.end(); ++it )
    {
        GuiObjectPtr spExitedObject = *it;
        if ( spExitedObject != NULL )
        {
            // Inform object
            spExitedObject->OnPointerLeft();
        }
    }

    for( GuiObjectPointerVec::iterator it = aspEnteredObjects.begin(); it != aspEnteredObjects.end(); ++it )
    {
        GuiObjectPtr spEnteredObject = *it;
        if ( spEnteredObject != NULL )
        {
            // Inform object
            spEnteredObject->OnPointerEntered();
        }
    }

    GuiObjectPtr spOldPointedObject = m_spPointedObject;
    m_spPointedObject = _spPointedObject;

    // Dispatch event
    DispatchEvent( INPUT_EVENT_POINTED_OBJECT_CHANGED, spOldPointedObject, m_spPointedObject );
}
       
void Input::SetCustomCursor( GuiObjectPtr _spCustomCursor )
{
    SetCustomCursor( _spCustomCursor, _spCustomCursor != NULL ? _spCustomCursor->GetSize() / 2 : CFoundation::Vector2Di32() );
}
        
void Input::SetCustomCursor( GuiObjectPtr _spCustomCursor, const CFoundation::Vector2Di32& _vHotSpot )
{
    if ( _spCustomCursor == m_spCustomCursor )
    {
        return;
    }

    if ( _spCustomCursor == NULL )
    {
        RemoveCustomCursor();
        return;
    }

    m_spCustomCursor = _spCustomCursor;
    m_vCustomCursorHotSpot = _vHotSpot;

    // Set to hover layer
    m_spCustomCursor->RemoveFromParent();
    GuiMain::GetInstance().GetLayer( CGui::LAYER_HOVER ).AddObjectLast( m_spCustomCursor );

    // Hide system cursor
    GuiMain::GetInstance().GetCursor()->SetVisible( false );

    // Set initial position
    const CSystem::InputState& inputState = CSystem::Input::GetInstance().GetState();
    const CFoundation::Vector2Di& vMousePosition = inputState.m_vMousePosition;   
    m_spCustomCursor->SetOffset( vMousePosition - m_vCustomCursorHotSpot );
}

void Input::RemoveCustomCursor()
{
    if ( m_spCustomCursor == NULL )
    {
        return;
    }

    // Show system cursor
    GuiMain::GetInstance().GetCursor()->SetVisible( true );

    // Remove from hover layer
    GuiMain::GetInstance().GetLayer( CGui::LAYER_HOVER ).RemoveObject( m_spCustomCursor );
    // TODO Add to old parent again?

    m_spCustomCursor.reset();
}

void Input::OnDoubleClickedLeft()
{
    // Update focused object
    if ( m_spPointedObject == NULL ||
         !m_spPointedObject->IsDisabled() )
    {
        SetFocusObject( m_spPointedObject );
    }

    if ( m_spPointedObject != NULL )
    {
        // Inform pointed object
        m_spPointedObject->OnDoubleClicked();
    }
}

void Input::OnDoubleClickedRight()
{
}
