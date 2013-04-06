#include "cgui/DragDrop/FreeDropController.h"

#include "cgui/GUI/Drawer.h"
#include "cgui/GUI/GuiObject.h"
#include "cgui/GUI/Panel.h"

using namespace CGui;

FreeDropController::FreeDropController()
{
    //! Create callback
    m_spPostRenderDelegate = RenderEventDelegate1Ptr( new RenderEventDelegate1( this, &FreeDropController::OnPostRenderDropObject ) );
}

void FreeDropController::SetObject( GuiObjectPtr _spObject )
{
    // Get current object
    GuiObjectPtr pDropObject = GetObject();
    if ( _spObject == pDropObject )
    {
        return;
    }

    // Remove callback from current object
    if ( pDropObject != NULL )
    {
        pDropObject->RemoveDelegate( RENDER_EVENT_POST_RENDER, m_spPostRenderDelegate );
    }

    // Register callback to new object
    _spObject->RegisterDelegate( RENDER_EVENT_POST_RENDER, m_spPostRenderDelegate );

    DropController::SetObject( _spObject );
}

void FreeDropController::RemoveObject()
{
    // Get current object
    GuiObjectPtr pDropObject = GetObject();

    // Remove callback from current object
    if ( pDropObject != NULL )
    {
        pDropObject->RemoveDelegate( RENDER_EVENT_POST_RENDER, m_spPostRenderDelegate );
    }

    DropController::RemoveObject();
}

bool FreeDropController::Execute( GuiObjectPtr _spDragObject, DragDataPtr _spDragData, const CFoundation::Vector2Di& _vDragPosition, const CFoundation::Vector2Di& _vDropPosition )
{
    // Get drop object
    GuiObjectPtr pDropObject = GetObject();

    // Cast to panel
    PanelPtr pDropPanel = dynamic_pointer_cast< Panel >( pDropObject );
    if ( pDropPanel == NULL )
    {
        return false;
    }

    // Add drag object to drop panel
    pDropPanel->AddChildLast( _spDragObject );

    // Set right attach point
    _spDragObject->SetAttachObject( ATTACH_OBJECT_PARENT );
    _spDragObject->SetLocalAttachPoint( ATTACH_POINT_TOP_LEFT );
    _spDragObject->SetReferenceAttachPoint( ATTACH_POINT_TOP_LEFT );

    // Set offset
    _spDragObject->SetOffset( _vDropPosition - _vDragPosition );

    // Always successful
    return true;
}
       
void FreeDropController::OnPostRenderDropObject( GuiObjectPtr _spObject )
{
    // Get current object
    GuiObjectPtr pDropObject = GetObject();

    // Highlight current drop object
    CFoundation::RectI32 clHighlightRect = pDropObject->GetAbsoluteRect();
    Drawer::GetInstance().DrawRect( clHighlightRect, CFoundation::Color( 1.0f, 0.0f, 0.0f ), 1.0f );
}
