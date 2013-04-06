#include "cgui/DragDrop/ProxyDragController.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/GuiObject.h"
#include "cgui/Gui/Layer.h"

using namespace CGui;

ProxyDragController::ProxyDragController()
{
}

ProxyDragController::~ProxyDragController()
{
}

void ProxyDragController::Start( CGui::GuiObjectPtr _spObject, const CFoundation::Vector2Di& _vRelativeMousePosition )
{
    // Create proxy drag object
    m_spProxyObject = CreateProxyObject( _spObject );

    // Set offset
    const CFoundation::Vector2Di& vAbsolutePosition = _spObject->GetAbsolutePosition();
    m_spProxyObject->SetOffset( vAbsolutePosition + _vRelativeMousePosition + CFoundation::Vector2Di( 2, 2 ) );  

    // Add to foreground
    CGui::GuiMain::GetInstance().GetLayer( CGui::LAYER_HOVER ).AddObjectLast( m_spProxyObject );

    DragController::Start( _spObject, _vRelativeMousePosition );
}

void ProxyDragController::Continue( const CFoundation::Vector2Di& _vMovement )
{
    // Move proxy object
    m_spProxyObject->SetOffset( m_spProxyObject->GetOffset() + _vMovement );
}

void ProxyDragController::Undo()
{
    // Remove from foreground
    //CGui::GuiMain::GetInstance().GetLayer( CGui::LAYER_HOVER ).RemoveObject( m_spProxyObject );

    DragController::Undo();
}

void ProxyDragController::End()
{
    // Remove from foreground
    //if ( CGui::GuiMain::GetInstance().GetLayer( CGui::LAYER_HOVER ).IsChild( m_spProxyObject ) )
    {
        CGui::GuiMain::GetInstance().GetLayer( CGui::LAYER_HOVER ).RemoveObject( m_spProxyObject );
    }

    DragController::End();
}
