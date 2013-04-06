#include "cctrl/GuiRecorder.h"

#include "cctrl/FocusEvent.h"
#include "cctrl/MouseEvent.h"

using namespace CCtrl;

GuiRecorder::GuiRecorder( CGui::GuiObjectPtr _spObject )
:	m_bActive( false ),
	m_spObject( _spObject ),
	m_clCurRecord( _spObject )
{
	//clObject.AddListener( *this );
}

GuiRecorder::~GuiRecorder()
{
	//m_clObject.RemoveListener( *this );
}

void GuiRecorder::ProcessMessage( const CFoundation::Message& clMessage )
{
	if( !m_bActive )
	{
		return;
	}

	//const CGui::MouseEventMsg* pMouseEventMsg = clMessage.CastTo<CGui::MouseEventMsg>();
	//if( pMouseEventMsg != NULL )
	//{
 //       MouseEvent clEvent( pMouseEventMsg->GetObject() );
 //       clEvent.SetTimeStamp( CFoundation::Time() );
 //       clEvent.SetMousePosition( pMouseEventMsg->GetMousePosition() );
 //       clEvent.SetEvent( pMouseEventMsg->GetEvent() );
 //       m_clCurRecord.AddEvent( clEvent );
	//}

	const CGui::FocusMsg *pFocusMsg = clMessage.CastTo<CGui::FocusMsg>();
	if( pFocusMsg != NULL )
	{
		FocusEvent clEvent( pFocusMsg->GetObject() );
		clEvent.SetTimeStamp( CFoundation::Time() );
		clEvent.SetMousePosition( pFocusMsg->GetMousePosition() );
		clEvent.SetFocusType( pFocusMsg->GetFocusType() );
		m_clCurRecord.AddEvent( clEvent );
	}
}

void GuiRecorder::Start()
{
	m_clCurRecord.Clear();
	m_bActive = true;
}

void GuiRecorder::Stop()
{
	m_bActive = false;
}

void GuiRecorder::Continue()
{
	m_bActive = true;
}

bool GuiRecorder::IsActive() const
{
	return m_bActive;
}

const GuiRecord&	GuiRecorder::GetCurrentRecord() const
{
	return m_clCurRecord;
}
