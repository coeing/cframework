#include "cctrl/GuiEvent.h"

using namespace CCtrl;

GuiEvent::GuiEvent( CGui::GuiObjectCPtr _spObject )
:   m_spObject( _spObject )
{
}

GuiEvent::GuiEvent( CGui::GuiObjectCPtr _spObject, const CFoundation::Time &clTimeStamp )
:	m_spObject( _spObject ),
    m_clTimeStamp( clTimeStamp )
{
}

GuiEvent::GuiEvent( CGui::GuiObjectCPtr _spObject, const CFoundation::Time &clTimeStamp, const CFoundation::Vector2Di &clMousePosition )
:	m_spObject( _spObject ),
    m_clTimeStamp( clTimeStamp ),
	m_clMousePosition( clMousePosition )
{
}

void GuiEvent::SetTimeStamp( const CFoundation::Time &clTimeStamp )
{
	m_clTimeStamp = clTimeStamp;
}

const CFoundation::Time& GuiEvent::GetTimeStamp() const
{
	return m_clTimeStamp;
}

void GuiEvent::SetMousePosition( const CFoundation::Vector2Di &clMousePosition )
{
	m_clMousePosition = clMousePosition;
}

const CFoundation::Vector2Di& GuiEvent::GetMousePosition() const
{
	return m_clMousePosition;
}

CFoundation::Message* GuiEvent::CreateMsg( CGui::GuiObjectPtr _spObject ) const
{
	CFoundation::Message *pMsg = MakeMsg( _spObject );
	FillMsg( pMsg );
	return pMsg;
}

void GuiEvent::FillMsg( CFoundation::Message *pMsg ) const
{
}

void GuiEvent::Save( CFoundation::Serializer &clSerializer ) const
{
	m_clTimeStamp.Save( clSerializer );
	m_clMousePosition.Save( clSerializer );
}

void GuiEvent::Load( const CFoundation::Serializer &clSerializer )
{
	m_clTimeStamp.Load( clSerializer );
	m_clMousePosition.Load( clSerializer );
}
