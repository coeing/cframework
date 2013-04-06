#include "cctrl/GuiRecord.h"
#include "cctrl/FocusEvent.h"
#include "cctrl/MouseEvent.h"
#include <iostream>

using namespace CCtrl;

GuiRecord::GuiRecord( CGui::GuiObjectCPtr _spObject )
:	m_spObject( _spObject )
{
}

GuiRecord::~GuiRecord()
{
}

void GuiRecord::Clear()
{
	m_vEvents.clear();
}

void GuiRecord::AddEvent( GuiEvent &clEvent )
{
	m_vEvents.push_back( clEvent.Clone() );

	// Debug Ausgabe
	std::cout << clEvent.GetTimeStamp().ToString() << " New Event " << clEvent.GetName() << " (" << clEvent.GetMousePosition().GetX() << ", " << clEvent.GetMousePosition().GetY() << ")" << std::endl;
}

Unsigned32 GuiRecord::GetNumEvents() const
{
	return m_vEvents.size();
}
GuiEvent* GuiRecord::GetEvent( Unsigned32 u32Idx )
{
	return m_vEvents.at( u32Idx );
}
const GuiEvent* GuiRecord::GetEvent( Unsigned32 u32Idx ) const
{
	return m_vEvents[ u32Idx ];
}

void GuiRecord::RemoveEvent( Unsigned32 u32Idx )
{
	//m_vEvents.( u32Idx );
}

void GuiRecord::Save( CFoundation::Serializer &clSerializer ) const
{
	Unsigned32 u32NumEvents = GetNumEvents();
	clSerializer.Add( u32NumEvents );
	for( Unsigned32 u32Idx = 0; u32Idx < u32NumEvents; ++u32Idx )
	{
		clSerializer.Add( GetEvent( u32Idx )->GetID() );
		GetEvent( u32Idx )->Save( clSerializer );
	}
}

void GuiRecord::Load( const CFoundation::Serializer &clSerializer )
{
	Unsigned32 u32NumEvents = clSerializer.GetU32();
	for( Unsigned32 u32Idx = 0; u32Idx < u32NumEvents; ++u32Idx )
	{
		GuiEvent *pEvent = NULL;
		Unsigned32 u32ID = clSerializer.GetU32();
		/*if( u32ID == MouseEvent::GetId() )
		{
			pEvent = new MouseEvent( m_spObject );
		}
		else */if( u32ID == FocusEvent::GetId() )
		{
			pEvent = new FocusEvent( m_spObject );
		}

		if( pEvent != NULL )
		{
			pEvent->Load( clSerializer );
			AddEvent( *pEvent );

			delete pEvent;
		}
	}
}
