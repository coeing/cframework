#include "cetris/windows/HighscoreScreen.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/EditBox.h"
#include "cgui/Gui/ListBox.h"
#include "cgui/Gui/TextBox.h"

#include "cgui/Input/Input.h"

#include "cetris/Game.h"
#include "cetris/OneGame.h"

using namespace Cetris;

const CFoundation::String HighscoreScreen::GetXMLFileName()
{
	return "data/windows/HighscoreScreen.xml";
}

HighscoreScreen::HighscoreScreen()
{    
    m_spDelegateClickedBtOk = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &HighscoreScreen::OnClickedBtOk ) );
    m_spDelegateClickedBtMenu = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &HighscoreScreen::OnClickedBtMenu ) );
}
       
HighscoreScreen::~HighscoreScreen()
{
}

void HighscoreScreen::InitWindow()
{
	m_spTbCongratulations	= GetTextBox( "TbCongratulations" );
	m_spTbName				= GetTextBox( "TbName" );
	m_spEbName				= GetEditBox( "EbName" );
	m_spBtOk			    = GetButton( "BtOk" );
    m_spBtOk->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedBtOk );

	m_spTbTop10				= GetTextBox( "TbTop10" );

	m_spLbHighscores	    = GetListBox( "LbHighscores" );

	m_spBtMenu				= GetButton( "BtMenu" );
    m_spBtMenu->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedBtMenu );

	NewHighscore( false );
	Update();
}
		
void HighscoreScreen::DeinitWindow()
{
    m_spBtMenu->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedBtMenu );
}
		
void HighscoreScreen::NewHighscore( bool bNewHighscore )
{
	EnableInput( bNewHighscore );
}

void HighscoreScreen::OnClickedBtOk()
{
    AddHighscore();
}

void HighscoreScreen::OnClickedBtMenu()
{
    Close();
}
		
//void HighscoreScreen::OnKeyDown( const CGui::KeyboardEventMsg &clMessage )
//{
//	if( clMessage.IsReceiver( m_spEbName ) )
//	{
//		if( clMessage.GetTrigger() == CGui::KEY_RETURN )
//		{
//			AddHighscore();
//		}
//	}
//}

void HighscoreScreen::EnableInput( bool bEnable )
{
	m_spTbCongratulations->SetVisible( bEnable );
	m_spTbName->SetVisible( bEnable );
	m_spEbName->SetVisible( bEnable );
    m_spBtOk->SetVisible( bEnable );
	m_spTbTop10->SetVisible( !bEnable );
    if ( bEnable )
    {
        CGui::Input::GetInstance().SetFocusObject( m_spEbName );
    }
}

void HighscoreScreen::AddHighscore()
{
	HighscoreList &clHighscoreList = Game::GetInstance().GetHighscoreList();
	clHighscoreList.AddHighscore( m_spEbName->GetText(), Game::GetInstance().GetRunningGame()->GetPoints() );

	EnableInput( false );
	Update();
}

void HighscoreScreen::Update()
{
	const HighscoreList &clHighscoreList = Game::GetInstance().GetHighscoreList();	

	// Fill listbox
	m_spLbHighscores->ClearContent();

	for( Unsigned8 u8Idx = 0; u8Idx < clHighscoreList.GetNumHighscores(); ++u8Idx )
	{
		m_spLbHighscores->AddColumnData( clHighscoreList.GetName( u8Idx ) );
		m_spLbHighscores->AddColumnData( static_cast<Integer64>( clHighscoreList.GetPoints( u8Idx ) ) );
		m_spLbHighscores->AddRow();
	}
}