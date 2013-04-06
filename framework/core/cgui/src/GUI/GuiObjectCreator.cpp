#include "cgui/GUI/GuiObjectCreator.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Debug/Profiling.h"
#include "cfoundation/Enum/EnumConverter.h"
#include "cfoundation/XML/XMLDocument.h"
#include "cfoundation/XML/XMLElement.h"

#include "cgui/Gui/CtrlStyle.h"
#include "cgui/Gui/GuiStyle.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/SizeMode.h"

using namespace CGui;

void CFoundation::EnumConverter< Outline >::Initialize()
{
    Assign( OUTLINE_RECTANGLE,  "rectangle" );
    Assign( OUTLINE_SQUARE,     "square" );
    Assign( OUTLINE_ELLIPSE,    "ellipse" );
    Assign( OUTLINE_CIRCLE,     "circle" );
}

void CFoundation::EnumConverter< AttachPoint >::Initialize()
{
    Assign( ATTACH_POINT_TOP_LEFT,      "top left" );
    Assign( ATTACH_POINT_TOP,           "top" );
    Assign( ATTACH_POINT_TOP_RIGHT,     "top right" );
    Assign( ATTACH_POINT_LEFT,          "left" );
    Assign( ATTACH_POINT_CENTER,        "center" );
    Assign( ATTACH_POINT_RIGHT,         "right" );
    Assign( ATTACH_POINT_BOTTOM_LEFT,   "bottom left" );
    Assign( ATTACH_POINT_BOTTOM,        "bottom" );
    Assign( ATTACH_POINT_BOTTOM_RIGHT,  "bottom right" );
}

CFoundation::EnumConverter< AttachObject >& CGui::g_ConverterAttachObject = CFoundation::EnumConverter< AttachObject >::GetInstance();
void CFoundation::EnumConverter< AttachObject >::Initialize()
{
    Assign( ATTACH_OBJECT_PARENT, "parent" );
    Assign( ATTACH_OBJECT_PREV_SIBLING, "prev sibling" );
}
		
const CFoundation::String GuiObjectCreator::GetTypeId() const
{
    return GuiObject::GetTypeID();
}

GuiObjectPtr GuiObjectCreator::CreateObject( const CFoundation::String& _strName ) const
{
    return GuiObjectPtr( new GuiObject( _strName ) );
}

void GuiObjectCreator::InitDefault( GuiObjectPtr _spObject ) const
{
}

void GuiObjectCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
    PROFILING( "GuiObjectCreator::InitFromXML" );

	CFoundation::String sTmp;

	// Get style
	if( _spXMLElement->HasAttribute( "style" ) )
	{
	    sTmp = _spXMLElement->GetAttribute( "style" );
	    const CFoundation::String& sStyleLocation = _spXMLElement->GetAttribute( "style_location" );

        CtrlStyleCPtr spStyle;
	    if ( sStyleLocation == "file" )
	    {
	        spStyle = CtrlStyleCPtr( new CtrlStyle( sTmp ) );
	    }
        else
        {
            if ( sTmp == "default" )
            {
                spStyle = GuiMain::GetInstance().GetDefaultStyle()->GetDefaultStyle( GetTypeId() );
                CF_WARN_IF( spStyle == NULL, L"GuiObjectCreator::InitFromXML: No default style found for type %s.", GetTypeId().wc_str() );
            }
            else
            {
	            spStyle = GuiMain::GetInstance().GetDefaultStyle()->GetStyle( sTmp );
                CF_WARN_IF( spStyle == NULL, L"GuiObjectCreator::InitFromXML: Style %s not found.", sTmp.wc_str() );
            }
        }

        if ( spStyle != NULL )
        {
            _spObject->SetStyle( spStyle );
        }
	}

	// Is visible?
	if ( _spXMLElement->HasAttribute( "visible" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "visible" );
        _spObject->SetVisible( sTmp == "true" );
    }

	// Get size mode
	if ( _spXMLElement->HasAttribute( "size_mode" ) )
	{
	    sTmp = _spXMLElement->GetAttribute( "size_mode" );
        const CFoundation::EnumConverter< SizeMode >& clConverterSizeMode = CFoundation::EnumConverter< SizeMode >::GetInstance();
        SizeMode eSizeMode = clConverterSizeMode.Convert( sTmp );
        _spObject->SetSizeMode( eSizeMode );
	}

	if ( _spXMLElement->HasAttribute( "size_mode_x" ) )
	{
	    sTmp = _spXMLElement->GetAttribute( "size_mode_x" );
        const CFoundation::EnumConverter< SizeMode >& clConverterSizeMode = CFoundation::EnumConverter< SizeMode >::GetInstance();
        SizeMode eSizeMode = clConverterSizeMode.Convert( sTmp );
        _spObject->SetSizeModeX( eSizeMode );
	}

	if ( _spXMLElement->HasAttribute( "size_mode_y" ) )
	{
	    sTmp = _spXMLElement->GetAttribute( "size_mode_y" );
        const CFoundation::EnumConverter< SizeMode >& clConverterSizeMode = CFoundation::EnumConverter< SizeMode >::GetInstance();
        SizeMode eSizeMode = clConverterSizeMode.Convert( sTmp );
        _spObject->SetSizeModeY( eSizeMode );
	}

	// Get width/height
	if ( _spXMLElement->HasAttribute( "width" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "width" );
        _spObject->SetCustomWidth( sTmp.ToUnsigned32() ); 
	}

	if ( _spXMLElement->HasAttribute( "height" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "height" );
        _spObject->SetCustomHeight( sTmp.ToUnsigned32() ); 
	}

	// Get ratio
	if ( _spXMLElement->HasAttribute( "size_ratio" ) )
	{
	    _spObject->SetSizeRatio( _spXMLElement->GetAttribute( "size_ratio" ).ToFloat16() );
    }

	// Get attach object
	if ( _spXMLElement->HasAttribute( "attach_object" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "attach_object" );
        const CFoundation::EnumConverter< AttachObject >& clConverterAttachObject = CFoundation::EnumConverter< AttachObject >::GetInstance();
        _spObject->SetAttachObject( clConverterAttachObject.Convert( sTmp ) );
	}

	// Get local attach point
	if ( _spXMLElement->HasAttribute( "local_attach_point" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "local_attach_point" );
        _spObject->SetLocalAttachPoint( CFoundation::EnumConverter< AttachPoint >::GetInstance().Convert( sTmp ) );
	}

	// Get reference attach point
	if ( _spXMLElement->HasAttribute( "reference_attach_point" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "reference_attach_point" );
        _spObject->SetReferenceAttachPoint( CFoundation::EnumConverter< AttachPoint >::GetInstance().Convert( sTmp ) );
	}

	// Get offset
    CFoundation::Vector2Di vOffset = _spObject->GetOffset();
	if ( _spXMLElement->HasAttribute( "offset_x" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "offset_x" );
        vOffset.SetX( sTmp.ToInteger32() );
	}
	if ( _spXMLElement->HasAttribute( "offset_y" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "offset_y" );
        vOffset.SetY( sTmp.ToInteger32() );
	}
    _spObject->SetOffset( vOffset );

	// get scale
	sTmp = _spXMLElement->GetAttribute( "scaleX" );
	//Float32 f32ScaleX = sTmp.ToFloat();
	sTmp = _spXMLElement->GetAttribute( "scaleY" );
	//Float32 f32ScaleY = sTmp.ToFloat();
	sTmp = _spXMLElement->GetAttribute( "scaleZ" );
	//Float32 f32ScaleZ = sTmp.ToFloat();

	//_spObject->SetScale( f32ScaleX, f32ScaleY, f32ScaleZ );

	if ( _spXMLElement->HasAttribute( "angle" ) )
	{
        _spObject->SetAngle( _spXMLElement->GetAttributeAsFloat16( "angle" ) );
	}

	// get rotation
	sTmp = _spXMLElement->GetAttribute( "rotX" );
	//Integer32 i32RotX = sTmp.ToInteger();
	sTmp = _spXMLElement->GetAttribute( "rotY" );
	//Integer32 i32RotY = sTmp.ToInteger();
	sTmp = _spXMLElement->GetAttribute( "rotZ" );
	//Integer32 i32RotZ = sTmp.ToInteger();

	//_spObject->SetRotation( i32RotX, i32RotY, i32RotZ );

	// Is background visible?
	if ( _spXMLElement->HasAttribute( "bg_visible" ) )
	{
        bool bBgVisible = _spXMLElement->GetAttributeAsBool( "bg_visible" );
        _spObject->SetBgVisible( bBgVisible );
    }

    // Get base background color
    if ( _spXMLElement->HasAttribute( "bg_color" ) )
    {
        sTmp = _spXMLElement->GetAttribute( "bg_color" );
        _spObject->SetBgColor( CFoundation::Color( sTmp ) );
    }

    // get transparency
	if ( _spXMLElement->HasAttribute( "transparency" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "transparency" );
        Float16 f16Transparency = sTmp.ToFloat16();
        _spObject->SetAlpha( 1.0f - f16Transparency );
	}

	// get outline
	if ( _spXMLElement->HasAttribute( "outline" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "outline" );
        _spObject->SetOutline( CFoundation::EnumConverter< Outline >::GetInstance().Convert( sTmp ) );
	}

	// get draggable
	if ( _spXMLElement->HasAttribute( "draggable" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "draggable" );
        bool bDraggable = sTmp == "true";
        _spObject->SetDraggable( bDraggable );
	}

	// get resizable
	if ( _spXMLElement->HasAttribute( "resizable" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "resizable" );
        bool bResizable = sTmp == "true";
        _spObject->SetResizable( bResizable );
	}

	// get tooltip
	if ( _spXMLElement->HasAttribute( "tooltip" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "tooltip" );
        _spObject->SetToolTip( sTmp );
	}

	// get tabindex
	if ( _spXMLElement->HasAttribute( "tabindex" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "tabindex" );
        _spObject->SetTabIndex( sTmp.ToUnsigned16() );
	}

    // Read state infos    
    const CFoundation::EnumConverter< State >& clStateConverter = CFoundation::EnumConverter< State >::GetInstance();
    CFoundation::XMLNodePtr pChildNode = _spXMLElement->GetFirstChild();
    while ( pChildNode != NULL )
    {
        if ( pChildNode->IsElement() )
        {
            CFoundation::XMLElementPtr pElement = static_pointer_cast< CFoundation::XMLElement >( pChildNode );

            // Get state
            State eState = clStateConverter.Convert( pElement->GetName() );

            if ( eState != STATE_NONE )
            {
	            // Is background visible?
	            if ( pElement->HasAttribute( "bg_visible" ) )
	            {
                    sTmp = pElement->GetAttribute( "bg_visible" );
                    _spObject->SetBgVisible( eState, sTmp == "true" );
                }

	            // Get background color
	            if ( pElement->HasAttribute( "bg_color" ) )
	            {
                    sTmp = pElement->GetAttribute( "bg_color" );
                    _spObject->SetBgColor( eState, CFoundation::Color( sTmp ) );
	            } 
            }
        }

        pChildNode = pChildNode->GetNextSibling();
    }
}
		
void GuiObjectCreator::InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const
{    
    // Copy size mode
    SizeMode eSizeModeX = _spTemplate->GetSizeModeX();
    SizeMode eSizeModeY = _spTemplate->GetSizeModeY();
    _spObject->SetSizeModeX( eSizeModeX );
    _spObject->SetSizeModeY( eSizeModeY );

	// Copy width/height
    if ( eSizeModeX == SIZE_MODE_CUSTOM ||
         eSizeModeY == SIZE_MODE_CUSTOM )
    {         
        _spObject->SetCustomSize( _spTemplate->GetCustomSize() );
    }

	// Copy local attach point
    _spObject->SetLocalAttachPoint( _spTemplate->GetLocalAttachPoint() );

	// Copy reference attach point
    _spObject->SetReferenceAttachPoint( _spTemplate->GetReferenceAttachPoint() );

	// Copy attach object
    _spObject->SetAttachObject( _spTemplate->GetAttachObject() );

	// Copy offset
    _spObject->SetOffset( _spTemplate->GetOffset() );

	// Copy scale
    //_spObject->SetScale( _spTemplate->GetScale() );

	// Copy rotation
    //_spObject->SetRotation( _spTemplate->GetRotation() );

    // Copy alpha
    _spObject->SetAlpha( _spTemplate->GetAlpha() );

	// Copy outline
    _spObject->SetOutline( _spTemplate->GetOutline() );

	// Copy draggability
    _spObject->SetDraggable( _spTemplate->IsDraggable() );

	// Copy resizability
    _spObject->SetResizableAnchor( _spTemplate->GetResizableAnchor() );

	// Copy tooltip
    _spObject->SetToolTip( _spTemplate->GetToolTip() );

	// Copy tabindex
    _spObject->SetTabIndex( _spTemplate->GetTabIndex() );

	// Copy state infos
    _spObject->m_clBgColors = _spTemplate->m_clBgColors;
    _spObject->m_bBgVisible = _spTemplate->m_bBgVisible;
}

void GuiObjectCreator::AssignStyle( GuiObjectPtr _spObject, const CtrlStyle& _clStyle ) const
{
	CFoundation::XMLElementCPtr spRoot = _clStyle.GetRoot();
    if ( spRoot == NULL )
    {
        return;
    }

    // Init from xml element
    return InitFromXML( _spObject, spRoot );
}

bool GuiObjectCreator::SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const
{
	CFoundation::String sTmp;
	CFoundation::Vector2Di clTmpVec1;
	CFoundation::Vector3Df clTmpVec2;

	// Set name
    if ( _spObject->GetName() != "" )
    {
	    _spXMLElement->SetAttribute( "name", _spObject->GetName() );
    }

	// Set size mode
    if ( _spObject->GetSizeModeX() != CGui::SIZE_MODE_CUSTOM )
    {
        const CFoundation::EnumConverter< SizeMode >& clConverterSizeMode = CFoundation::EnumConverter< SizeMode >::GetInstance();
        _spXMLElement->SetAttribute( "size_mode_x", clConverterSizeMode.Convert( _spObject->GetSizeModeX() ) );
    }
    if ( _spObject->GetSizeModeY() != CGui::SIZE_MODE_CUSTOM )
    {
        const CFoundation::EnumConverter< SizeMode >& clConverterSizeMode = CFoundation::EnumConverter< SizeMode >::GetInstance();
	    _spXMLElement->SetAttribute( "size_mode_y", clConverterSizeMode.Convert( _spObject->GetSizeModeY() ) );
    }

	// Set width
    if ( _spObject->GetWidth() != 0 )
    {
	    _spXMLElement->SetAttribute( "width", CFoundation::String( _spObject->GetWidth() ) );
    }

    // Set height
    if ( _spObject->GetHeight() != 0 )
    {
	    _spXMLElement->SetAttribute( "height", CFoundation::String( _spObject->GetHeight() ) );
    }

	// Set ratio
    if ( _spObject->GetSizeRatio() > 0.0f )
	{
	    _spXMLElement->SetAttribute( "size_ratio", CFoundation::String( _spObject->GetSizeRatio() ) );
    }

	// Set attach object
    if ( _spObject->GetAttachObject() != CGui::ATTACH_OBJECT_PARENT )
	{
        _spXMLElement->SetAttribute( "attach_object", CFoundation::EnumConverter< AttachObject >::GetInstance().Convert( _spObject->GetAttachObject() ) );
	}

	// Set local attach point
    if ( _spObject->GetLocalAttachPoint() != CGui::ATTACH_POINT_TOP_LEFT )
	{
        _spXMLElement->SetAttribute( "local_attach_point", CFoundation::EnumConverter< AttachPoint >::GetInstance().Convert( _spObject->GetLocalAttachPoint() ) );
	}

	// Set reference attach point
	if ( _spObject->GetReferenceAttachPoint() != CGui::ATTACH_POINT_TOP_LEFT )
	{
        _spXMLElement->SetAttribute( "reference_attach_point", CFoundation::EnumConverter< AttachPoint >::GetInstance().Convert( _spObject->GetReferenceAttachPoint() ) );
	}

	// Set offset
    const CFoundation::Vector2Di& vOffset = _spObject->GetOffset();
    if ( vOffset.GetX() != 0 )
    {
	    _spXMLElement->SetAttribute( "offset_x", CFoundation::String( vOffset.GetX() ) );
    }

    if ( vOffset.GetY() != 0 )
    {
	    _spXMLElement->SetAttribute( "offset_y", CFoundation::String( vOffset.GetY() ) );
    }

	// set scale
	//clTmpVec2 = clNode.GetScale();

	//sTmp = String( clTmpVec2.GetX() );
	//_spXMLElement->SetAttribute( "scaleX", sTmp );

	//sTmp = String( clTmpVec2.GetY() );
	//_spXMLElement->SetAttribute( "scaleY", sTmp );

	//sTmp = String( clTmpVec2.GetZ() );
	//_spXMLElement->SetAttribute( "scaleZ", sTmp );

	// set rotation
	//clTmpVec2 = clNode.GetRotation();

	//sTmp = String( clTmpVec2.GetX() );
	//_spXMLElement->SetAttribute( "rotX", sTmp );

	//sTmp = String( clTmpVec2.GetY() );
	//_spXMLElement->SetAttribute( "rotY", sTmp );

	//sTmp = String( clTmpVec2.GetZ() );
	//_spXMLElement->SetAttribute( "rotZ", sTmp );

	// set outline
	Outline eOutline = _spObject->GetOutline();
    if ( eOutline != OUTLINE_RECTANGLE )
    {
	    _spXMLElement->SetAttribute( "outline", CFoundation::EnumConverter< Outline >::GetInstance().Convert( eOutline ) );
    }

	// set draggable
	bool bDraggable = _spObject->IsDraggable();
    if ( bDraggable )
    {
	    _spXMLElement->SetAttribute( "draggable", bDraggable );
    }

	// set resizable
	//bool bResizable = _spObject->IsResizable();
	//_spXMLElement->SetAttribute( "resizable", bResizable );

	// set tooltip
	sTmp = _spObject->GetToolTip();
    if ( sTmp != "" )
    {
	    _spXMLElement->SetAttribute( "tooltip", sTmp );
    }

	// set tabindex
	Unsigned16 u16TabIndex = _spObject->GetTabIndex();
    if ( u16TabIndex != 0 )
    {
	    _spXMLElement->SetAttribute( "tabindex", CFoundation::String( u16TabIndex ) );
    }

    // Set state infos
    const CFoundation::EnumConverter< State >& clStateConverter = CFoundation::EnumConverter< State >::GetInstance();
    for ( State eState = STATE_BEGIN; eState != STATE_END; ++eState )
    {
        const CFoundation::String& strNodeName = clStateConverter.Convert( eState );
        if ( strNodeName.IsEmpty() )
        {
            continue;
        }

        // Find node
        bool bNewNode = false;
        CFoundation::XMLNodePtr spXMLNodeState = _spXMLElement->GetFirstChild( strNodeName );
        CFoundation::XMLElementPtr spXMLElementState;
        if ( spXMLNodeState != NULL &&
            spXMLNodeState->IsElement() )
        {
            spXMLElementState = static_pointer_cast< CFoundation::XMLElement >( spXMLNodeState );
        }
        else
        {
            spXMLElementState = _spDoc->CreateElement( strNodeName );
            spXMLNodeState = spXMLElementState;
            _spXMLElement->InsertChildLast( spXMLElementState );
            bNewNode = true;
        }

        bool bAddState = false;

	    // Set background
	    bool bBgVisible = _spObject->IsBgVisible( eState, false );
        if ( !bBgVisible )
        {
            bAddState = true;
	        spXMLElementState->SetAttribute( "bg_visible", bBgVisible );
        }

        static CFoundation::Color clDefaultBgColor( 1.0f, 1.0f, 1.0f, 1.0f );
        const CFoundation::Color& clBgColor = _spObject->GetBgColor( eState, false );
        if ( clBgColor != clDefaultBgColor )
        {
            bAddState = true;
	        sTmp = clBgColor.GetHexString();
	        spXMLElementState->SetAttribute( "bg_color", sTmp );
        }

        // Remove state node obsolete
        if ( bNewNode &&
            !bAddState )
        {
            _spXMLElement->RemoveChildLast();
        }
    }

    return true;
}
