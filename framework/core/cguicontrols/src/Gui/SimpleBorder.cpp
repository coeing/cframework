#include "cgui/Gui/SimpleBorder.h"

#include "cgui/Renderer/Elements/RenderElementPrimitives.h"

using namespace CGui;

///////////////////////////////////////////////////////////////////////////////
///
/// SimpleBorder
///
///////////////////////////////////////////////////////////////////////////////

SimpleBorder::SimpleBorder( const CFoundation::String& _strName )
:	Border( _strName )
{
}

const CFoundation::String SimpleBorder::GetTypeID()
{
	return "simpleborder";
}

const CFoundation::String SimpleBorder::GetTypeId() const
{
	return SimpleBorder::GetTypeID();
}
        
void SimpleBorder::OnPostRenderReference( GuiObjectPtr _spObject, const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements )
{
#ifdef MULTIPLE_RENDER_ELEMENTS
    // Add border as primitive
    RenderElementPrimitivesPtr spRenderElement( new RenderElementPrimitives() );
    spRenderElement->SetClippingPlane( _Params.GetRenderRect() );

    spRenderElement->SetType( RenderElementPrimitives::TYPE_LINES );
    spRenderElement->SetThickness( 2.0f );
    
    // Get reference rect
    const CFoundation::RectI32& rectObject = _spObject->GetAbsoluteRect();
    spRenderElement->AddPoint( rectObject.GetTopLeft() );
    spRenderElement->AddPoint( rectObject.GetTopRight() );
    spRenderElement->AddPoint( rectObject.GetTopRight() );
    spRenderElement->AddPoint( rectObject.GetBottomRight() );
    spRenderElement->AddPoint( rectObject.GetBottomRight() );
    spRenderElement->AddPoint( rectObject.GetBottomLeft() );
    spRenderElement->AddPoint( rectObject.GetBottomLeft() );
    spRenderElement->AddPoint( rectObject.GetTopLeft() );

    _aspRenderElements.push_back( spRenderElement );
#endif
}

const CFoundation::String SimpleBorderCreator::GetTypeId() const
{
	return SimpleBorder::GetTypeID();
}

GuiObjectPtr SimpleBorderCreator::CreateObject( const CFoundation::String &sName ) const
{
	return GuiObjectPtr( new SimpleBorder( sName ) );
}
