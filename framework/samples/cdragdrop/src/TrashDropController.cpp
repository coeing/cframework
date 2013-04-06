#include "TrashDropController.h"

#include "cgui/GUI/GuiMain.h"

bool TrashDropController::Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr /*_spDragData*/, const CFoundation::Vector2Di& /*_vDragPosition*/, const CFoundation::Vector2Di& /*_vDropPosition*/ )
{
    // Remove drag object from gui
    CGui::GuiMain::GetInstance().Delete( _spDragObject );

    return true;
}