#include "cgui/DragDrop/FreeDragController.h"

#include "cgui/GUI/GuiMain.h"
#include "cgui/GUI/Layer.h"
#include "cgui/GUI/Panel.h"

using namespace CGui;

FreeDragController::FreeDragController()
{
}

void FreeDragController::Start( GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vRelativeMousePosition )
{
    // Get position
    m_vOldPosition = _spObject->GetAbsolutePosition();

    // Get parent
    m_spOldParent = _spObject->GetParent();

    // Remove from parent
    _spObject->RemoveFromParent();

    // Add to foreground layer
    GuiMain::GetInstance().GetLayer( LAYER_HOVER ).AddObjectLast( _spObject );

    // Set offset
    _spObject->SetLocalAttachPoint( ATTACH_POINT_TOP_LEFT );
    _spObject->SetAttachObject( ATTACH_OBJECT_PARENT );
    _spObject->SetReferenceAttachPoint( ATTACH_POINT_TOP_LEFT );
    _spObject->SetOffset( m_vOldPosition );

    DragController::Start( _spObject, _vRelativeMousePosition );
}

void FreeDragController::Continue( const CFoundation::Vector2Di& _vMovement )
{
    GuiObjectPtr pObject = GetObject();
    if ( pObject != NULL )
    {
        CFoundation::Vector2Di clNewOffset = pObject->GetOffset() + _vMovement;
        pObject->SetOffset( clNewOffset );
    }
}

void FreeDragController::Undo() 
{
    GuiObjectPtr pObject = GetObject();
    if ( pObject != NULL )
    {
        // Remove from gui root
        GuiMain::GetInstance().GetLayer( LAYER_HOVER ).RemoveObject( pObject );

        // Add to old parent
        if ( m_spOldParent != NULL )
        {
            m_spOldParent->AddChildLast( pObject );

            // Set old offset
            pObject->SetOffset( m_vOldPosition - m_spOldParent->GetAbsolutePosition() );
        }
    }
}
        
void FreeDragController::End()
{
    GuiObjectPtr pObject = GetObject();
    if ( pObject != NULL )
    {
        // Remove from gui root
        GuiMain::GetInstance().GetLayer( LAYER_HOVER ).RemoveObject( pObject );
    }

    DragController::End();
}