#include "cgui/Renderer/Elements/RenderElementTexturedRect.h"

#ifdef MULTIPLE_RENDER_ELEMENTS

#include "cgui/Font/Font.h"

using namespace CGui;

///////////////////////////////////////////////////////////////////////////////
///
/// RenderElementTexturedRect
///
///////////////////////////////////////////////////////////////////////////////

RenderElementTexturedRect::RenderElementTexturedRect()
:   m_bBgVisible( false )
{
}

RenderElementTexturedRect::~RenderElementTexturedRect()
{
}
       
void RenderElementTexturedRect::SetMask( const Texture& _Mask )
{
    m_Mask = _Mask;
}

const Texture& RenderElementTexturedRect::GetMask() const
{
    return m_Mask;
}
       
void RenderElementTexturedRect::SetBgVisible( bool _bBgVisible )
{
    m_bBgVisible = _bBgVisible;
}

bool RenderElementTexturedRect::IsBgVisible() const
{
    return m_bBgVisible;
}
        
void RenderElementTexturedRect::SetBgColor( CFoundation::Color _Color )
{
    m_Color = _Color;
}

CFoundation::Color RenderElementTexturedRect::GetBgColor() const
{
    return m_Color;
}
        
void RenderElementTexturedRect::SetTexture( const Texture& _Texture )
{
    m_Texture = _Texture;
}

const Texture& RenderElementTexturedRect::GetTexture() const
{
    return m_Texture;
}
        
void RenderElementTexturedRect::SetTextureRect( const CFoundation::RectF16& _rectTexture )
{
    m_rectTexture = _rectTexture;
}
        
const CFoundation::RectF16& RenderElementTexturedRect::GetTextureRect() const
{
    return m_rectTexture;
}
        
const CFoundation::RectF16 RenderElementTexturedRect::GetRelativeTextureRect() const
{
    if ( !m_Texture.IsValid() )
    {
        return m_rectTexture;
    }

    const CFoundation::Vector2Du& vTextureSize = m_Texture.GetSize();
    return CFoundation::RectF16( m_rectTexture.GetX() / vTextureSize.GetX(), m_rectTexture.GetY() / vTextureSize.GetY(),
                                 m_rectTexture.GetWidth() / vTextureSize.GetX(),  m_rectTexture.GetHeight() / vTextureSize.GetY() );
}

#endif
