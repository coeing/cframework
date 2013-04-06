#include "cgui/Renderer/Elements/RenderElementRect.h"

#ifdef MULTIPLE_RENDER_ELEMENTS

#include "cgui/Font/Font.h"

using namespace CGui;

///////////////////////////////////////////////////////////////////////////////
///
/// RenderElementRect
///
///////////////////////////////////////////////////////////////////////////////

RenderElementRect::RenderElementRect()
:   m_f16Angle( 0.0f )
{
}

RenderElementRect::~RenderElementRect()
{
}
void RenderElementRect::SetSize( const CFoundation::Vector2Du& _vSize )
{
    m_vSize = _vSize;
}

const CFoundation::Vector2Du& RenderElementRect::GetSize() const
{
    return m_vSize;
}
        
void RenderElementRect::SetAngle( Float16 _f16Angle )
{
    m_f16Angle = _f16Angle;
}

Float16 RenderElementRect::GetAngle() const
{
    return m_f16Angle;
}
       
void RenderElementRect::SetMask( const Texture& _Mask )
{
    m_Mask = _Mask;
}

const Texture& RenderElementRect::GetMask() const
{
    return m_Mask;
}
        
void RenderElementRect::SetColor( CFoundation::Color _Color )
{
    m_Color = _Color;
}

CFoundation::Color RenderElementRect::GetColor() const
{
    return m_Color;
}

#endif
