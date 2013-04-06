#include "MainWindow.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/DragDrop/FreeDragController.h"
#include "cgui/DragDrop/FreeDropController.h"
#include "TrashDropController.h"

MainWindow::MainWindow()
{
}

CFoundation::String MainWindow::GetXMLFileName()
{
	return "./data/MainWindow.xml";
};

void MainWindow::InitWindow()
{
    // Get drag & drop system and drag/drop controllers
    CGui::DragDropSystem& clDragDropSystem = CGui::DragDropSystem::GetInstance();

    // Get and setup drag objects
    CGui::PanelPtr pPnlDrop1 = GetPanel( "PnlDrop1" );
    if ( pPnlDrop1 != NULL )
    {
        clDragDropSystem.SetDropController( pPnlDrop1, CGui::DropControllerPtr( new CGui::FreeDropController() ) );
        for ( Unsigned32 u32ChildIdx = 0; u32ChildIdx < pPnlDrop1->GetNumChildren(); ++u32ChildIdx )
        {
            CGui::GuiObjectPtr pChild = pPnlDrop1->GetChild( u32ChildIdx );
            pChild->SetDraggable( true );
            clDragDropSystem.SetDragController( pChild, CGui::DragControllerPtr( new CGui::FreeDragController() ) );
        }
    }
    CGui::PanelPtr pPnlDrop2 = GetPanel( "PnlDrop2" );
    if ( pPnlDrop1 != NULL )
    {
        clDragDropSystem.SetDropController( pPnlDrop2, CGui::DropControllerPtr( new CGui::FreeDropController() ) );
    }

    // Setup trash can
    CGui::GuiObjectPtr pTrashCan = GetGuiObject( "ImgTrashCan" );
    if ( pTrashCan != NULL )
    {
        clDragDropSystem.SetDropController( pTrashCan, CGui::DropControllerPtr( new TrashDropController() ) );
    }
}
