#include "cgui/Renderer/Elements/RenderElementPrimitives.h"

#include "cgui/Font/Font.h"

using namespace CGui;

#ifdef MULTIPLE_RENDER_ELEMENTS

///////////////////////////////////////////////////////////////////////////////
///
/// RenderElementPrimitives
///
///////////////////////////////////////////////////////////////////////////////

RenderElementPrimitives::RenderElementPrimitives( Type _eType )
:   m_eType( _eType ),
    m_f16Thickness( 1.0f ),
    m_Color( 0.0f, 0.0f, 0.0f, 1.0f )
{
}

RenderElementPrimitives::~RenderElementPrimitives()
{
}

void RenderElementPrimitives::SetType( Type _eType )
{
    m_eType = _eType;
}

RenderElementPrimitives::Type RenderElementPrimitives::GetType() const
{
    return m_eType;
}

void RenderElementPrimitives::ClearPoints()
{
    m_avPoints.clear();
}

void RenderElementPrimitives::AddPoint( const CFoundation::Vector2Df& _vPoint )
{
    m_avPoints.push_back( _vPoint );
}

const std::vector< CFoundation::Vector2Df > RenderElementPrimitives::GetPoints() const
{
    return m_avPoints;
}
        
void RenderElementPrimitives::SetThickness( Float16 _f16Thickness )
{
    m_f16Thickness = _f16Thickness;
}

Float16 RenderElementPrimitives::GetThickness() const
{
    return m_f16Thickness;
}

void RenderElementPrimitives::SetColor( const CFoundation::Color& _Color )
{
    m_Color = _Color;
}

const CFoundation::Color& RenderElementPrimitives::GetColor() const
{
    return m_Color;
}

#endif
