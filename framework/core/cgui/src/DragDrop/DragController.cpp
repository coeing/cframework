#include "cgui/DragDrop/DragController.h"

using namespace CGui;

DragController::DragController()
{
}

void DragController::Start( GuiObjectPtr _spObject, const CFoundation::Vector2Di& /*_vRelativeMousePosition*/ )
{
    m_spObject = _spObject;

    // Dispatch event
    DispatchEvent( EVENT_DRAG_STARTED, GetSharedPtr(), m_spObject, GetDragData() );
}

GuiObjectPtr DragController::GetObject() const
{
    return m_spObject;
}
        
bool DragController::IsDropValid( GuiObjectPtr _spObject )
{
    return true;
}
        
DragDataPtr DragController::GetDragData() const
{
    return DragDataPtr();
}
        
void DragController::Undo()
{
    // Dispatch event
    DispatchEvent( EVENT_DRAG_UNDONE, GetSharedPtr(), m_spObject, GetDragData() );
}
        
void DragController::End()
{
    // Dispatch event
    DispatchEvent( EVENT_DRAG_ENDED, GetSharedPtr(), m_spObject, GetDragData() );

    // Reset object
    m_spObject.reset();
}
     