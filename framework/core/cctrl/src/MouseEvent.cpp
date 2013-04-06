#include "cctrl/MouseEvent.h"

using namespace CCtrl;

//MouseEvent::MouseEvent( CGui::GuiObjectCPtr clObject )
//:   GuiEvent( clObject )
//{
//}
//
//void MouseEvent::SetEvent( CGui::MouseEvent eEvent )
//{
//	m_eEvent = eEvent;
//}
//
//CGui::MouseEvent MouseEvent::GetEvent() const
//{
//	return m_eEvent;
//}
//
//void MouseEvent::SetTrigger( CGui::MouseButton eTrigger )
//{
//	m_eTrigger = eTrigger;
//}
//
//CGui::MouseButton MouseEvent::GetTrigger() const
//{
//	return m_eTrigger;
//}
//
//CFoundation::String MouseEvent::GetName()
//{
//	CFoundation::String sName = "MouseEvent ";
//	switch( m_eEvent )
//	{
//	case CGui::ME_ENTERED:
//		sName += "Entering";
//		break;
//	case CGui::ME_EXITED:
//		sName += "Exited";
//		break;
//	case CGui::ME_PRESSED:
//		sName += "Pressed";
//		break;
//	case CGui::ME_RELEASED:
//		sName += "Released";
//		break;
//	default:
//		sName += "Undefined";
//		break;
//	}
//	return sName;
//}
//
//void MouseEvent::Save( CFoundation::Serializer &clSerializer ) const
//{
//	GuiEvent::Save( clSerializer );
//	clSerializer.AddData( &m_eEvent, sizeof( CGui::MouseEvent ) );
//}
//
//void MouseEvent::Load( const CFoundation::Serializer &clSerializer )
//{
//	GuiEvent::Load( clSerializer );
//	m_eEvent = *static_cast<const CGui::MouseEvent*>( clSerializer.GetData( sizeof( CGui::MouseEvent ) ) );
//}
