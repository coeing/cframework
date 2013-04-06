#include "cgui/Process/GuiProcessImpl.h"

#include "csystem/Input/Input.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Input/Input.h"
#include "cgui/Tooltip/TooltipSystem.h"

using namespace CGui;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// GuiProcessImpl
///
////////////////////////////////////////////////////////////////////////////////////////////

GuiProcessImpl::GuiProcessImpl( CSystem::ProcessManagerPtr _spProcessManager )
:   ProcessImpl( _spProcessManager )
{
}

void GuiProcessImpl::Update( Float32 _f32TimeDifference )
{
    // Update the system input
    CSystem::Input::GetInstance().Update( _f32TimeDifference );

    // Update tooltip system
    CGui::TooltipSystem::GetInstance().Update( _f32TimeDifference );

    // Update gui input
	CGui::Input::GetInstance().Update( _f32TimeDifference );

    // Update GUI
	CGui::GuiMain::GetInstance().Update( _f32TimeDifference );

    // Let windows process input
    //WindowManager::GetInstance().ProcessInput( CSystem::Input::GetInstance().GetState() );

    // Update Window Manager
    //WindowManager::GetInstance().Update( _f32TimeDifference );

    // Render gui
	CGui::GuiMain::GetInstance().Render();
}
