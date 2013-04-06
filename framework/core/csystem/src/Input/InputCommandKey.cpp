#include "csystem/Input/InputCommandKey.h"

#include "cfoundation/Enum/EnumConverter.h"
#include "cfoundation/XML/XMLElement.h"

using namespace CSystem;
        
void InputCommandKey::LoadFromXML( CFoundation::XMLElementCPtr _spElement )
{
    CFoundation::String strKey = _spElement->GetAttribute( "key" );
    strKey.ToUpper();
    m_eKey = CFoundation::EnumConverter< KeyCode >::GetInstance().Convert( strKey );
    m_eActivation = CFoundation::EnumConverter< KeyState >::GetInstance().Convert( _spElement->GetAttribute( "activation" ) );
}

bool InputCommandKey::IsActive( const InputState& _state ) const
{
    return _state.m_eKeyStates[ m_eKey ] == m_eActivation;
}
