#include "DragDropApp.h"

#include "MainWindow.h"

DragDropApp::DragDropApp()
{
}

void DragDropApp::CreateStartWindows()
{
}
		
const CFoundation::String DragDropApp::GetWindowCaption() const
{
    return "Drag & Drop Sample";
}
		
void DragDropApp::Exit()
{
    CCtrl::App::Exit();
}
		
bool DragDropApp::Init()
{
    CCtrl::App::Init();

	m_spWindow = WindowController::CreateWindow< MainWindow >();

    return true;
}