#include "cgui/Messages/ParentChangedMsg.h"

using namespace CGui;

ParentChangedMsg::ParentChangedMsg( GuiObjectPtr _spObject, GuiObjectPtr _spNewParent, GuiObjectPtr _spOldParent )
:	GuiMessageTemplate( _spObject ),
	m_spOldParent( _spOldParent ),
	m_spNewParent( _spNewParent )
{
}

void ParentChangedMsg::SetOldParent( GuiObjectPtr _spOldParent )
{
	m_spOldParent = _spOldParent;
}

GuiObjectPtr ParentChangedMsg::GetOldParent() const
{
	return m_spOldParent;
}

void ParentChangedMsg::SetNewParent( GuiObjectPtr _spNewParent )
{
	m_spNewParent = _spNewParent;
}

GuiObjectPtr ParentChangedMsg::GetNewParent() const
{
	return m_spNewParent;
}
