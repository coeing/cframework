//#include <vld.h>

#include "cgui/Globals.h"

#include "cgui/GUI/Image.h"
#include "cgui/GUI/Layer.h"
#include "cgui/GUI/Panel.h"
#include "cgui/GUI/TextBox.h"
#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/DragDrop/FreeDropController.h"

int main()
{
    // Init the gui system
    CGui::Init( "Hello World", 800, 600 );

    // Get gui manager
    CGui::GuiMain& clGuiMain = CGui::GuiMain::GetInstance();

    // Create a panel
    CGui::PanelPtr spPanel = clGuiMain.CreateDefault< CGui::Panel >( "MainPanel" );
    spPanel->SetCustomSize( CFoundation::Vector2Du( 800, 600 ) );
    CGui::DragDropSystem::GetInstance().SetDropController( spPanel, CGui::DropControllerPtr( new CGui::FreeDropController() ) );
    
    // Create a text box
    CGui::TextBoxPtr spTextBox = clGuiMain.CreateDefault< CGui::TextBox >( "TbHelloWorld" );
    spTextBox->SetText( "Hello World!" );
    spTextBox->SetCustomSize( CFoundation::Vector2Du( 200, 50 ) );
    spTextBox->SetOffset( CFoundation::Vector2Di( 0, 0 ) );
    spTextBox->SetBgColor( CFoundation::Color( 0xff0000ff ) );
    spTextBox->SetTextAlignment( CGui::ALIGN_HORIZONTAL, CGui::ALIGN_CENTER );
    spTextBox->SetTextAlignment( CGui::ALIGN_VERTICAL, CGui::ALIGN_MIDDLE );
    spTextBox->SetDraggable( true );
    spTextBox->SetResizable( true );

    // Add the text box
    spPanel->AddChildLast( spTextBox );

    // Create an image
    CGui::ImagePtr spImage = clGuiMain.CreateDefault< CGui::Image >( "ImgTest" );
    spImage->SetCustomSize( CFoundation::Vector2Du( 200, 200 ) );
    spImage->SetOffset( CFoundation::Vector2Di( 300, 50 ) );
    spImage->SetTexture( "data/tux.tif" );
    spImage->SetTextureRect( CFoundation::RectF16( CFoundation::Vector2Df( 0, 0 ), CFoundation::Vector2Df( 256, 256 ) ) );
    spImage->SetDraggable( true );
    spImage->SetResizable( true );

    // Add the image
    spPanel->AddChildLast( spImage );

    // Add panel
    clGuiMain.GetLayer( CGui::LAYER_MAIN ).AddObjectFirst( spPanel );

    // Run
    CGui::Run();

    // Deinitialize gui
    CGui::Deinit();

    return 0;
}
