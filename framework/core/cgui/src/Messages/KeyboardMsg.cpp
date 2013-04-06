#include "cgui/Messages/KeyboardMsg.h"

using namespace CGui;

KeyboardMsg::KeyboardMsg( GuiObjectPtr _spObject, const wchar_t& wChar )
:	GuiMessageTemplate( _spObject ),
    m_wChar( wChar )
{
}

const wchar_t& KeyboardMsg::GetChar() const
{
	return m_wChar;
}
