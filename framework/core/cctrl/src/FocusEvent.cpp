#include "cctrl/FocusEvent.h"

using namespace CCtrl;

FocusEvent::FocusEvent( CGui::GuiObjectCPtr _spObject )
:   GuiEvent( _spObject )
{
}

void FocusEvent::SetFocusType( CGui::FocusMsg::FocusType eFocusType )
{
	m_eFocusType = eFocusType;
}

CGui::FocusMsg::FocusType FocusEvent::GetFocusType() const
{
	return m_eFocusType;
}

CFoundation::String FocusEvent::GetName()
{
	CFoundation::String sName = "FocusEvent ";
	switch( m_eFocusType )
	{
		case CGui::FocusMsg::TYPE_GAINED:
			sName += "Gained";
			break;
		default:
			sName += "Lost";
			break;
	}
	return sName;
}

void FocusEvent::Save( CFoundation::Serializer &clSerializer ) const
{
	GuiEvent::Save( clSerializer );
	Unsigned32 u32FocusType = m_eFocusType;
	clSerializer.Add( u32FocusType );
}

void FocusEvent::Load( const CFoundation::Serializer &clSerializer )
{
	GuiEvent::Load( clSerializer );
	Unsigned32 u32FocusType = clSerializer.GetU32();
	m_eFocusType = static_cast<CGui::FocusMsg::FocusType>( u32FocusType );
}
