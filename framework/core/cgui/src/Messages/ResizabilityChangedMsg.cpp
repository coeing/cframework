#include "cgui/Messages/ResizabilityChangedMsg.h"

using namespace CGui;

ResizabilityChangedMsg::ResizabilityChangedMsg( GuiObjectPtr _spObject, bool bNewValue, bool bOldValue )
:	GuiMessageTemplate( _spObject ),
    m_bNewValue( bNewValue ),
	m_bOldValue( bOldValue )
{
}

bool ResizabilityChangedMsg::IsResizable() const
{
	return m_bNewValue;
}

bool ResizabilityChangedMsg::WasResizable() const
{
	return m_bOldValue;
}
