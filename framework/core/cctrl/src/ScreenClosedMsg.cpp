#include "cctrl/ScreenClosedMsg.h"

using namespace CCtrl;

ScreenClosedMsg::ScreenClosedMsg( WindowControllerPtr _spScreen )
:	m_spScreen( _spScreen )
{
}

const ScreenClosedMsg& ScreenClosedMsg::operator=( const ScreenClosedMsg& rhs )
{
    m_spScreen = rhs.m_spScreen;

    return *this;
}

WindowControllerPtr ScreenClosedMsg::GetScreen() const
{
	return m_spScreen;
}
