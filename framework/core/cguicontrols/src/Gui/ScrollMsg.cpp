#include "cgui/Messages/ScrollMsg.h"

using namespace CGui;

ScrollMsg::ScrollMsg( GuiObjectPtr _spScrollBar, Float32 f32ScrollPosition, Unsigned32 u32ScrollOffset, ScrollBar::Orientation eOrientation )
:	GuiMessageTemplate( _spScrollBar ),
    m_f32ScrollPosition( f32ScrollPosition ),
	m_u32ScrollOffset( u32ScrollOffset ),
	m_eOrientation( eOrientation )
{
}

void ScrollMsg::SetScrollPosition( Float32 f32ScrollPosition )
{
	m_f32ScrollPosition = f32ScrollPosition;
}

Float32	ScrollMsg::GetScrollPosition() const
{
	return m_f32ScrollPosition;
}

void ScrollMsg::SetScrollOffset( Unsigned32 u32ScrollOffset )
{
	m_u32ScrollOffset = u32ScrollOffset;
}

Unsigned32 ScrollMsg::GetScrollOffset() const
{
	return m_u32ScrollOffset;
}

void ScrollMsg::SetOrientation( ScrollBar::Orientation eOrientation )
{
	m_eOrientation = eOrientation;
}

ScrollBar::Orientation ScrollMsg::GetOrientation() const
{
	return m_eOrientation;
}
