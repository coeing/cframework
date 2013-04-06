#include "cgui/GUI/GuiObjectView.h"

#include "cfoundation/Debug/Assert.h"

#include "cgui/Renderer/Elements/RenderElement.h"
#include "cgui/Renderer/Elements/RenderElementTexturedRect.h"

using namespace CGui;

GuiObjectView::GuiObjectView()
:	m_vSize( 0, 0 ),
	m_f16Alpha( 1.0f ),
    m_bBgVisible( true ),
	m_clBgColor( 1.0f, 1.0f, 1.0f, 1.0f ),
	m_bRepeatX( false ),
	m_bRepeatY( false ),
	m_bScaleX( true ),
	m_bScaleY( true ),
	m_eOutline( OUTLINE_RECTANGLE ),
    m_f16Angle( 0.0f )
{
	m_rectTexture.SetPosition( 0.0f, 0.0f );
	m_rectTexture.SetSize( 1.0f, 1.0f );
}

GuiObjectView::~GuiObjectView()
{
}

Unsigned32 GuiObjectView::GetWidth() const
{
	return m_vSize.GetX();
}

Unsigned32 GuiObjectView::GetHeight() const
{
	return m_vSize.GetY();
}

void GuiObjectView::SetBgVisible( bool _bVisible )
{
	m_bBgVisible = _bVisible;
}

bool GuiObjectView::IsBgVisible() const
{
	return m_bBgVisible;
}

const CFoundation::Color& GuiObjectView::GetBgColor() const
{
	return m_clBgColor;
}

const Texture& GuiObjectView::GetTexture() const
{
	return m_clTexture;
}

const Texture& GuiObjectView::GetMask() const
{
	return m_clMask;
}

bool GuiObjectView::GetRepeatX() const
{
	return m_bRepeatX;
}
bool GuiObjectView::GetRepeatY() const
{
	return m_bRepeatY;
}

bool GuiObjectView::GetScaleX() const
{
	return m_bScaleX;
}
bool GuiObjectView::GetScaleY() const
{
	return m_bScaleY;
}
       
void GuiObjectView::SetTextureRect( const CFoundation::RectF16& _rectTexture )
{
    m_rectTexture = _rectTexture;
}

const CFoundation::RectF16& GuiObjectView::GetTextureRect() const
{
    return m_rectTexture;
}

Outline GuiObjectView::GetOutline() const
{
	return m_eOutline;
}
        
void GuiObjectView::SetRotation( Float16 _f16Angle )
{
    m_f16Angle = _f16Angle;
}

Float16 GuiObjectView::GetRotation() const
{
    return m_f16Angle;
}

void GuiObjectView::SetSize( const CFoundation::Vector2Du32& _clSize )
{
    m_vSize = _clSize;
	RecalcTextureRect();
}

void GuiObjectView::SetSize( Unsigned32 u32Width, Unsigned32 u32Height )
{
    SetSize( CFoundation::Vector2Du32( u32Width, u32Height ) );
}

const CFoundation::Vector2Du& GuiObjectView::GetSize() const
{
    return m_vSize;
}

void GuiObjectView::SetWidth( Unsigned32 u32Width )
{
	m_vSize.SetX( u32Width );
	RecalcTextureRect();
}

void GuiObjectView::SetHeight( Unsigned32 u32Height )
{
	m_vSize.SetY( u32Height );
	RecalcTextureRect();
}

void GuiObjectView::SetAlpha( Float16 _f16Alpha )
{
    m_f16Alpha = _f16Alpha;
}

Float16 GuiObjectView::GetAlpha() const
{
    return m_f16Alpha;
}

void GuiObjectView::SetBgColor( const CFoundation::Color &clBgColor )
{
	m_clBgColor = clBgColor;
}

void GuiObjectView::SetTexture( const Texture& _clTexture )
{
    if ( m_clTexture != _clTexture )
    {
        m_clTexture = _clTexture;
	    RecalcTextureRect();
    }
}

void GuiObjectView::SetMask( const Texture& _clMask )
{
    if ( m_clMask == _clMask )
    {
        return;
    }
    
    m_clMask = _clMask;
}

void GuiObjectView::SetRepeatX( bool bRepeat )
{
	m_bRepeatX = bRepeat;
	RecalcTextureRect();
}

void GuiObjectView::SetRepeatY( bool bRepeat )
{
	m_bRepeatY = bRepeat;
	RecalcTextureRect();
}

void GuiObjectView::SetScaleX( bool bScale )
{
	m_bScaleX = bScale;
	RecalcTextureRect();
}
void GuiObjectView::SetScaleY( bool bScale )
{
	m_bScaleY = bScale;
	RecalcTextureRect();
}

void GuiObjectView::SetOutline( Outline eOutline )
{
    if( m_eOutline != eOutline )
    {
	    m_eOutline = eOutline;
    }
}

void GuiObjectView::RecalcTextureRect()
{
    if ( !m_clTexture.IsValid() )
    {
        return;
    }

	Float16 f16Width  = static_cast< Float16 >( GetWidth() );
	Float16 f16Height = static_cast< Float16 >( GetHeight() );

	Float16 f16TexS = 1.0f;
	Float16 f16TexT = 1.0f;

	if( !m_bScaleX )
	{
		f16TexS = f16Width / m_clTexture.GetWidth();

		if( !m_bRepeatX )
		{
            f16TexS = std::min( f16TexS, 1.0f );
		}
	}

	if( !m_bScaleY )
	{
		f16TexT = f16Height / m_clTexture.GetHeight();

		if( !m_bRepeatY )
		{
			f16TexT = std::min( f16TexT, 1.0f );
		}
	}

//	m_clTexRect.SetSize( f16TexS, f16TexT );
}

#ifdef MULTIPLE_RENDER_ELEMENTS
void GuiObjectView::Render( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements )
#else
void GuiObjectView::Render( const RenderParams& _Params, std::vector< RenderElement >& _aspRenderElements )
#endif
{
	if ( m_clTexture.IsValid() )
	{
		RenderTexture( _Params, _aspRenderElements );
	}
    else if ( m_bBgVisible )
	{
        RenderBg( _Params, _aspRenderElements );
	}
}

#ifdef MULTIPLE_RENDER_ELEMENTS
void GuiObjectView::RenderBg( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements )
#else
void GuiObjectView::RenderBg( const RenderParams& _Params, std::vector< RenderElement >& _aRenderElements )
#endif
{
	switch ( m_eOutline )
	{
	case OUTLINE_SQUARE:
	case OUTLINE_RECTANGLE:
		{
#ifdef MULTIPLE_RENDER_ELEMENTS
            // Create render element
            RenderElementRectPtr spElement = RenderElementRectPtr( new RenderElementRect() );
            spElement->SetClippingPlane( _Params.GetRenderRect() );
            spElement->SetPosition( _Params.GetPosition() );
            spElement->SetSize( GetSize() );
            spElement->SetAngle( _Params.GetAngle() + m_f16Angle );
            CFoundation::Color clColor = m_clBgColor;
            clColor.SetF16A( _Params.GetAlpha() * GetAlpha() );
            spElement->SetColor( clColor );

            // Add render element
            _aspRenderElements.push_back( spElement );
#else
            // Create render element
            RenderElement newElement( RenderElement::TYPE_RECT );
            newElement.SetClippingPlane( _Params.GetRenderRect() );
            newElement.SetPosition( _Params.GetPosition() );
            newElement.SetSize( GetSize() );
            newElement.SetAngle( _Params.GetAngle() + m_f16Angle );
            CFoundation::Color clColor = m_clBgColor;
            clColor.SetF16A( _Params.GetAlpha() * GetAlpha() );
            newElement.SetColor( clColor );

            // Add render element
            _aRenderElements.push_back( newElement );
#endif
		}
		break;
	case OUTLINE_CIRCLE:
	case OUTLINE_ELLIPSE:
		{
            CF_WARN( "TODO CO: Ellipse/Circle render element" );
	        /*glLoadIdentity();
            glDisable( GL_TEXTURE_2D );
            Drawer::GetInstance().DrawEllipse( _Params.GetPosition(), GetWidth() / 2, GetHeight() / 2, m_clBgColor );*/
		}
		break;
    default:
        break;
	}
}

#ifdef MULTIPLE_RENDER_ELEMENTS
void GuiObjectView::RenderTexture( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements )
#else
void GuiObjectView::RenderTexture( const RenderParams& _Params, std::vector< RenderElement >& _aRenderElements )
#endif
{
    // Compute size
	Unsigned32 f32Width  = GetWidth();
	Unsigned32 f32Height = GetHeight();
	if( !m_bScaleX &&
		!m_bRepeatX &&
		f32Width > m_clTexture.GetWidth() )
	{
		f32Width = m_clTexture.GetWidth();
	}
	if( !m_bScaleY &&
		!m_bRepeatY &&
		f32Height > m_clTexture.GetHeight() )
	{
		f32Height = m_clTexture.GetHeight();
	}

#ifdef MULTIPLE_RENDER_ELEMENTS
    // Create render element
    RenderElementTexturedRectPtr spElement = RenderElementTexturedRectPtr( new RenderElementTexturedRect() );
    spElement->SetClippingPlane( _Params.GetRenderRect() );
    spElement->SetPosition( _Params.GetPosition() );
    spElement->SetSize( CFoundation::Vector2Du( f32Width, f32Height ) );
    spElement->SetAngle( _Params.GetAngle() + m_f16Angle );
    spElement->SetMask( m_clMask );
    spElement->SetBgVisible( m_bBgVisible );
    CFoundation::Color clColor = m_clBgColor;
    clColor.SetF16A( _Params.GetAlpha() * GetAlpha() );
    spElement->SetBgColor( clColor );
    spElement->SetTexture( m_clTexture );
    spElement->SetTextureRect( m_rectTexture );

    _aspRenderElements.push_back( spElement );
#else
    // Create render element
    RenderElement newElement( RenderElement::TYPE_TEXTURED_RECT );
    newElement.SetClippingPlane( _Params.GetRenderRect() );
    newElement.SetPosition( _Params.GetPosition() );
    newElement.SetSize( CFoundation::Vector2Du( f32Width, f32Height ) );
    newElement.SetAngle( _Params.GetAngle() + m_f16Angle );
    newElement.SetMask( m_clMask );
    newElement.SetBgVisible( m_bBgVisible );
    CFoundation::Color clColor = m_clBgColor;
    clColor.SetF16A( _Params.GetAlpha() * GetAlpha() );
    newElement.SetBgColor( clColor );
    newElement.SetTexture( m_clTexture );
    newElement.SetTextureRect( m_rectTexture );

    _aRenderElements.push_back( newElement );
#endif
}
