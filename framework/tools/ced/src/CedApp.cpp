#include "ced/CedApp.h"

#include "ced/MainWindow.h"

using namespace Ced;

const CFoundation::String CedApp::GetWindowCaption() const
{
	return "CEd";
}

void CedApp::CreateStartWindows()
{
    CCtrl::WindowController::CreateWindow< MainWindow >();
}

bool CedApp::Init()
{
	bool bSuccess = App::Init();

	return bSuccess;
}

void CedApp::Save( CFoundation::Serializer &clSerializer ) const
{
}

void CedApp::Load( const CFoundation::Serializer &clSerializer )
{
}

void CedApp::Exit()
{
	App::Exit();
}
