#include <iostream>

#include "cfoundation/Debug/Profiling.h"

#include "csystem/Files/Directory.h"

#include "cgui/Globals.h"
#include "cgui/GuiControls.h"

#include "cgui/Gui/CtrlStyle.h"
#include "cgui/Gui/GuiStyle.h"
#include "cgui/Gui/SizeMode.h"

#include "cgui/Gui/BorderFactory.h"
#include "cgui/Gui/CheckBox.h"
#include "cgui/Gui/ComboBox.h"
#include "cgui/Gui/EditBox.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/Layer.h"
#include "cgui/Gui/ListBox.h"
#include "cgui/Gui/ListBoxColumn.h"
#include "cgui/Gui/ScaleImage.h"
#include "cgui/Gui/ScrollBar.h"
#include "cgui/Gui/ScrollPanel.h"
#include "cgui/Gui/TextBox.h"
#include "cgui/GUI/Tree.h"
#include "cgui/GUI/TreeNode.h"
#include "cgui/Gui/Window.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/DragDrop/FreeDropController.h"

using namespace std;

enum ETestFlags
{
	TEST_TEXTBOX = 1,
	TEST_IMAGE = 2,
	TEST_BORDER = 4,
	TEST_SCALE_IMAGE = 8,
	TEST_BUTTON = 16,
	TEST_EDITBOX = 32,
	TEST_LISTBOX = 64,
	TEST_COMBOBOX = 128,
	TEST_CHECKBOX = 256,
	TEST_SCROLLBAR = 1 << 9,
	TEST_SCROLLPANEL = 1 << 10,
    TEST_TREE = 1 << 11,
};

// Adds the contents of the passed directory to the passed tree under the passed parent node
void AddContents( CSystem::DirectoryPtr _spDirectory, CGui::TreePtr _spTree, Unsigned32 _u32Parent )
{
    if ( _spDirectory == NULL )
    {
        return;
    }

    // Add sub directories
    const std::vector< CSystem::DirectoryPtr >& aspSubDirectories = _spDirectory->GetSubDirectories();
    Unsigned32 u32NumSubDirectories = aspSubDirectories.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumSubDirectories; ++u32Idx )
    {
        CSystem::DirectoryPtr spSubDirectory = aspSubDirectories[ u32Idx ];

        // Add sub directory
        Unsigned32 u32NodeIdx = _spTree->CreateNode( _u32Parent );
        CGui::TreeNodePtr spNode = _spTree->GetNode( u32NodeIdx );
        if ( spNode == NULL )
        {
            continue;
        }

        // Set name
        spNode->SetText( spSubDirectory->GetName() );

        // Add contents
        AddContents( spSubDirectory, _spTree, u32NodeIdx );
    }

    /*
    // Add files
    const std::vector< CSystem::FilePtr >& aspFiles = _spDirectory->GetSubDirectories();
    Unsigned32 u32NumSubDirectories = aspSubDirectories.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumSubDirectories; ++u32Idx )
    {
        // Add sub directory
        Unsigned32 u32NodeIdx = spTree->CreateNode( _u32Parent );
        CGui::TreeNodePtr spNode = spTree->GetNode( u32NodeIdx );
        if ( spTopic == NULL )
        {
            continue;
        }

        // Add contents
        AddContents( spSubDirectory, _spTree, u32NodeIdx );
    }*/
}

int main()
{
    // Init the gui system
    CGui::Init( "Test Controls", 800, 600, CFoundation::Color( 1.0f, 1.0f, 1.0f, 1.0f ) );

    // Init controls
    CGui::InitControls();

    // Get gui manager
    CGui::GuiMain& clGuiMain = CGui::GuiMain::GetInstance();

    // Create a default window
    CGui::WindowPtr spWindow = clGuiMain.CreateDefault< CGui::Window >( "MainWindow" );
    spWindow->SetCustomSize( CFoundation::Vector2Du( 800, 600 ) );
    spWindow->SetBgColor( CFoundation::Color( 1.0f, 1.0f, 1.0f, 1.0f ) );
    CGui::DragDropSystem::GetInstance().SetDropController( spWindow, CGui::FreeDropControllerPtr( new CGui::FreeDropController() ) );

    // Set flags who should be tested
    Unsigned32 u32TestFlags = TEST_COMBOBOX;
    //Unsigned32 u32TestFlags = TEST_CHECKBOX | TEST_BUTTON | TEST_EDITBOX | TEST_LISTBOX;
    //Unsigned32 u32TestFlags = /*TEST_IMAGE | TEST_TEXTBOX | TEST_BUTTON |*/ TEST_EDITBOX | TEST_SCROLLBAR | TEST_SCROLLPANEL |TEST_LISTBOX;

    // setup the gui system
    clGuiMain.SetDefaultStyle( CGui::GuiStylePtr( new CGui::GuiStyle( "common/ctrls/styles/default/default_gui_style.xml" ) ) );

	if( u32TestFlags & TEST_TEXTBOX )
	{
		// Add a Textbox
		CGui::TextBoxPtr spTextBox = clGuiMain.CreateDefault< CGui::TextBox >( "TbHelloWorld" );
		spTextBox->SetText( "Hello World!" );
        spTextBox->SetCustomSize( CFoundation::Vector2Du( 200, 50 ) );
        spTextBox->SetBgColor( CFoundation::Color( 0xff0000ff ) );
        spTextBox->SetTextAlignment( CGui::ALIGN_HORIZONTAL, CGui::ALIGN_CENTER );
        spTextBox->SetTextAlignment( CGui::ALIGN_VERTICAL, CGui::ALIGN_MIDDLE );
		spTextBox->SetDraggable( true );
		spTextBox->SetResizable( true );

        spWindow->AddChildLast( spTextBox );
	}

	if( u32TestFlags & TEST_IMAGE )
	{
        // Add a test square
        CGui::ImagePtr spSquare = clGuiMain.CreateDefault<CGui::Image>( "ImgBlock" );
        spSquare->SetBgColor( CFoundation::Color( 0.0f, 0.0f, 0.0f ) );
        spSquare->SetCustomSize( CFoundation::Vector2Du( 100, 100 ) );
        spSquare->SetOffset( CFoundation::Vector2Di( 200, 200 ) );

        spWindow->AddChildLast( spSquare );

		// Add an Image
		CGui::ImagePtr spImage = clGuiMain.CreateDefault< CGui::Image >( "ImgTest" );
		spImage->SetCustomSize( CFoundation::Vector2Du( 200, 200 ) );
		spImage->SetOffset( CFoundation::Vector2Di( 0, 50 ) );
		spImage->SetTexture( "data/tux.png" );
		spImage->SetDraggable( true );
		spImage->SetResizable( true );

		spWindow->AddChildLast( spImage );
	}

	if( u32TestFlags & TEST_BORDER )
	{
		// Add an Image
		CGui::ImagePtr spImage = clGuiMain.CreateDefault< CGui::Image >( "ImgTest" );
		spImage->SetTexture( "data/tux2.png" );

		// Add a Border
		/*CGui::BorderPtr spBorder = gBorderFactory.CreateBorder( CGui::BORDER_SIMPLE );
		spBorder->SetBgColor( CFoundation::Color( 0.0f, 1.0f, 0.0f ) );
		spBorder->SetThickness( 5 );
		spBorder->SetCustomSize( CFoundation::Vector2Du( 200, 200 ) );
		spBorder->SetReference( spImage );

		spWindow->AddChildLast( spBorder );*/
	}

	if( u32TestFlags & TEST_SCALE_IMAGE )
	{
		// Add a scaleable Image
		CGui::ScaleImagePtr spScaleImage = clGuiMain.CreateDefault<CGui::ScaleImage>( "ScaleImgTest" );
		clGuiMain.InitFromXML( spScaleImage, "common/ctrls/scaleimage.xml" );
		spScaleImage->SetSize( CFoundation::Vector2Du( 200, 200 ) );
		spScaleImage->SetOffset( CFoundation::Vector2Di( 200, 50 ) );
		/*clScaleImage.SetTexture( CGui::ScaleImage::TOP_LEFT, "common/art/ctrls/buttons/wbtl.png" );
		clScaleImage.SetTexture( CGui::ScaleImage::TOP_CENTER, "common/art/ctrls/buttons/wbtm.png" );
		clScaleImage.SetTexture( CGui::ScaleImage::TOP_RIGHT, "common/art/ctrls/buttons/wbtr.png" );
		clScaleImage.SetTexture( CGui::ScaleImage::MIDDLE_LEFT, "common/art/ctrls/buttons/wbtl.png" );
		clScaleImage.SetTexture( CGui::ScaleImage::MIDDLE_CENTER, "common/art/ctrls/buttons/wbmm.png" );
		clScaleImage.SetTexture( CGui::ScaleImage::MIDDLE_RIGHT, "common/art/ctrls/buttons/wbmr.png" );
		clScaleImage.SetTexture( CGui::ScaleImage::BOTTOM_LEFT, "common/art/ctrls/buttons/wbbl.png" );
		clScaleImage.SetTexture( CGui::ScaleImage::BOTTOM_CENTER, "common/art/ctrls/buttons/wbbm.png" );
		clScaleImage.SetTexture( CGui::ScaleImage::BOTTOM_RIGHT, "common/art/ctrls/buttons/wbbr.png" );
		clScaleImage.SetDraggable( true );
		clScaleImage.SetResizable( true );*/

		//clGuiMain.AddChildLast( clScaleImage );

        CGui::ScaleImagePtr spScaleImageDown = clGuiMain.CreateDefault< CGui::ScaleImage >( "ScaleImgTest" );
        clGuiMain.InitFromXML( spScaleImageDown, "common/ctrls/scaleimage_down.xml" );
	}

	if( u32TestFlags & TEST_BUTTON )
	{
		// Add a button
		CGui::ButtonPtr spButton = clGuiMain.Create<CGui::Button>( "BtTest" );
		spButton->SetCustomSize( CFoundation::Vector2Du( 200, 50 ) );
		//spButton->SetPosition( 0, 0 );
		spButton->SetText( "MyFirstButton" );
		spButton->SetDraggable( true );
		spButton->SetResizable( true );
        spButton->SetStyle( CGui::CtrlStylePtr( new CGui::CtrlStyle( "common/ctrls/styles/windows/windows_button_style.xml" ) ) );
		spButton->SetBgColor( CFoundation::Color( 0.0f, 1.0f, 0.0f ) );

		spWindow->AddChildLast( spButton );

		// Add a second button
		CGui::ButtonPtr spButton2 = clGuiMain.Create<CGui::Button>( "BtTest2" );
		spButton2->SetCustomSize( CFoundation::Vector2Du( 100, 100 ) );
		spButton2->SetText( "SecondButton" );
		spButton2->SetDraggable( true );
		spButton2->SetResizable( true );
        spButton2->SetStyle( CGui::CtrlStylePtr( new CGui::CtrlStyle( "common/ctrls/styles/default/default_button_style.xml" ) ) );
        spButton2->SetBgColor( CFoundation::Color( 1.0f, 0, 0 ) );

		spWindow->AddChildLast( spButton2 );
	}

	if( u32TestFlags & TEST_EDITBOX )
	{
		// Add an EditBox
		CGui::EditBoxPtr spEditBox = clGuiMain.CreateDefault<CGui::EditBox>( "EbTest" );
		spEditBox->SetCustomSize( CFoundation::Vector2Du( 200, 50 ) );
		spEditBox->SetOffset( CFoundation::Vector2Di( 0, 300 ) );
		spEditBox->SetDraggable( true );
		spEditBox->SetResizable( true );
		spEditBox->SetText( "Welcome" );

		spWindow->AddChildLast( spEditBox );
	}

	if( u32TestFlags & TEST_CHECKBOX )
	{
		// Add a CheckBox
		CGui::CheckBoxPtr spCheckBox = clGuiMain.Create<CGui::CheckBox>( "ChkTest" );
		spCheckBox->SetStyle( CGui::CtrlStylePtr( new CGui::CtrlStyle( "common/ctrls/styles/default/default_checkbox_style.xml" ) ) );
		spCheckBox->SetCustomSize( CFoundation::Vector2Du( 200, 50 ) );
		spCheckBox->SetOffset( CFoundation::Vector2Di( 0, 300 ) );
		spCheckBox->SetDraggable( true );
		spCheckBox->SetResizable( true );
		spCheckBox->SetText( "Test" );

		spWindow->AddChildLast( spCheckBox );

		// Add a second CheckBox
		CGui::CheckBoxPtr spCheckBox2 = clGuiMain.Create<CGui::CheckBox>( "ChkTest2" );
		spCheckBox2->SetStyle( CGui::CtrlStylePtr( new CGui::CtrlStyle( "common/ctrls/styles/windows/windows_checkbox_style.xml" ) ) );
		spCheckBox2->SetCustomSize( CFoundation::Vector2Du( 50, 13 ) );
		spCheckBox2->SetOffset( CFoundation::Vector2Di( 200, 400 ) );
		spCheckBox2->SetDraggable( true );
		spCheckBox2->SetResizable( true );
		spCheckBox2->SetText( "Test 2" );

		spWindow->AddChildLast( spCheckBox2 );

        /*
		// Add a checkbox button
		CGui::Button& clCheckBoxButton = clGuiMain.CreateDefault<CGui::Button>( "ChkBtTest" );
		clCheckBoxButton.SetCustomSize( 200, 50 );
		clCheckBoxButton.SetOffset( 200, 300 );
		clCheckBoxButton.SetDraggable( true );
		clCheckBoxButton.SetResizable( true );
		CGui::CtrlStyle clStyle( "common/ctrls/styles/default/default_toggle_button_style.xml" );
		clCheckBoxButton.SetStyle( clStyle );

		clGuiMain.AddObject( clCheckBoxButton );
        */
	}

	if( u32TestFlags & TEST_LISTBOX )
	{
		// Add a ListBox
		CGui::ListBoxColumnPtr spColumn1 = clGuiMain.CreateDefault<CGui::ListBoxColumn>( "LbTestColumn1" );
		spColumn1->SetTitle( "Column1" );
		CGui::ListBoxColumnPtr spColumn2 = clGuiMain.CreateDefault<CGui::ListBoxColumn>( "LbTestColumn2" );
		spColumn2->SetTitle( "Column2" );

		CGui::ListBoxPtr spListBox = clGuiMain.CreateDefault<CGui::ListBox>( "LbTest" );
		spListBox->AddColumn( spColumn1 );
		spListBox->AddColumn( spColumn2 );
		spListBox->SetCustomSize( CFoundation::Vector2Du( 200, 200 ) );
		spListBox->SetOffset( CFoundation::Vector2Di( 200, 0 ) );
		spListBox->SetDraggable( true );
		spListBox->SetResizable( true );
		//spListBox->SetRowHeight( 20 );
		//spListBox->SetHeaderVisible( false );

		spWindow->AddChildLast( spListBox );

		// Fill ListBox
		for( char cIdx = 'A'; cIdx < 'F'; ++cIdx )
		{
			for( Unsigned16 u16Col = 0; u16Col < spListBox->GetNumColumns(); ++u16Col )
			{
				CFoundation::String sData = cIdx;
				spListBox->AddColumnData( sData );
			}
			spListBox->AddRow();
		}
	}

	if( u32TestFlags & TEST_COMBOBOX )
	{
		// Add a ComboBox
		CGui::ComboBoxPtr spComboBox = clGuiMain.CreateDefault<CGui::ComboBox>( "CbTest" );
		//spComboBox->SetStyle( CGui::CtrlStylePtr( new CGui::CtrlStyle( "common/ctrls/styles/windows/windows_combobox_style.xml" ) ) );
		spComboBox->SetCustomSize( CFoundation::Vector2Du( 200, 250 ) );
		spComboBox->SetOffset( CFoundation::Vector2Di( 0, 350 ) );
		spComboBox->SetDraggable( true );
		spComboBox->SetResizable( true );

		spWindow->AddChildLast( spComboBox );

		// Fill ComboBox
		for( Unsigned32 u32Idx = 0; u32Idx < 7; ++u32Idx )
		{
            CFoundation::String sData( "Entry " );
            sData += CFoundation::String( u32Idx );
            spComboBox->AddItem( sData, u32Idx );
		}
	}

	if( u32TestFlags & TEST_SCROLLBAR )
	{
		// Add a scroll panel
        CGui::ScrollBarPtr spScrollBar = clGuiMain.CreateDefault< CGui::ScrollBar >( "ScrTest" );
		spScrollBar->SetCustomSize( CFoundation::Vector2Du( 200, 200 ) );
		spScrollBar->SetOffset( CFoundation::Vector2Di( 0, 500 ) );
        spScrollBar->SetBgColor( CFoundation::Color( 1.0f, 0.0f, 0.0f ) );
		spScrollBar->SetDraggable( true );
		spScrollBar->SetResizable( true );

		spWindow->AddChildLast( spScrollBar );
	}

	if( u32TestFlags & TEST_SCROLLPANEL )
	{
		// Add a scroll panel
        CGui::ScrollPanelPtr spScrollPanel = clGuiMain.CreateDefault< CGui::ScrollPanel >( "PnlScrollTest" );
		spScrollPanel->SetCustomSize( CFoundation::Vector2Du( 200, 200 ) );
		spScrollPanel->SetOffset( CFoundation::Vector2Di( 500, 0 ) );
        spScrollPanel->SetBgColor( CFoundation::Color( 1.0f, 0.0f, 0.0f ) );
		spScrollPanel->SetDraggable( true );
		spScrollPanel->SetResizable( true );
        spScrollPanel->ShowScrollBarHoriz( true );
        spScrollPanel->ShowScrollBarVert( true );

        // Add image to scroll panel
		CGui::ImagePtr spImage = clGuiMain.CreateDefault< CGui::Image >( "ImgTestScroll" );
		spImage->SetCustomSize( CFoundation::Vector2Du( 300, 300 ) );
		spImage->SetTexture( "data/tux.png" );

        // Add list box to scroll panel
		CGui::ListBoxColumnPtr spColumn1 = clGuiMain.CreateDefault<CGui::ListBoxColumn>( "LbTestColumn1" );
		spColumn1->SetTitle( "Column1" );
		CGui::ListBoxColumnPtr spColumn2 = clGuiMain.CreateDefault<CGui::ListBoxColumn>( "LbTestColumn2" );
		spColumn2->SetTitle( "Column2" );

		CGui::ListBoxPtr spListBox = clGuiMain.CreateDefault<CGui::ListBox>( "LbTest" );
        spListBox->SetSizeMode( CGui::SIZE_MODE_STRETCH );
		spListBox->AddColumn( spColumn1 );
		spListBox->AddColumn( spColumn2 );
		spListBox->SetCustomSize( CFoundation::Vector2Du( 200, 200 ) );
		spListBox->SetOffset( CFoundation::Vector2Di( 200, 0 ) );
		spListBox->SetDraggable( true );
		spListBox->SetResizable( true );

		// Fill ListBox
		for( char cIdx = 'A'; cIdx < 'F'; ++cIdx )
		{
			for( Unsigned16 u16Col = 0; u16Col < spListBox->GetNumColumns(); ++u16Col )
			{
				CFoundation::String sData = cIdx;
				spListBox->AddColumnData( sData );
			}
			spListBox->AddRow();
		}

        spScrollPanel->SetReference( spImage );
        //spScrollPanel->SetReference( spListBox );

		spWindow->AddChildLast( spScrollPanel );
	}

    if ( u32TestFlags & TEST_TREE )
    {
		// Add a tree
        CGui::TreePtr spTree = clGuiMain.Create< CGui::Tree >( "TreeTest" );
        spTree->StartInit();
		spTree->SetStyle( CGui::CtrlStylePtr( new CGui::CtrlStyle( "common/ctrls/styles/windows/windows_tree_style.xml" ) ) );
		spTree->SetCustomSize( CFoundation::Vector2Du( 200, 500 ) );
		spTree->SetOffset( CFoundation::Vector2Di( 0, 0 ) );
        spTree->SetBgColor( CFoundation::Color( 1.0f, 0.0f, 0.0f ) );

        // Set node template
        CGui::PanelPtr spPnlNodeTemplate = clGuiMain.Create< CGui::CombinedCtrl >( "PnlNodeTemplate" );
		spPnlNodeTemplate->SetStyle( CGui::CtrlStylePtr( new CGui::CtrlStyle( "common/ctrls/styles/windows/windows_treenode_style.xml" ) ) );

        spTree->SetNodeTemplate( spPnlNodeTemplate );

        // Get current directory
        CSystem::DirectoryPtr spDirectory( new CSystem::Directory( "D:\\Games" ) );

        CGui::TreeNodePtr spRoot = spTree->GetRoot();
        if ( spRoot != NULL )
        {
            spRoot->SetText( "D:\\Games" );
        }

        // Add contents
        AddContents( spDirectory, spTree, 0 );

        /*
        const char* astrTopics[] = { "Topic 1", "Topic 2" };
        const char* astrChapters[] = { "Chapter 1", "Chapter 2" };
        for ( Unsigned32 u32Idx = 0; u32Idx < sizeof( astrTopics ) / sizeof( const char* ); ++u32Idx )
        {
            Unsigned32 u32TopicNodeIdx = spTree->CreateNode( 0 );
            CGui::TreeNodePtr spTopic = spTree->GetNode( u32TopicNodeIdx );
            if ( spTopic == NULL )
            {
                continue;
            }

		    //spTopic->SetStyle( CGui::CtrlStylePtr( new CGui::CtrlStyle( "common/ctrls/styles/windows/windows_treenode_style.xml" ) ) );
            spTopic->SetText( astrTopics[ u32Idx ] );

            Unsigned32 u32ChapterNodeIdx = spTree->CreateNode( u32TopicNodeIdx );
            CGui::TreeNodePtr spChapter = spTree->GetNode( u32ChapterNodeIdx );
            if ( spChapter != NULL )
            {
		        //spChapter->SetStyle( CGui::CtrlStylePtr( new CGui::CtrlStyle( "common/ctrls/styles/windows/windows_treenode_style.xml" ) ) );
                spChapter->SetText( astrChapters[ u32Idx ] );
            }
        }*/

        spTree->EndInit();

		spWindow->AddChildLast( spTree );
    }

    // Add window
    clGuiMain.GetLayer( CGui::LAYER_MAIN ).AddObjectLast( spWindow );

    // Output profiling startup
    OutputProfiling();

    // Run
    CGui::Run();

    return 0;
}
