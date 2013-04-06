#ifndef TRASHDROPCONTROLLER_H
#define TRASHDROPCONTROLLER_H

#include "cfoundation/Utils/Singleton.h"
#include "cgui/DragDrop/DropController.h"

class TrashDropController : public CGui::DropController
{
public:
    /// Executes a drop action (the drag object is dropped on the drop object)
    virtual bool    Execute( CGui::GuiObjectPtr _spDragObject, CGui::DragDataPtr _spDragData, const CFoundation::Vector2Di& _vDragPosition, const CFoundation::Vector2Di& _vDropPosition );
};

#endif
