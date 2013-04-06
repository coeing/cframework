#include "cgui/Renderer/Elements/RenderElement.h"

using namespace CGui;

///////////////////////////////////////////////////////////////////////////////
///
/// RenderElement
///
///////////////////////////////////////////////////////////////////////////////

#ifdef MULTIPLE_RENDER_ELEMENTS
RenderElement::RenderElement()
#else
RenderElement::RenderElement( Type _eType )
:   m_eType( _eType )
#endif
{
}

RenderElement::~RenderElement()
{
}
       
void RenderElement::SetClippingPlane( const CFoundation::RectI32& _rectClipping )
{
    m_rectClipping = _rectClipping;
}

const CFoundation::RectI32& RenderElement::GetClippingPlane() const
{
    return m_rectClipping;
}
        
void RenderElement::SetPosition( const CFoundation::Vector2Di& _vPosition )
{
    m_vPosition = _vPosition;
}

const CFoundation::Vector2Di& RenderElement::GetPosition() const
{
    return m_vPosition;
}

#ifndef MULTIPLE_RENDER_ELEMENTS

void RenderElement::SetText( const CFoundation::String& _strText )
{
    m_strText = _strText;
}

const CFoundation::String& RenderElement::GetText() const
{
    return m_strText;
}
       
void RenderElement::SetFont( FontCPtr _spFont )
{
    m_spFont = _spFont;
}

FontCPtr RenderElement::GetFont() const
{
    return m_spFont;
}

void RenderElement::SetSize( const CFoundation::Vector2Du& _vSize )
{
    m_vSize = _vSize;
}

const CFoundation::Vector2Du& RenderElement::GetSize() const
{
    return m_vSize;
}
        
void RenderElement::SetAngle( Float16 _f16Angle )
{
    m_f16Angle = _f16Angle;
}

Float16 RenderElement::GetAngle() const
{
    return m_f16Angle;
}
       
void RenderElement::SetMask( const Texture& _Mask )
{
    m_Mask = _Mask;
}

const Texture& RenderElement::GetMask() const
{
    return m_Mask;
}
       
void RenderElement::SetBgVisible( bool _bBgVisible )
{
    m_bBgVisible = _bBgVisible;
}

bool RenderElement::IsBgVisible() const
{
    return m_bBgVisible;
}
        
void RenderElement::SetBgColor( CFoundation::Color _Color )
{
    m_BgColor = _Color;
}

CFoundation::Color RenderElement::GetBgColor() const
{
    return m_BgColor;
}
        
void RenderElement::SetTexture( const Texture& _Texture )
{
    m_Texture = _Texture;
}

const Texture& RenderElement::GetTexture() const
{
    return m_Texture;
}
        
void RenderElement::SetTextureRect( const CFoundation::RectF16& _rectTexture )
{
    m_rectTexture = _rectTexture;
}
        
const CFoundation::RectF16& RenderElement::GetTextureRect() const
{
    return m_rectTexture;
}
        
const CFoundation::RectF16 RenderElement::GetRelativeTextureRect() const
{
    if ( !m_Texture.IsValid() )
    {
        return m_rectTexture;
    }

    const CFoundation::Vector2Du& vTextureSize = m_Texture.GetSize();
    return CFoundation::RectF16( m_rectTexture.GetX() / vTextureSize.GetX(), m_rectTexture.GetY() / vTextureSize.GetY(),
                                 m_rectTexture.GetWidth() / vTextureSize.GetX(),  m_rectTexture.GetHeight() / vTextureSize.GetY() );
}

void RenderElement::SetType( Type _eType )
{
    m_eType = _eType;
}

RenderElement::Type RenderElement::GetType() const
{
    return m_eType;
}

void RenderElement::ClearPoints()
{
    m_avPoints.clear();
}

void RenderElement::AddPoint( const CFoundation::Vector2Df& _vPoint )
{
    m_avPoints.push_back( _vPoint );
}

const std::vector< CFoundation::Vector2Df > RenderElement::GetPoints() const
{
    return m_avPoints;
}
        
void RenderElement::SetThickness( Float16 _f16Thickness )
{
    m_f16Thickness = _f16Thickness;
}

Float16 RenderElement::GetThickness() const
{
    return m_f16Thickness;
}

void RenderElement::SetColor( const CFoundation::Color& _Color )
{
    m_FgColor = _Color;
}

const CFoundation::Color& RenderElement::GetColor() const
{
    return m_FgColor;
}

#endif
