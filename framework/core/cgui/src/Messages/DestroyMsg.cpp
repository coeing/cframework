#include "cgui/Messages/DestroyMsg.h"

using namespace CGui;

DestroyMsg::DestroyMsg( GuiObject& _clObject )
:	m_clObject( _clObject )
{
}
        
DestroyMsg::DestroyMsg( const DestroyMsg& _rhs )
:   MessageTemplate( _rhs ),
    m_clObject( _rhs.m_clObject )
{
}

GuiObject& DestroyMsg::GetObject() const
{
	return m_clObject;
}
