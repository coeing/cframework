#include "cmadtv/DragDrop/PresentDragController.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Layer.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/Input/Input.h"

#include "cmadtv/Data/Betty/Present.h"
#include "cmadtv/Windows/Supermarket/PresentObject.h"

using namespace CMadTV;

PresentDragController::PresentDragController( PresentPtr _spPresent, PlayerPtr _spPlayer )
:   m_spDragData( new PresentDragData() )
{
    // Create present object
    m_spPresentObject = PresentObject::Create( _spPresent, false, false );
    m_spPresentObject->ShowPrice( false );
    
    m_spDragData->m_spPresent = _spPresent;
    m_spDragData->m_spPlayer = _spPlayer;
}

PresentDragController::~PresentDragController()
{
}
        
CGui::DragDataPtr PresentDragController::GetDragData() const
{
    return m_spDragData;
}
        
void PresentDragController::SetPresent( PresentPtr _spPresent )
{
    m_spDragData->m_spPresent = _spPresent;
    m_spPresentObject->SetPresent( _spPresent );
}
        
void PresentDragController::SetPlayer( PlayerPtr _spPlayer )
{
    m_spDragData->m_spPlayer = _spPlayer;
}
        
CGui::GuiObjectPtr PresentDragController::CreateProxyObject( CGui::GuiObjectPtr _spObject ) const
{
    return m_spPresentObject->GetObject();
}
