#include <iostream>

#include "cgui/Globals.h"

#include "cgui/Gui/Image.h"
#include "cgui/Gui/Layer.h"
#include "cgui/Gui/Panel.h"

#include "cgui/Gui/SizeMode.h"

int main()
{
    // Init the gui system
    CGui::Init( "Test size modes", 1024, 768, CFoundation::Color( 1.0f, 0.9f, 0.9f ) );

    const CGui::GuiMain& gGuiMain = CGui::GuiMain::GetInstance();

    // Blue parent rect
    CGui::PanelPtr spParentTemplate1 = gGuiMain.Create< CGui::Panel >( "ImgTemplate1" );
    spParentTemplate1->SetBgColor( CFoundation::Color( 0x0000ffff ) );
    spParentTemplate1->SetCustomSize( 60, 60 );

    // Red parent rect
    CGui::PanelPtr spParentTemplate2 = gGuiMain.Create< CGui::Panel >( "ImgTemplate2" );
    spParentTemplate2->SetBgColor( CFoundation::Color( 0xff0000ff ) );
    spParentTemplate2->SetCustomSize( 30, 30 );
    
    // Green child rect
    CGui::ImagePtr spChildTemplate = gGuiMain.Create<CGui::Image>( "ImgTemplate3" );
    spChildTemplate->SetBgColor( CFoundation::Color( 0x00ff00ff ) );
    spChildTemplate->SetCustomSize( 10, 10 );
    spChildTemplate->SetAttachObject( CGui::ATTACH_OBJECT_PREV_SIBLING );

    for ( Unsigned32 u32SizeMode = CGui::SIZE_MODE_NONE + 1; u32SizeMode != CGui::SIZE_MODE_END; ++u32SizeMode )
    {
        CGui::PanelPtr spParent1 = gGuiMain.CreateFromTemplate( "Img1", spParentTemplate1 );

        CGui::PanelPtr spParent2 = gGuiMain.CreateFromTemplate( "Img1", spParentTemplate2 );
        spParent2->SetOffset( CFoundation::Vector2Di( 5, 5 ) );

        spParent1->AddChildLast( spParent2 );

        spParent2->SetSizeMode( static_cast< CGui::SizeMode >( u32SizeMode ) );

        CGui::ImagePtr spChild1 = gGuiMain.CreateFromTemplate( "Img2", spChildTemplate );
        spChild1->SetOffset( CFoundation::Vector2Di( -2, -2 ) );
        CGui::ImagePtr spChild2 = gGuiMain.CreateFromTemplate( "Img3", spChildTemplate );
        spChild2->SetOffset( CFoundation::Vector2Di( 40, 40 ) );
        
        spParent2->AddChildLast( spChild1 );
        spParent2->AddChildLast( spChild2 );

        spParent1->SetOffset( CFoundation::Vector2Di( u32SizeMode * 100, 100 ) );

        gGuiMain.GetLayer( CGui::LAYER_MAIN ).AddObjectLast( spParent1 );
    }

    // Run
    CGui::Run();

    return 0;
}
