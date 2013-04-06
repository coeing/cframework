#include "cgui/Messages/DraggabilityChangedMsg.h"

#include "cgui/Messages/MessageID.h"

using namespace CGui;

DraggabilityChangedMsg::DraggabilityChangedMsg( GuiObjectPtr _spObject, bool bDraggable, bool bOldValue )
:	GuiMessageTemplate( _spObject ),
    m_bDraggable( bDraggable ),
	m_bOldValue( bOldValue )
{
}

bool DraggabilityChangedMsg::IsDraggable() const
{
	return m_bDraggable;
}

bool DraggabilityChangedMsg::WasDraggable() const
{
	return m_bOldValue;
}

