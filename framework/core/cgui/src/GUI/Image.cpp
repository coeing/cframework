#include "cgui/GUI/Image.h"

#include "cfoundation/Debug/Makros.h"
#include "cfoundation/Enum/EnumConverter.h"
#include "cfoundation/XML/XMLElement.h"

#include "cgui/Gui/GuiObjectView.h"
#include "cgui/Gui/SizeMode.h"

#include "cgui/Texture/TextureLoader.h"

using namespace CGui;

Image::Image( const CFoundation::String& _strName )
:	GuiObject( _strName )
{
	m_pView = new GuiObjectView();

    // For images the background is by default invisible
    //SetBgVisible( false );
}

Image::~Image()
{
    SAFE_DELETE( m_pView );
}

const CFoundation::String Image::GetTypeID()
{
	return "image";
}

const CFoundation::String Image::GetTypeId() const
{
	return Image::GetTypeID();
}

void Image::SetState( State _eState )
{
    if ( GetState() != _eState )
    {
        GuiObject::SetState( _eState );

        // Set texture
        m_pView->SetTexture( GetTexture( _eState ) );

        // Recompute size
        if ( GetSizeModeX() == SIZE_MODE_CONTENT ||
             GetSizeModeY() == SIZE_MODE_CONTENT )
        {
            ComputeSize();
        }
    }
}

void Image::SetTexture( const CFoundation::String& _sTextureFile )
{
    SetTexture( STATE_NORMAL, _sTextureFile );
}

void Image::SetTexture( const Texture& _clTexture )
{
    for ( State eState = STATE_BEGIN; eState != STATE_END; ++eState )
    {
        SetTexture( eState, _clTexture );
    }
}
        
void Image::SetTextureRect( const CFoundation::RectF16& _rectTexture )
{
    for ( State eState = STATE_BEGIN; eState != STATE_END; ++eState )
    {
        SetTextureRect( eState, _rectTexture );
    }
}

void Image::SetTexture( State _eState, const CFoundation::String& _sTextureFile )
{
    Texture clTexture;
	if ( _sTextureFile != "" )
	{
		TextureLoader::GetInstance().GetTextureFromDisk( _sTextureFile, clTexture );
	}
    SetTexture( _eState, clTexture );
}
        
void Image::SetTexture( State _eState, const Texture& _clTexture )
{
    if ( _clTexture == m_clTextures[ _eState ] )
    {
        return;
    }

    m_clTextures[ _eState ] = _clTexture;

    if ( GetState() == _eState )
    {
        // Set texture
        m_pView->SetTexture( _clTexture );

        // Recompute size
        if ( GetSizeModeX() == SIZE_MODE_CONTENT ||
             GetSizeModeY() == SIZE_MODE_CONTENT )
        {
            ComputeSize();
        }
    }

    // Set texture rect if none is set
    if ( m_mapTextureRects.find( _eState ) == m_mapTextureRects.end() )
    {
        SetTextureRect( CFoundation::RectF16( CFoundation::Vector2Df( 0, 0 ), _clTexture.GetSize() ) );
    }
}

const Texture& Image::GetTexture( State _eState ) const
{
    do
    {
        // Try to find texture
        StateTextureMap::const_iterator it = m_clTextures.find( _eState );
        if ( it != m_clTextures.end() )
        {
            return it->second;
        }

        // Get fallback state
        _eState = GetFallbackState( _eState );

    } while ( _eState != STATE_NONE );

    static Texture clDefaultTexture;
    return clDefaultTexture;
}
        
void Image::SetTextureRect( State _eState, const CFoundation::RectF16& _rectTexture )
{
    m_mapTextureRects[ _eState ] = _rectTexture;

    if ( GetState() == _eState )
    {
        // Set texture rect
        m_pView->SetTextureRect( _rectTexture );
    }
}

const CFoundation::RectF16& Image::GetTextureRect( State _eState ) const
{
    do
    {
        // Try to find texture rect
        StateRectMap::const_iterator it = m_mapTextureRects.find( _eState );
        if ( it != m_mapTextureRects.end() )
        {
            return it->second;
        }

        // Get fallback state
        _eState = GetFallbackState( _eState );

    } while ( _eState != STATE_NONE );

    static CFoundation::RectF16 rectDefaultTextureRect( 0.0f, 0.0f, 1.0f, 1.0f );
    return rectDefaultTextureRect;
}

void Image::SetMask( const CFoundation::String& _strMask )
{
    Texture clMask;
	if ( _strMask != "" )
	{
		TextureLoader::GetInstance().GetTextureFromDisk( _strMask, clMask );
	}
    m_pView->SetMask( clMask );
}

const CFoundation::String Image::GetMask() const
{
	return m_pView->GetMask().GetFileName();
}

void Image::SetRepeatX( bool bRepeat )
{
    m_pView->SetRepeatX( bRepeat );
}
		
bool Image::GetRepeatX() const
{
    return m_pView->GetRepeatX();
}

void Image::SetRepeatY( bool bRepeat )
{
    m_pView->SetRepeatY( bRepeat );
}
		
bool Image::GetRepeatY() const
{
    return m_pView->GetRepeatY();
}

void Image::SetScaleX( bool bScale )
{
    m_pView->SetScaleX( bScale );
}
		
bool Image::GetScaleX() const
{
    return m_pView->GetScaleX();
}

void Image::SetScaleY( bool bScale )
{
    m_pView->SetScaleY( bScale );
}
		
bool Image::GetScaleY() const
{
    return m_pView->GetScaleY();
}
        
void Image::SetRotation( Float16 _fAngle )
{
    m_pView->SetRotation( _fAngle );
}

Float16 Image::GetRotation() const
{
    return m_pView->GetRotation();
}
    
CFoundation::Vector2Du Image::ComputeSize()
{
    CFoundation::Vector2Du32 vNewSize = GuiObject::ComputeSize();

    // Use size if necessary
    SizeMode eSizeModeX = GetSizeModeX();
    SizeMode eSizeModeY = GetSizeModeY();
    if ( eSizeModeX == SIZE_MODE_CONTENT ||
         eSizeModeY == SIZE_MODE_CONTENT )
    {
        // Get current texture
        Texture curTexture = GetTexture( GetState() );

        // The rect must always start at position (0,0)
        // because it should not be moved
        if ( eSizeModeX == SIZE_MODE_CONTENT )
        {
            Integer32 i32Width = static_cast< Integer32 >( curTexture.GetWidth() );
            if ( i32Width > 0 )
            {
                vNewSize.SetX( static_cast< Unsigned32 >( i32Width ) );
            }
        }

        if ( eSizeModeY == SIZE_MODE_CONTENT )
        {
            Integer32 i32Height = static_cast< Integer32 >( curTexture.GetHeight() );
            if ( i32Height > 0 )
            {
                vNewSize.SetY( static_cast< Unsigned32 >( i32Height ) );
            }
        }
    }

    return vNewSize;
}

const CFoundation::String ImageCreator::GetTypeId() const
{
	return Image::GetTypeID();
}

GuiObjectPtr ImageCreator::CreateObject( const CFoundation::String &sName ) const
{
	ImagePtr spImage( new Image( sName ) );
	return spImage;
}

void ImageCreator::InitDefault( GuiObjectPtr _spObject ) const
{
	GuiObjectCreator::InitDefault( _spObject );
}

void ImageCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	GuiObjectCreator::InitFromXML( _spObject, _spXMLElement );

	ImagePtr spImage = static_pointer_cast< Image >( _spObject );

	CFoundation::String sTmp = "";

	// Get texture
    Texture clTexture;
	if ( _spXMLElement->HasAttribute( "texture" ) )
	{
        const CFoundation::String& sTextureName = _spXMLElement->GetAttribute( "texture" );
        TextureLoader::GetInstance().GetTextureFromDisk( sTextureName, clTexture );
        spImage->SetTexture( clTexture );
	}

    // Get texture rectangle
    bool bSetTextureRect = false;
    CFoundation::RectF16 rectTexture( 0, 0, 1, 1 );
    if ( _spXMLElement->HasAttribute( "texture_x" ) )
    {
        rectTexture.SetX( _spXMLElement->GetAttributeAsFloat16( "texture_x" ) );
        bSetTextureRect = true;
    }
    if ( _spXMLElement->HasAttribute( "texture_y" ) )
    {
        rectTexture.SetY( _spXMLElement->GetAttributeAsFloat16( "texture_y" ) );
        bSetTextureRect = true;
    }
    if ( _spXMLElement->HasAttribute( "texture_width" ) )
    {
        rectTexture.SetWidth( _spXMLElement->GetAttributeAsFloat16( "texture_width" ) );
        bSetTextureRect = true;
    }
    if ( _spXMLElement->HasAttribute( "texture_height" ) )
    {
        rectTexture.SetHeight( _spXMLElement->GetAttributeAsFloat16( "texture_height" ) );
        bSetTextureRect = true;
    }

    if ( bSetTextureRect )
    {
        spImage->SetTextureRect( rectTexture );
    }
    else if ( clTexture.IsValid() )
    {
        spImage->SetTextureRect( CFoundation::RectF16( CFoundation::Vector2Df( 0, 0 ), clTexture.GetSize() ) );
    }

	// Get mask
	if ( _spXMLElement->HasAttribute( "mask" ) )
	{
        const CFoundation::String& sMask = _spXMLElement->GetAttribute( "mask" );
        spImage->SetMask( sMask );
	}

	// Get scale/repeat parameters
	if ( _spXMLElement->HasAttribute( "scalex" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "scalex" );
        spImage->SetScaleX( sTmp == "true" );
	}

	if ( _spXMLElement->HasAttribute( "scaley" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "scaley" );
        spImage->SetScaleY( sTmp == "true" );
	}

	if ( _spXMLElement->HasAttribute( "repeatx" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "repeatx" );
        spImage->SetRepeatX( sTmp == "true" );
	}

	if ( _spXMLElement->HasAttribute( "repeaty" ) )
	{
        sTmp = _spXMLElement->GetAttribute( "repeaty" );
        spImage->SetRepeatY( sTmp == "true" );
	}

	if ( _spXMLElement->HasAttribute( "rotation" ) )
	{
        spImage->SetRotation( _spXMLElement->GetAttributeAsFloat16( "rotation" ) );
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
	            // Get texture
                Texture clTexture;
	            if ( pElement->HasAttribute( "texture" ) )
	            {
		            const CFoundation::String& sTextureName = pElement->GetAttribute( "texture" );
		            TextureLoader::GetInstance().GetTextureFromDisk( sTextureName, clTexture );
                    spImage->SetTexture( eState, clTexture );
	            }

                // Get texture rectangle
                bool bSetTextureRect = false;
                CFoundation::RectF16 rectTexture = spImage->GetTextureRect( eState );
                if ( pElement->HasAttribute( "texture_x" ) )
                {
                    rectTexture.SetX( pElement->GetAttributeAsFloat16( "texture_x" ) );
                    bSetTextureRect = true;
                }
                if ( pElement->HasAttribute( "texture_y" ) )
                {
                    rectTexture.SetY( pElement->GetAttributeAsFloat16( "texture_y" ) );
                    bSetTextureRect = true;
                }
                if ( pElement->HasAttribute( "texture_width" ) )
                {
                    rectTexture.SetWidth( pElement->GetAttributeAsFloat16( "texture_width" ) );
                    bSetTextureRect = true;
                }
                if ( pElement->HasAttribute( "texture_height" ) )
                {
                    rectTexture.SetHeight( pElement->GetAttributeAsFloat16( "texture_height" ) );
                    bSetTextureRect = true;
                }

                if ( bSetTextureRect )
                {
                    spImage->SetTextureRect( eState, rectTexture );
                }
                else if ( clTexture.IsValid() )
                {
                    spImage->SetTextureRect( eState, CFoundation::RectF16( CFoundation::Vector2Df( 0, 0 ), clTexture.GetSize() ) );
                }

                // Get rotation
	            if ( pElement->HasAttribute( "rotation" ) )
	            {
                    //spImage->SetRotation( eState, _spXMLElement->GetAttributeAsFloat16( "rotation" ) );
	            }
            }
        }

        pChildNode = pChildNode->GetNextSibling();
    }
}
		
void ImageCreator::InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const
{
	GuiObjectCreator::InitFromTemplate( _spObject, _spTemplate );

	ImagePtr spImage = static_pointer_cast< Image >( _spObject );
	ImageCPtr spImageTemplate = static_pointer_cast< const Image >( _spTemplate );

	// Get scale/repeat parameters
    spImage->SetScaleX( spImageTemplate->GetScaleX() );
    spImage->SetScaleY( spImageTemplate->GetScaleY() );
    spImage->SetRepeatX( spImageTemplate->GetRepeatX() );
    spImage->SetRepeatY( spImageTemplate->GetRepeatY() );

    // Read state infos    
    for ( Image::StateTextureMap::const_iterator it = spImageTemplate->m_clTextures.begin(); it != spImageTemplate->m_clTextures.end(); ++it )
    {
        spImage->SetTexture( it->first, it->second );
    }
    for ( Image::StateRectMap::const_iterator it = spImageTemplate->m_mapTextureRects.begin(); it != spImageTemplate->m_mapTextureRects.end(); ++it )
    {
        spImage->SetTextureRect( it->first, it->second );
    }
    //spImage->SetMask( eState, spImageTemplate->GetMask( eState ) );
}

    /*
Image& ImageCreator::AssignStyle( GuiObject& _clObject, const CtrlStyle& _clStyle ) const
{
	GuiObjectCreator::AssignStyle( _clObject, _clStyle );

	Image& clImage = static_cast< Image& >( _clObject );
    return clImage;

	const CFoundation::XMLElement* pRoot = _clStyle.GetRoot();
    if ( pRoot == NULL )
    {
        return clImage;
    }

    CFoundation::String sTmp = "";

	// Get mask
	if ( pRoot->HasAttribute( "mask" ) )
	{
        const CFoundation::String& sMask = pRoot->GetAttribute( "mask" );
        clImage.SetMask( sMask );
	}

	// Get scale/repeat parameters
	if ( pRoot->HasAttribute( "scalex" ) )
	{
        sTmp = pRoot->GetAttribute( "scalex" );
        clImage.SetScaleX( sTmp == "true" );
	}

	if ( pRoot->HasAttribute( "scaley" ) )
	{
        sTmp = pRoot->GetAttribute( "scaley" );
        clImage.SetScaleY( sTmp == "true" );
	}

	if ( pRoot->HasAttribute( "repeatx" ) )
	{
        sTmp = pRoot->GetAttribute( "repeatx" );
        clImage.SetRepeatX( sTmp == "true" );
	}

	if ( pRoot->HasAttribute( "repeaty" ) )
	{
        sTmp = pRoot->GetAttribute( "repeaty" );
        clImage.SetRepeatY( sTmp == "true" );
	}

    // Read state infos    
    CFoundation::XMLNode* pChildNode = pRoot->GetFirstChild();
    while ( pChildNode != NULL )
    {
        if ( pChildNode->IsElement() )
        {
            CFoundation::XMLElement& clElement = *static_cast< CFoundation::XMLElement* >( pChildNode );

            // Get state
            State eState = StringToState( clElement.GetName() );

            if ( eState != STATE_END )
            {
	            // Get texture
	            if ( clElement.HasAttribute( "texture" ) )
	            {
                    const CFoundation::String& sTextureName = clElement.GetAttribute( "texture" );
                    clImage.SetTexture( eState, sTextureName );
	            }
            }
        }

        pChildNode = pChildNode->GetNextSibling();
    }

	return clImage;
}
*/
bool ImageCreator::SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const
{
	return GuiObjectCreator::SaveToXML( _spObject, _spDoc, _spXMLElement );

//	const Image &clImage = static_cast<const Image&>( clObject );
//
//	// set texture
////	_spXMLElement->SetAttribute( "texture", clImage.GetTexture() );
//	_spXMLElement->SetAttribute( "mask", clImage.GetMask() );
}
