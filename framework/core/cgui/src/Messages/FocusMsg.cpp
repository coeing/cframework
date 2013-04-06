#include "cgui/Messages/FocusMsg.h"

using namespace CGui;

FocusMsg::FocusMsg( GuiObjectPtr _spObject, FocusType eFocusType, const CFoundation::Vector2Di &clMousePos )
:	GuiMessageTemplate( _spObject ),
	m_eFocusType( eFocusType ),
	m_clMousePos( clMousePos )
{
}

FocusMsg::FocusMsg( const FocusMsg& rhs )
:   GuiMessageTemplate( rhs ),
    m_eFocusType( rhs.m_eFocusType ),
    m_clMousePos( rhs.m_clMousePos )
{
}

FocusMsg::FocusType FocusMsg::GetFocusType() const
{
	return m_eFocusType;
}

const CFoundation::Vector2Di& FocusMsg::GetMousePosition() const
{
	return m_clMousePos;
}
