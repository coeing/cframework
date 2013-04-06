#include "cgui/GUI/ChildIterator.h"

#include "cgui/GUI/Panel.h"

using namespace CGui;

ChildIterator::ChildIterator( PanelPtr _spPanel )
:	m_spPanel( _spPanel ),
	m_u32Idx( 0 )
{
}

void ChildIterator::operator=( const ChildIterator &rhs )
{
	m_spPanel = rhs.m_spPanel;
	m_u32Idx = rhs.m_u32Idx;
}

GuiObjectPtr ChildIterator::GetFirst()
{
	m_u32Idx = 0;
	return m_spPanel->GetChild( m_u32Idx );
}

GuiObjectPtr ChildIterator::GetNext()
{
	return m_spPanel->GetChild( ++m_u32Idx );
}

const GuiObjectPtr ChildIterator::GetFirst() const
{
	m_u32Idx = 0;
	return m_spPanel->GetChild( m_u32Idx );
}

const GuiObjectPtr ChildIterator::GetNext() const
{
	return m_spPanel->GetChild( ++m_u32Idx );
}

bool ChildIterator::IsDone() const
{
	return m_u32Idx >= m_spPanel->GetNumChildren();
}

GuiObjectPtr ChildIterator::GetCurrent() const
{
	return m_spPanel->GetChild( m_u32Idx );
}
