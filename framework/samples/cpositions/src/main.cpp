#include <iostream>

#include "cgui/Globals.h"

#include "cgui/Gui/Image.h"
#include "cgui/Gui/Layer.h"

int main()
{
    // Init the gui system
    CGui::Init( "Test Positioning with attach points", 1024, 768, CFoundation::Color( 1.0f, 0.9f, 0.9f ) );

    const CGui::GuiMain& clGuiMain = CGui::GuiMain::GetInstance();

    // Red rect
    CGui::ImagePtr spTemplateImage1 = clGuiMain.Create<CGui::Image>( "ImgTemplate1" );
    spTemplateImage1->SetBgColor( CFoundation::Color( 0xff0000ff ) );
    spTemplateImage1->SetCustomSize( CFoundation::Vector2Du( 30, 30 ) );
    
    // Blue rect
    CGui::ImagePtr spTemplateImage2 = clGuiMain.Create<CGui::Image>( "ImgTemplate2" );
    spTemplateImage2->SetBgColor( CFoundation::Color( 0x00ff00ff ) );
    spTemplateImage2->SetCustomSize( CFoundation::Vector2Du( 10, 10 ) );
    spTemplateImage2->SetAttachObject( CGui::ATTACH_OBJECT_PREV_SIBLING );

    for ( Unsigned32 u32LocalPoint = CGui::ATTACH_POINT_INVALID + 1; u32LocalPoint != CGui::NUM_ATTACH_POINTS; ++u32LocalPoint )
    {
        for ( Unsigned32 u32ReferencePoint = CGui::ATTACH_POINT_INVALID + 1; u32ReferencePoint != CGui::NUM_ATTACH_POINTS; ++u32ReferencePoint )
        {
            CGui::ImagePtr spImage1 = clGuiMain.CreateFromTemplate( "Img1", spTemplateImage1 );
            CGui::ImagePtr spImage2 = clGuiMain.CreateFromTemplate( "Img2", spTemplateImage2 );

            spImage2->SetLocalAttachPoint( static_cast< CGui::AttachPoint >( u32LocalPoint ) );
            spImage2->SetReferenceAttachPoint( static_cast< CGui::AttachPoint >( u32ReferencePoint ) );

            //clImage1.AddChildFirst( clImage2 );
            spImage1->SetOffset( CFoundation::Vector2Di( u32LocalPoint * 50, u32ReferencePoint * 50 ) );

            clGuiMain.GetLayer( CGui::LAYER_MAIN ).AddObjectLast( spImage1 );
            clGuiMain.GetLayer( CGui::LAYER_MAIN ).AddObjectLast( spImage2 );
        }
    }

    // Run
    CGui::Run();

    return 0;
}
