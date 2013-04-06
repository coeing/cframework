#include "cctrlhelloworld/TestWindow.h"

#include "csystem/Input/InputState.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/CheckBox.h"
#include "cgui/Gui/ListBox.h"

#include "cctrlhelloworld/TestApp.h"

using namespace CTest;

TestWindow::TestWindow()
{
	m_clHighlightColors[ 0 ] = CFoundation::Color( 1.0f, 1.0f, 0.0f );
	m_clHighlightColors[ 1 ] = CFoundation::Color( 0.0f, 1.0f, 0.0f );

    // Create delegates
    m_spDelegateClickedDirection = CGui::ObjectInputEventDelegate1Ptr( new CGui::ObjectInputEventDelegate1( this, &TestWindow::OnClickedDirection ) );
    m_spDelegateClickedExit = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &TestWindow::OnClickedExit ) );
    m_spDelegateChangeHighlightColor = CGui::CheckBoxEventDelegate0Ptr( new CGui::CheckBoxEventDelegate0( this, &TestWindow::OnChangeHighlightColor ) );
}

const CFoundation::String TestWindow::GetXMLFileName()
{
	return "./data/TestWindow.xml";
};

void TestWindow::InitWindow()
{
	m_spTbFirst		= GetTextBox( "TbTest1" );
	m_spBtUp	    = GetButton( "BtUp" );
    SAFE_CALL( m_spBtUp )->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedDirection );
	m_spBtDown		= GetButton( "BtDown" );
    SAFE_CALL( m_spBtDown )->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedDirection );
	m_spBtLeft		= GetButton( "BtLeft" );
    SAFE_CALL( m_spBtLeft )->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedDirection );
	m_spBtRight		= GetButton( "BtRight" );
    SAFE_CALL( m_spBtRight )->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedDirection );
	m_spBtExit		= GetButton( "BtExit" );
    SAFE_CALL( m_spBtExit )->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedExit );
	m_spLbFirst		= GetListBox( "LbTest1" );
	m_spChbChangeHighlightColor = GetCheckBox( "ChkChgHighlight" );
    SAFE_CALL( m_spChbChangeHighlightColor )->RegisterDelegate( CGui::EVENT_CHECKBOX_CHECK_CHANGED, m_spDelegateChangeHighlightColor );

	if( m_spLbFirst != NULL )
	{
		for( Unsigned16 u16Row = 0; u16Row < 4; ++u16Row )
		{
			for( Unsigned16 u16Col = 0; u16Col < 4; ++u16Col )
			{
				CFoundation::String sTmp;
				sTmp.Format( "( %d / %d )", u16Row + 1, u16Col + 1 );
				m_spLbFirst->AddColumnData( sTmp );
			}
			m_spLbFirst->AddRow();
		}

		const CFoundation::Color &clColor = m_spChbChangeHighlightColor != NULL && m_spChbChangeHighlightColor->IsChecked() ? m_clHighlightColors[ 0 ] : m_clHighlightColors[ 1 ];
		m_spLbFirst->SetHighlightColor( clColor );
		m_spLbFirst->SetSelectedRow( 0 );
	}
}
        
void TestWindow::DeinitWindow()
{
    // Remove delegates
    SAFE_CALL( m_spBtUp )->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedDirection );
    SAFE_CALL( m_spBtDown )->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedDirection );
    SAFE_CALL( m_spBtLeft )->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedDirection );
    SAFE_CALL( m_spBtRight )->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedDirection );
    SAFE_CALL( m_spBtExit )->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedExit );
    SAFE_CALL( m_spChbChangeHighlightColor )->RemoveDelegate( CGui::EVENT_CHECKBOX_CHECK_CHANGED, m_spDelegateChangeHighlightColor );
}
        
void TestWindow::ProcessInput( const CSystem::InputState& _State )
{
    if ( _State.m_eKeyStates[ CSystem::KEY_ESC ] == CSystem::KEY_STATE_FIRST_DOWN )
    {
        TestApp::GetInstance().Exit();
    }
}

void TestWindow::OnClickedDirection( CGui::GuiObjectPtr _spObject )
{
	if ( _spObject == m_spBtUp )
	{
        Integer32 i32SelectedRow = m_spLbFirst->GetSelectedRow();
		if ( i32SelectedRow > 0 )
		{
			m_spLbFirst->SetSelectedRow( i32SelectedRow - 1 );
		}
	}
	else if ( _spObject == m_spBtDown )
	{
        Integer32 i32SelectedRow = m_spLbFirst->GetSelectedRow();
		if ( i32SelectedRow < m_spLbFirst->GetNumRows() - 1 )
		{
			m_spLbFirst->SetSelectedRow( i32SelectedRow + 1 );
		}
	}
	else if ( _spObject == m_spBtLeft )
	{
        m_spLbFirst->SetOffsetX( m_spLbFirst->GetOffset().GetX() - 10 );
	}
	else if ( _spObject == m_spBtRight )
	{
        m_spLbFirst->SetOffsetX( m_spLbFirst->GetOffset().GetX() + 10 );
	}
}

void TestWindow::OnClickedExit()
{
    TestApp::GetInstance().Exit();
}

void TestWindow::OnChangeHighlightColor()
{
	const CFoundation::Color &clColor = m_spChbChangeHighlightColor->IsChecked() ? m_clHighlightColors[ 0 ] : m_clHighlightColors[ 1 ];
	m_spLbFirst->SetHighlightColor( clColor );
}
