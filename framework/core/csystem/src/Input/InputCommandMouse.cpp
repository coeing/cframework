#include "csystem/Input/InputCommandMouse.h"

#include "cfoundation/Enum/EnumConverter.h"
#include "cfoundation/XML/XMLElement.h"

using namespace CSystem;
        
void InputCommandMouse::LoadFromXML( CFoundation::XMLElementCPtr _spElement )
{
    m_eButton = CFoundation::EnumConverter< MouseButton >::GetInstance().Convert( _spElement->GetAttribute( "button" ) );
    m_eActivation = CFoundation::EnumConverter< MouseButtonState >::GetInstance().Convert( _spElement->GetAttribute( "activation" ) );
}

bool InputCommandMouse::IsActive( const InputState& _state ) const
{
    return _state.m_eMouseButtonStates[ m_eButton ] == m_eActivation;
}
