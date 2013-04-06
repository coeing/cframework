#include "cgui/GUI/GuiMain.h"

#include "cfoundation/DateTime/Timer.h"
#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Debug/Profiling.h"
#include "cfoundation/XML/XMLParser.h"

#include "cgui/GUI/GuiStyle.h"
#include "cgui/GUI/Layer.h"
#include "cgui/GUI/Panel.h"
#include "cgui/GUI/RenderWindow.h"
#include "cgui/Input/Cursor.h"
#include "cgui/Messages/DestroyMsg.h"
#include "cgui/Renderer/Renderer.h"
#include "cgui/Renderer/Elements/RenderElement.h"

using namespace CGui;

GuiMain::GuiMain()
{
	m_spRenderWindow = RenderWindowPtr( new RenderWindow() );
}

GuiMain::~GuiMain()
{
}

bool GuiMain::Init( CFoundation::String sCaption, Unsigned32 u32Width, Unsigned32 u32Height, const CFoundation::Color& clBgColor )
{
	m_spRenderWindow->Init( sCaption, u32Width, u32Height );

    // Create cursor
    m_spCursor = CursorPtr( new Cursor() );

    // Initialize layer
    for ( Unsigned32 u32LayerType = 0; u32LayerType < LAYER_END; ++u32LayerType )
    {
        LayerPtr spLayer( new Layer() );
        spLayer->SetSize( CFoundation::Vector2Du( u32Width, u32Height ) );

        if ( u32LayerType == LAYER_BG )
        {
            spLayer->SetBgColor( clBgColor );
        }
        else
        {
            spLayer->SetBgColor( CFoundation::Color( 0x00000000 ) );
        }

        m_aspLayers[ u32LayerType ] = spLayer;
    }

	m_clRenderParams.SetRenderWindow( m_spRenderWindow );
	m_clRenderParams.SetRenderRect( CFoundation::RectI32( 0, 0, m_spRenderWindow->GetWidth(), m_spRenderWindow->GetHeight() ) );

	return true;
}
        
void GuiMain::Deinit()
{
    for ( Unsigned32 u32LayerType = 0; u32LayerType < LAYER_END; ++u32LayerType )
    {
        LayerPtr spLayer = m_aspLayers[ u32LayerType ];
        spLayer->Deinit();
    }
}

void GuiMain::SetSize( Unsigned32 _u32Width, Unsigned32 _u32Height )
{
    if( _u32Height == 0 )
    {
        _u32Height = 1;
    }

    if ( m_spRenderer != NULL )
    {
        m_spRenderer->SetViewportSize( _u32Width, _u32Height );
    }
}

Unsigned32	GuiMain::GetWidth() const
{
	return m_spRenderWindow->GetWidth();
}

Unsigned32	GuiMain::GetHeight() const
{
	return m_spRenderWindow->GetHeight();
}

void GuiMain::Update( Float32 _f32TimeDifference )
{
    // Animate layers
    for ( Unsigned32 u32LayerType = 0; u32LayerType < LAYER_END; ++u32LayerType )
    {
        m_aspLayers[ u32LayerType ]->Update( _f32TimeDifference );
    }
}

RenderWindowPtr GuiMain::GetRenderWindow() const
{
    return m_spRenderWindow;
}
       
void GuiMain::SetRenderer( RendererPtr _spRenderer )
{
    m_spRenderer = _spRenderer;

    // Set size
    if ( m_spRenderer != NULL )
    {
        m_spRenderer->SetViewportSize( m_spRenderWindow->GetWidth(), m_spRenderWindow->GetHeight() );
    }
}

void GuiMain::Render()
{
	PROFILING( "GuiMain::Render()" );

    // Clear renderer
    m_spRenderer->PreRender();

    // Collect render elements
    static Unsigned32 u32NumRenderElements = 256;
#ifdef MULTIPLE_RENDER_ELEMENTS
    std::vector< RenderElementPtr > aspRenderElements;
#else
    std::vector< RenderElement > aspRenderElements;
    aspRenderElements.reserve( u32NumRenderElements );
#endif
    for ( Unsigned32 u32LayerType = 0; u32LayerType < LAYER_END; ++u32LayerType )
    {
        m_aspLayers[ u32LayerType ]->Render( m_clRenderParams, aspRenderElements );
    }

    if ( aspRenderElements.size() > u32NumRenderElements )
    {
        u32NumRenderElements *= 2;
    }

    // Render elements
    m_spRenderer->Render( aspRenderElements );

    // Update render window
	m_spRenderWindow->Update();
}
        
CursorPtr GuiMain::GetCursor() const
{
    return m_spCursor;
}

void GuiMain::SetDefaultStyle( GuiStylePtr _spStyle )
{
	m_spDefaultStyle = _spStyle;
}

GuiStyleCPtr GuiMain::GetDefaultStyle() const
{
    return m_spDefaultStyle;
}

void GuiMain::RegisterObjectCreator( const CFoundation::String& _strObjectType, GuiObjectCreatorPtr _spObjectCreator )
{
	CF_WARN_IF( m_aspGuiObjectCreators[ _strObjectType ] != NULL, L"Creator is been overwritten: %s", _strObjectType.wc_str() );

	m_aspGuiObjectCreators[ _strObjectType ] = _spObjectCreator;
}

void GuiMain::RemoveObjectCreator( const CFoundation::String& _strObjectType )
{
	m_aspGuiObjectCreators.erase( _strObjectType );
}

GuiObjectCreatorPtr GuiMain::GetObjectCreator( const CFoundation::String& _strObjectType ) const
{
	GuiObjectCreatorPtr spCreator;
	CreatorMap::const_iterator it = m_aspGuiObjectCreators.find( _strObjectType );
	if ( it != m_aspGuiObjectCreators.end() )
	{
		spCreator = it->second;
	}

	return spCreator;
}

GuiObjectPtr GuiMain::GetObjectAtPoint( const CFoundation::Vector2Di& _clPoint, CFoundation::Vector2Di& _clRelativePoint ) const
{
    for ( Integer32 i32LayerType = LAYER_END - 1; i32LayerType >= 0; --i32LayerType )
    {
        GuiObjectPtr pObject = m_aspLayers[ i32LayerType ]->GetObjectAtPoint( _clPoint, _clRelativePoint );
        if ( pObject != NULL )
        {
            return pObject;
        }
    }

    // No object found
    return GuiObjectPtr();
}

GuiObjectPtr GuiMain::Create( const CFoundation::String &sType, const CFoundation::String &sName ) const
{
    PROFILING( "GuiMain::Create" );

	GuiObjectCreatorPtr spCreator = GetObjectCreator( sType );
    if ( spCreator == NULL )
    {
        return GuiObjectPtr();
    }
	//CF_ASSERT( spCreator != NULL, L"Could not create Gui object %s of type %s, no creator.", sName.wc_str(), sType.wc_str() );

	GuiObjectPtr spObject = spCreator->CreateObject( sName );

	return spObject;
}

GuiObjectPtr GuiMain::CreateDefault( const CFoundation::String &sType, const CFoundation::String &sName ) const
{
    PROFILING( "GuiMain::CreateDefault" );

    GuiObjectPtr spObject = Create( sType, sName );

	// set style if exist
	if( m_spDefaultStyle != NULL )
	{
		CtrlStyleCPtr spCtrlStyle = m_spDefaultStyle->GetDefaultStyle( sType );
		if ( spCtrlStyle != NULL )
		{
			spObject->SetStyle( spCtrlStyle );
		}
	}

	return spObject;
}

GuiObjectPtr GuiMain::CreateFromXML( const CFoundation::String &sXMLFile ) const
{
	CFoundation::XMLParser &clParser = CFoundation::XMLParser::GetInstance();
    CFoundation::XMLDocumentPtr spDocument = clParser.ParseFile( sXMLFile );
    if ( spDocument == NULL )
    {
        CF_WARN( L"GuiMain::CreateFromXML: Could not parse xml file %s.", sXMLFile.wc_str() );
        return GuiObjectPtr();
    }

    CFoundation::XMLElementCPtr pXMLElement = spDocument->GetRootElement();
    return CreateFromXML( pXMLElement );
}

GuiObjectPtr GuiMain::CreateFromXML( CFoundation::XMLElementCPtr _spXMLElement ) const
{
    PROFILING( "GuiMain::CreateFromXML" );

	CF_ASSERT( _spXMLElement->IsValid(), L"GuiMain::CreateFromXML: No valid XML node." );

    const CFoundation::String& strType = _spXMLElement->GetName();
    const CFoundation::String& sName = _spXMLElement->GetAttribute( "name" );

	GuiObjectPtr spObject = Create( strType, sName );

	InitFromXML( spObject, _spXMLElement );

	return spObject;
}

GuiObjectPtr GuiMain::CreateFromTemplate( const CFoundation::String &sType, const CFoundation::String &sName, GuiObjectCPtr _spTemplate ) const
{
	GuiObjectCreatorPtr spCreator = GetObjectCreator( sType );
	CF_ASSERT( spCreator != NULL, L"Could not create Gui object of type %s, no creator.", sType.wc_str() );

	GuiObjectPtr spObject = spCreator->CreateObject( sName );
	InitFromTemplate( spObject, _spTemplate );

	return spObject;
}

void GuiMain::InitDefault( GuiObjectPtr _spObject ) const
{
	GuiObjectCreatorPtr spCreator = GetObjectCreator( _spObject->GetTypeId() );
	CF_ASSERT( spCreator != NULL, L"Could not create Gui object of type %s, no creator.", _spObject->GetTypeId().wc_str() );

	spCreator->InitDefault( _spObject );
}

void GuiMain::InitFromXML( GuiObjectPtr _spObject, const CFoundation::String &sXMLFile ) const
{
	CFoundation::XMLParser &clParser = CFoundation::XMLParser::GetInstance();
    CFoundation::XMLDocumentPtr spDocument = clParser.ParseFile( sXMLFile );
	if ( spDocument != NULL )
	{
        CFoundation::XMLElementCPtr spXMLElement = spDocument->GetRootElement();

        // Start initializing
        _spObject->StartInit();

	    InitFromXML( _spObject, spXMLElement );

        // End initializing
        _spObject->EndInit();
	}
}

void GuiMain::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
    PROFILING( "GuiMain::InitFromXML" );

    if ( _spObject == NULL ||
         _spXMLElement == NULL )
    {
        return;
    }

    //CFoundation::String sObjectType = _spXMLElement->GetName();
	//CF_WARN_IF( sObjectType != _spObject->GetTypeId(), L"GuiMain::InitFromXML: Type of Control '%s' (%s) is different from type in XML Node (%s)", _spObject->GetName().wc_str(), _spObject->GetTypeId().wc_str(), sObjectType.wc_str() );

	GuiObjectCreatorPtr spCreator = GetObjectCreator( _spObject->GetTypeId() );
	CF_ASSERT( spCreator != NULL, L"GuiMain::InitFromXML: Could not init Gui object of type %s, no creator.", _spObject->GetTypeId().wc_str() );

    // Start initializing
    _spObject->StartInit();

	spCreator->InitFromXML( _spObject, _spXMLElement );

    // End initializing
    _spObject->EndInit();
}

void GuiMain::InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const
{
	CF_ASSERT( _spObject->GetTypeId() == _spTemplate->GetTypeId(), L"GuiMain::InitFromTemplate: Type of Control %s (%s) is different from type of template %s (%s)", _spObject->GetName().wc_str(), _spObject->GetTypeId().wc_str(), _spTemplate->GetName().wc_str(), _spTemplate->GetTypeId().wc_str() );

	GuiObjectCreatorPtr spCreator = GetObjectCreator( _spObject->GetTypeId() );
	CF_ASSERT( spCreator != NULL, L"GuiMain::InitFromTemplate: Could not init Gui object of type %s, no creator.", _spObject->GetTypeId().wc_str() );

    // Start initializing
    _spObject->StartInit();

	spCreator->InitFromTemplate( _spObject, _spTemplate );

    // End initializing
    _spObject->EndInit();
}

bool GuiMain::SaveToXML( GuiObjectCPtr _spObject, const CFoundation::String &sXMLFile ) const
{
	CFoundation::XMLParser &clParser = CFoundation::XMLParser::GetInstance();
	CFoundation::XMLDocumentPtr spDoc = clParser.CreateDocument( "window" );
    CFoundation::XMLElementPtr spXMLElement = spDoc->GetRootElement();
	spXMLElement->SetAttribute( "xmlns", "http://www.oeing.eu/cframework/schemas/ctrls" );
	spXMLElement->SetAttribute( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" );
	spXMLElement->SetAttribute( "xsi:schemaLocation", "http://www.oeing.eu/cframework/schemas/ctrls ./common/ctrls/window.xsd" );

	if ( !SaveToXML( _spObject, spDoc, spXMLElement ) )
    {
        return false;
    }

	//spDoc->Refine();

	return clParser.WriteFile( spDoc, sXMLFile );
}

bool GuiMain::SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const
{
	CFoundation::String sObjectType = _spObject->GetTypeId();

	GuiObjectCreatorPtr spCreator = GetObjectCreator( sObjectType );

	CF_ASSERT( spCreator != NULL, L"Could not save Gui object of type %s, no creator.", sObjectType.wc_str() );

	return spCreator->SaveToXML( _spObject, _spDoc, _spXMLElement );
}

void GuiMain::Delete( GuiObjectPtr _spObject ) const
{
    // Suspend layout
    _spObject->SuspendLayout();

    // Clear object
    _spObject->Clear();

    // Remove from parent
    _spObject->RemoveFromParent();

	// Inform message system
	// MessageSystem::GetInstance().DeletedObject( clObject );

	// Send a destroy message, perhaps the Object is in use at the moment
	/*DestroyMsg clMsg( clObject );
	clMsg.SetReceiver( this );
	gMessageSystem.Send( clMsg );*/

    // Resume layout
    _spObject->ResumeLayout( false );
}
        
Layer& GuiMain::GetLayer( LayerType _eType ) const
{
    CF_ERROR_IF( _eType < 0 || _eType >= LAYER_END, "GuiMain::GetLayer: No valid layer." );
    return *m_aspLayers[ _eType ];
}
