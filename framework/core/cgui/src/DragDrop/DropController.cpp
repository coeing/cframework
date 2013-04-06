#include "cgui/DragDrop/DropController.h"

using namespace CGui;
        
DropController::DropController()
:   m_bEnabled( true )
{
}

void DropController::SetObject( GuiObjectPtr _spObject )
{
    m_spObject = _spObject;
}

void DropController::RemoveObject()
{
    m_spObject.reset();
}
        
GuiObjectPtr DropController::GetObject() const
{
    return m_spObject;
}
        
void DropController::SetEnabled( bool _bEnabled )
{
    m_bEnabled = _bEnabled;
}

bool DropController::IsEnabled() const
{
    return m_bEnabled;
}
