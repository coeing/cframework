#include "cdynamic/App.h"

#include "cdynamic/DynamicWindow.h"

const CCtrl::WindowController* CDynamic::App::CreateStartScreen() const
{
	return CCtrl::WindowController::CreateScreen<DynamicWindow>();
}