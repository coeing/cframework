#include "csystem/Input/InputState.h"

using namespace CSystem;

InputState::InputState()
:   m_u32Events( 0 )
{
}

bool InputState::IsDown( MouseButton _eButton ) const
{
    return m_eMouseButtonStates[ _eButton ] == MBS_DOWN ||
           m_eMouseButtonStates[ _eButton ] == MBS_FIRST_DOWN;
}
        
bool InputState::IsDown( KeyCode _eKey ) const
{
    return m_eKeyStates[ _eKey ] == KEY_STATE_DOWN ||
           m_eKeyStates[ _eKey ] == KEY_STATE_FIRST_DOWN;
}
        
bool InputState::WasPressed( MouseButton _eButton ) const
{
    return m_eMouseButtonStates[ _eButton ] == MBS_FIRST_DOWN;
}
        
bool InputState::WasPressed( KeyCode _eKey ) const
{
    return m_eKeyStates[ _eKey ] == KEY_STATE_FIRST_DOWN;
}
       
CFoundation::Vector2Di InputState::GetMousePosition() const
{
    return m_vMousePosition;
}
        
CFoundation::Vector2Di InputState::GetMouseMovement() const
{
    return m_vMousePosition - m_vLastMousePosition;
}
