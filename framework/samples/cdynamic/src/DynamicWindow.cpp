#include "cdynamic/DynamicWindow.h"

using namespace CDynamic;

void DynamicWindow::InitWindow()
{
	m_pPanel = GetPanel( "MainWindow" );

	// Test

	// Scrollbar anzeigen
	//ScrollBar &clScrollBar = gGuiMain.CreateDefault<ScrollBar>( "ScrollBar01" );
	//clScrollBar.SetPosition( 200, 200 );
	//clScrollBar.SetDimensions( 20, 100 );
	//clScrollBar.SetDraggable( true );
	//clScrollBar.SetResizable( true );

	//m_pPanel->AddChild( clScrollBar );

	//// Button anzeigen
	//Button &clButton = gGuiMain.CreateDefault<Button>( "Button01" );
	//clButton.SetPosition( 300, 300 );
	//clButton.SetDimensions( 200, 200 );
	//clButton.SetDraggable( true );
	//clButton.SetResizable( true );
	//
	//m_pPanel->AddChild( clButton );

	// Bild erstellen
	//Image &clImage = gGuiMain.CreateDefault<Image>( "Image01" );
	//m_pImage = &clImage;

	//// Bild hinzufügen
	//m_pPanel->AddChildLast( clImage );

	////// Border erstellen
	////Border &clBorder = gGuiMain.CreateDefault<Border>( "Image01Border" );
	////m_pBorder = &clBorder;
	////clBorder.SetSize( 5 );
	////clBorder.SetBgColor( 0xff0000 );

	////// Border zuweisen
	////clImage.SetBorder( clBorder );

	//// ScrollPanel erstellen
	//ScrollPanel &clScrollPanel = gGuiMain.CreateDefault<ScrollPanel>( "Image01ScrollPanel" );
	//clScrollPanel.SetPosition( 400, 400 );

	//// ScrollPanel zuweisen
	//clScrollPanel.SetReference( &clImage );
	//clScrollPanel.SetDimensions( 100, 100 );

	//clImage.SetDimensions( 200, 200 );
	//clImage.SetPosition( 200, 300 );
	//clImage.SetDraggable( true );
	//clImage.SetResizable( true );
	//clImage.SetTexture( "data/wir.tga" );

	// Scrolling Tests mit ListBox
	ListBox &clListBox = gGuiMain.CreateDefault<ListBox>( "ListBox01" );
	clListBox.SetPosition( 0, 200 );
	clListBox.SetVisibleDimensions( 200, 200 );
	clListBox.SetDraggable( true );
	clListBox.SetResizable( true );

	ListBoxColumn &clColumn1 = gGuiMain.CreateDefault<ListBoxColumn>( "ListBox01Col01" );
	ListBoxColumn &clColumn2 = gGuiMain.CreateDefault<ListBoxColumn>( "ListBox01Col02" );

	clColumn1.SetWidth( 150 );
	clColumn2.SetWidth( 150 );

	clColumn1.SetTitle( "TestSpalte" );
	clColumn2.SetTitle( "Nr.2" );

	clListBox.AddColumn( clColumn1 );
	clListBox.AddColumn( clColumn2 );

	clListBox.SetScrollingHoriz( CGui::SCROLLING_AUTO );
	clListBox.SetScrollingVert( CGui::SCROLLING_AUTO );

	m_pPanel->AddChildLast( clListBox );

	m_pListBox = &clListBox;
}
		
void DynamicWindow::OnKeyDown( const CGui::KeyboardEventMsg &clMessage )
{
	if( clMessage.GetTrigger() == CGui::KEY_B )
	{
		if( m_pImage->HasBorder() )
		{
			m_pImage->RemoveBorder();
		}
		else
		{
			m_pImage->SetBorder( *m_pBorder );
		}
	}
	if( clMessage.GetTrigger() == CGui::KEY_L )
	{
		if( m_pListBox != NULL )
		{
			m_pListBox->AddColumnData( "A" );
			m_pListBox->AddColumnData( 1 );
			m_pListBox->AddRow();
		}
	}
}