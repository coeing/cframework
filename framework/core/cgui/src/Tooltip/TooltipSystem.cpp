#include "cgui/Tooltip/ToolTipSystem.h"

#include "csystem/Input/Input.h"

#include "cgui/GUI/GuiMain.h"
#include "cgui/GUI/Layer.h"
#include "cgui/GUI/Panel.h"
#include "cgui/GUI/TextBox.h"
#include "cgui/Input/Input.h"
#include "cgui/Tooltip/TooltipController.h"

using namespace CGui;

TooltipSystem::TooltipSystem()
{
    m_spDelegatePointedObjectChanged = InputEventDelegatePtr( new InputEventDelegate( this, &TooltipSystem::OnPointedObjectChanged ) );
    Input::GetInstance().RegisterDelegate( INPUT_EVENT_POINTED_OBJECT_CHANGED, m_spDelegatePointedObjectChanged );
}
		
TooltipSystem::~TooltipSystem()
{
    Input::GetInstance().RemoveDelegate( INPUT_EVENT_POINTED_OBJECT_CHANGED, m_spDelegatePointedObjectChanged );
}

//void TooltipSystem::Init()
//{
    //m_pTbToolTip = gGuiMain.CreateDefault< TextBox >( "TbToolTip" );
    //m_pTbToolTip->SetCustomSize( CFoundation::Vector2Du( 200, 20 ) );
    //m_pTbToolTip->SetBgColor( STATE_NORMAL, CFoundation::Color( 0xffff99ff ) );
    //m_pTbToolTip->SetVisible( false );
    //m_pTbToolTip->SetCutType( TextBox::CUT_ABBR );
    //GuiMain::GetInstance().GetLayer( LAYER_HOVER ).AddObjectLast( m_pTbToolTip );
//}

void TooltipSystem::SetTooltipController( CGui::GuiObjectCPtr _spObject, TooltipControllerPtr _spTooltipController )
{
    m_mapTooltipControllers[ _spObject ] = _spTooltipController;
}

void TooltipSystem::RemoveTooltipController( CGui::GuiObjectCPtr _spObject )
{
    ObjectTooltipControllerMap::iterator it = m_mapTooltipControllers.find( _spObject );
    if ( it != m_mapTooltipControllers.end() )
    {
        m_mapTooltipControllers.erase( it );
    }
}

TooltipControllerPtr TooltipSystem::GetTooltipController( CGui::GuiObjectCPtr _spObject ) const
{
    ObjectTooltipControllerMap::const_iterator it = m_mapTooltipControllers.find( _spObject );
    if ( it != m_mapTooltipControllers.end() )
    {
        return it->second;
    }
    else
    {
        return TooltipControllerPtr();
    }
}

void TooltipSystem::Update( Float32 /*_f32TimeDifference*/ )
{
    // Update tooltip
    if ( m_spTooltipController != NULL )
    {
        m_spTooltipController->Continue( CSystem::Input::GetInstance().GetState().GetMouseMovement() );
    }

	//if ( m_pObject != NULL )
	//{
	//	if( m_clTimer.GetElapsedMs() > 500 &&
	//		m_pObject->GetToolTip() != "" )
	//	{
	//		CFoundation::Vector2Di clPosition = clMousePosition;
	//		clPosition.Add( 10, 10 );
	//		m_pTbToolTip->SetText( m_pObject->GetToolTip() );
	//		m_pTbToolTip->SetOffset( clPosition );
	//		m_pTbToolTip->SetVisible( true );
	//	}
	//	else
	//	{
	//		m_pTbToolTip->SetText( "" );
	//		m_pTbToolTip->SetVisible( false );
	//	}
	//}
	//else
	//{
	//	m_pTbToolTip->SetText( "" );
	//	m_pTbToolTip->SetVisible( false );
	//}
}
        
void TooltipSystem::OnPointedObjectChanged( GuiObjectPtr _spOldObject, GuiObjectPtr _spNewObject )
{
    // Find tooltip controller
    GuiObjectPtr spTooltipObject = _spNewObject;
    TooltipControllerPtr spTooltipController;
    while ( spTooltipObject != NULL )
    {
        spTooltipController = GetTooltipController( spTooltipObject );
        if ( spTooltipController != NULL )
        {
            break;
        }
        spTooltipObject = spTooltipObject->GetParent();
    }

    // Set new tooltip controller
    SetActiveTooltipController( spTooltipController, spTooltipObject );
}

void TooltipSystem::SetActiveTooltipController( TooltipControllerPtr _spTooltipController, GuiObjectPtr _spObject )
{
    if ( m_spTooltipController == _spTooltipController &&
         m_spTooltipObject == _spObject )
    {
        return;
    }    

    // End old tooltip
    if ( m_spTooltipController != NULL )
    {
        m_spTooltipController->End();
        m_spTooltipController.reset();
    }

    m_spTooltipController = _spTooltipController;
    m_spTooltipObject = _spObject;

    // Start tooltip
    if ( m_spTooltipController != NULL )
    {
        m_spTooltipController->Start( _spObject, CSystem::Input::GetInstance().GetState().m_vMousePosition );
    }
}

