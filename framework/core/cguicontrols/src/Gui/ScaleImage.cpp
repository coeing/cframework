#include "cgui/Gui/ScaleImage.h"

#include "cfoundation/Debug/Profiling.h"
#include "cfoundation/Enum/EnumConverter.h"
#include "cfoundation/XML/XMLElement.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Texture/Texture.h"
#include "cgui/Texture/TextureLoader.h"

using namespace CGui;

template <>
void CFoundation::EnumConverter< ScaleImage::Position >::Initialize()
{
    Assign( ScaleImage::TOP_LEFT, "topleft" );
    Assign( ScaleImage::TOP_CENTER, "topcenter" );
    Assign( ScaleImage::TOP_RIGHT, "topright" );
    Assign( ScaleImage::MIDDLE_LEFT, "middleleft" );
    Assign( ScaleImage::MIDDLE_CENTER, "middlecenter" );
    Assign( ScaleImage::MIDDLE_RIGHT, "middleright" );
    Assign( ScaleImage::BOTTOM_LEFT, "bottomleft" );
    Assign( ScaleImage::BOTTOM_CENTER, "bottomcenter" );
    Assign( ScaleImage::BOTTOM_RIGHT, "bottomright" );
}

ScaleImage::ScaleImage( const CFoundation::String &sName )
:   CombinedCtrl( sName )
{
}

ScaleImage::~ScaleImage()
{
}

const CFoundation::String ScaleImage::GetTypeID()
{
	return CFoundation::String( "scaleimage" );
}

const CFoundation::String ScaleImage::GetTypeId() const
{
	return GetTypeID();
}

void ScaleImage::Clear()
{
    // Remove pieces
    for ( Unsigned32 u32Position = 0; u32Position < ScaleImage::POSITION_END; ++u32Position )
	{
		if ( u32Position != ScaleImage::INVALID_POS )
		{
            m_aspImages[ u32Position ].reset();
		}
	}

    CombinedCtrl::Clear();
}
        
void ScaleImage::EndInit()
{
    CombinedCtrl::EndInit();

    UpdatePositions();
    UpdatePieceSizes();
}

void ScaleImage::SetTexture( ScaleImage::Position ePosition, State _eState, const CFoundation::String& sTexture )
{
	ImagePtr pImage = m_aspImages[ ePosition ];
	Texture clTexture;
	if ( TextureLoader::GetInstance().GetTextureFromDisk( sTexture, clTexture ) )
	{
		pImage->SetTexture( _eState, sTexture );
        
        if ( !IsInitializing() )
        {
		    UpdatePositions();
		    UpdatePieceSizes();
	    }
    }
}
        
ImagePtr ScaleImage::GetImage( ScaleImage::Position ePosition )
{
    return m_aspImages[ ePosition ];
}
		
void ScaleImage::SetSize( Unsigned32 u32Width, Unsigned32 u32Height )
{
    SetSize( CFoundation::Vector2Du32( u32Width, u32Height ) );
}

void ScaleImage::SetSize( const CFoundation::Vector2Du& _vSize )
{
    if ( _vSize == GetSize() )
    {
        return;
    }

	CombinedCtrl::SetSize( _vSize );
    
    if ( !IsInitializing() )
    {
	    UpdatePositions();
	    UpdatePieceSizes();
    }
}

void ScaleImage::SetSideSizes( Unsigned32 _u32Left, Unsigned32 _u32Right, Unsigned32 _u32Top, Unsigned32 _u32Bottom )
{
    m_u32Left = _u32Left;
    m_u32Right = _u32Right;
    m_u32Top = _u32Top;
    m_u32Bottom = _u32Bottom;
    
    if ( !IsInitializing() )
    {
	    UpdatePositions();
	    UpdatePieceSizes();
    }
}

Unsigned32 ScaleImage::GetSideSizeLeft() const
{
    return m_u32Left;
}

Unsigned32 ScaleImage::GetSideSizeRight() const
{
    return m_u32Right;
}

Unsigned32 ScaleImage::GetSideSizeTop() const
{
    return m_u32Top;
}

Unsigned32 ScaleImage::GetSideSizeBottom() const
{
    return m_u32Bottom;
}

void ScaleImage::UpdatePieceSizes()
{
	ImagePtr pTopLeft = m_aspImages[ TOP_LEFT ];
	ImagePtr pTopCenter = m_aspImages[ TOP_CENTER ];
	ImagePtr pTopRight = m_aspImages[ TOP_RIGHT ];
	ImagePtr pMiddleLeft = m_aspImages[ MIDDLE_LEFT ];
	ImagePtr pMiddleCenter = m_aspImages[ MIDDLE_CENTER ];
	ImagePtr pMiddleRight = m_aspImages[ MIDDLE_RIGHT ];
	ImagePtr pBottomLeft = m_aspImages[ BOTTOM_LEFT ];
	ImagePtr pBottomCenter = m_aspImages[ BOTTOM_CENTER ];
	ImagePtr pBottomRight = m_aspImages[ BOTTOM_RIGHT ];

    // TODO CO Cleaner solution
    if ( pTopLeft == NULL )
    {
        return;
    }

	Unsigned32 u32Left = m_u32Left;
	Unsigned32 u32Right = m_u32Right;
	Unsigned32 u32Center = 0;
	if( GetWidth() > u32Left + u32Right )
	{
		u32Center = GetWidth() - u32Left - u32Right;
	}

	Unsigned32 u32Top = m_u32Top;
	Unsigned32 u32Bottom = m_u32Bottom;
	Unsigned32 u32Middle = 0;
	if( GetHeight() > u32Top + u32Bottom )
	{
		u32Middle = GetHeight() - u32Top - u32Bottom;
	}

    pTopLeft->SetCustomSize( CFoundation::Vector2Du( u32Left, u32Top ) );
	pTopCenter->SetCustomSize( CFoundation::Vector2Du( u32Center, u32Top ) );
	pTopRight->SetCustomSize( CFoundation::Vector2Du( u32Right, u32Top ) );

	pMiddleLeft->SetCustomSize( CFoundation::Vector2Du( u32Left, u32Middle ) );
	pMiddleCenter->SetCustomSize( CFoundation::Vector2Du( u32Center, u32Middle ) );
	pMiddleRight->SetCustomSize( CFoundation::Vector2Du( u32Right, u32Middle ) );

	pBottomLeft->SetCustomSize( CFoundation::Vector2Du( u32Left, u32Bottom ) );
	pBottomCenter->SetCustomSize( CFoundation::Vector2Du( u32Center, u32Bottom ) );
	pBottomRight->SetCustomSize( CFoundation::Vector2Du( u32Right, u32Bottom ) );
}

void ScaleImage::UpdatePositions()
{
	ImagePtr pTopLeft = m_aspImages[ TOP_LEFT ];
	ImagePtr pTopCenter = m_aspImages[ TOP_CENTER ];
	ImagePtr pTopRight = m_aspImages[ TOP_RIGHT ];
	ImagePtr pMiddleLeft = m_aspImages[ MIDDLE_LEFT ];
	ImagePtr pMiddleCenter = m_aspImages[ MIDDLE_CENTER ];
	ImagePtr pMiddleRight = m_aspImages[ MIDDLE_RIGHT ];
	ImagePtr pBottomLeft = m_aspImages[ BOTTOM_LEFT ];
	ImagePtr pBottomCenter = m_aspImages[ BOTTOM_CENTER ];
	ImagePtr pBottomRight = m_aspImages[ BOTTOM_RIGHT ];

    // TODO CO Cleaner solution
    if ( pTopLeft == NULL )
    {
        return;
    }

    Unsigned32 u32Left = pTopLeft->GetWidth();
    Unsigned32 u32Right = pTopRight->GetWidth();
	Unsigned32 u32Center = 0;
	if( GetWidth() > u32Left + u32Right )
	{
		u32Center = GetWidth() - u32Left - u32Right;
	}

	Unsigned32 u32Top = std::max( pTopLeft->GetHeight(), std::max( pTopCenter->GetHeight(), pTopRight->GetHeight() ) );
	Unsigned32 u32Bottom = std::max( pBottomLeft->GetHeight(), std::max( pBottomCenter->GetHeight(), pBottomRight->GetHeight() ) );
	Unsigned32 u32Middle = 0;
	if( GetHeight() > u32Top + u32Bottom )
	{
		u32Middle = GetHeight() - u32Top - u32Bottom;
	}

	pTopLeft->SetOffset( CFoundation::Vector2Du( 0, 0 ) );
	pTopCenter->SetOffset( CFoundation::Vector2Du( u32Left, 0 ) );
	pTopRight->SetOffset( CFoundation::Vector2Du( u32Left + u32Center, 0 ) );

	pMiddleLeft->SetOffset( CFoundation::Vector2Du( 0, u32Top ) );
	pMiddleCenter->SetOffset( CFoundation::Vector2Du( u32Left, u32Top ) );
	pMiddleRight->SetOffset( CFoundation::Vector2Du( u32Left + u32Center, u32Top ) );

	pBottomLeft->SetOffset( CFoundation::Vector2Du( 0, u32Top + u32Middle ) );
	pBottomCenter->SetOffset( CFoundation::Vector2Du( u32Left, u32Top + u32Middle ) );
	pBottomRight->SetOffset( CFoundation::Vector2Du( u32Left + u32Center, u32Top + u32Middle ) );
}

const CFoundation::String ScaleImageCreator::GetTypeId() const
{
	return ScaleImage::GetTypeID();
}

GuiObjectPtr ScaleImageCreator::CreateObject( const CFoundation::String &sName ) const
{
	ScaleImagePtr spScaleImage( new ScaleImage( sName ) );

    InitDefault( spScaleImage );

	return spScaleImage;
}

void ScaleImageCreator::InitDefault( GuiObjectPtr _spObject ) const
{
	CombinedCtrlCreator::InitDefault( _spObject );

	ScaleImagePtr spScaleImage = static_pointer_cast< ScaleImage >( _spObject );

    // Create pieces
    const CFoundation::EnumConverter< ScaleImage::Position >& clConverter = CFoundation::EnumConverter< ScaleImage::Position >::GetInstance();
    for ( Unsigned32 u32Position = 0; u32Position < ScaleImage::POSITION_END; ++u32Position )
	{
		if ( u32Position != ScaleImage::INVALID_POS )
		{
            ImagePtr spImage = GuiMain::GetInstance().CreateDefault< Image >( clConverter.Convert( static_cast< ScaleImage::Position >( u32Position ) ) );
			spScaleImage->AddPieceLast( spImage );
			spScaleImage->m_aspImages[ u32Position ] = spImage;
		}
	}
}

void ScaleImageCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
    PROFILING( "ScaleImageCreator::InitFromXML" );

	CombinedCtrlCreator::InitFromXML( _spObject, _spXMLElement );

	ScaleImagePtr spScaleImage = static_pointer_cast< ScaleImage >( _spObject );

    CFoundation::String sTmp;

	// Read side size
    Unsigned32 u32Left = spScaleImage->GetSideSizeLeft();
    if ( _spXMLElement->HasAttribute( "left" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "left" );
	    u32Left = sTmp.ToUnsigned32();
    }

    Unsigned32 u32Right = spScaleImage->GetSideSizeRight();
    if ( _spXMLElement->HasAttribute( "right" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "right" );
	    u32Right = sTmp.ToUnsigned32();
    }

    Unsigned32 u32Top = spScaleImage->GetSideSizeTop();
    if ( _spXMLElement->HasAttribute( "top" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "top" );
	    u32Top = sTmp.ToUnsigned32();
    }

    Unsigned32 u32Bottom = spScaleImage->GetSideSizeBottom();
    if ( _spXMLElement->HasAttribute( "bottom" ) )
    {
	    sTmp = _spXMLElement->GetAttribute( "bottom" );
	    u32Bottom = sTmp.ToUnsigned32();
    }

	spScaleImage->SetSideSizes( u32Left, u32Right, u32Top, u32Bottom );

    const CFoundation::EnumConverter< ScaleImage::Position >& clConverter = CFoundation::EnumConverter< ScaleImage::Position >::GetInstance();
	CFoundation::XMLNodePtr pChild = _spXMLElement->GetFirstChild();
	while ( pChild != NULL )
	{
		if ( pChild->IsElement() )
		{
            CFoundation::XMLElementPtr spElement = static_pointer_cast< CFoundation::XMLElement >( pChild );

			CFoundation::String sName = spElement->GetName();
            ScaleImage::Position ePosition = clConverter.Convert( sName );
            ImagePtr spImage = spScaleImage->GetImage( ePosition );
			if ( spImage != NULL )
			{
                GuiMain::GetInstance().InitFromXML( spImage, spElement );
			}
		}
			
        pChild = pChild->GetNextSibling();
	}
}

    /*
ScaleImage& ScaleImageCreator::AssignStyle( GuiObject& _clObject, const CGui::CtrlStyle& _clStyle ) const
{
	ScaleImage& clImage = static_cast< ScaleImage& >( _clObject );

    CFoundation::XMLElementConstPtr pRoot = _clStyle.GetRoot();
    if ( pRoot == NULL )
    {
        return clImage;
    }

    // Init from xml
    return InitFromXML( _clObject, *pRoot );

	GuiObjectCreator::AssignStyle( _clObject, _clStyle );

	// Read side size
	CFoundation::String sTmp = pRoot->GetAttribute( "left" );
	Unsigned32 u32Left = sTmp.ToUnsigned32();

	sTmp = pRoot->GetAttribute( "right" );
	Unsigned32 u32Right = sTmp.ToUnsigned32();

	sTmp = pRoot->GetAttribute( "top" );
	Unsigned32 u32Top = sTmp.ToUnsigned32();

	sTmp = pRoot->GetAttribute( "bottom" );
	Unsigned32 u32Bottom = sTmp.ToUnsigned32();

	spImage->SetSideSizes( u32Left, u32Right, u32Top, u32Bottom );

    const CFoundation::EnumConverter< ScaleImage::Position, ScaleImage::POSITION_END >& clConverter = CFoundation::EnumConverter< ScaleImage::Position, ScaleImage::POSITION_END >::GetInstance();
	CFoundation::XMLNode* pChild = pRoot->GetFirstChild();
	while ( pChild != NULL )
	{
		if ( pChild->IsElement() )
		{
            CFoundation::XMLElement& clElement = *static_cast< CFoundation::XMLElement* >( pChild );

			CFoundation::String sPosition = clElement.GetName();
            ScaleImage::Position ePosition = clConverter.ConvertFromString( sPosition );
            Image* pImage = spImage->GetImage( ePosition );
			if( pImage != NULL )
			{
                pImage->SetStyle( CtrlStyle( clElement ) );
			}
		}
			
        pChild = pChild->GetNextSibling();
	}

	return clImage;
}*/
