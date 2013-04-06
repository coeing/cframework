#include "cgui/Font/CharMetrics.h"

using namespace CGui;

CharMetrics::CharMetrics()
:	m_clBoundingBox( 0, 0, 0, 0 ),
    m_f32HoriBearingX( 0.0f ),
    m_f32HoriBearingY( 0.0f ),
    m_f32HoriAdvance( 0.0f ),
    m_f32VertBearingX( 0.0f ),
    m_f32VertBearingY( 0.0f ),
    m_f32VertAdvance( 0.0f )
{
}

CharMetrics::CharMetrics( const CFoundation::RectF32 &clBoundingBox )
:	m_clBoundingBox( clBoundingBox ),
    m_f32HoriBearingX( 0.0f ),
    m_f32HoriBearingY( 0.0f ),
    m_f32HoriAdvance( 0.0f ),
    m_f32VertBearingX( 0.0f ),
    m_f32VertBearingY( 0.0f ),
    m_f32VertAdvance( 0.0f )
{
}

void CharMetrics::SetBoundingBox( const CFoundation::RectF32 &clBoundingBox )
{
	m_clBoundingBox = clBoundingBox;
}

const CFoundation::RectF32& CharMetrics::GetBoundingBox() const
{
	return m_clBoundingBox;
}

void CharMetrics::SetHoriBearingX( Float32 _f32HoriBearingX )
{
    m_f32HoriBearingX = _f32HoriBearingX;
}

Float32 CharMetrics::GetHoriBearingX() const
{
    return m_f32HoriBearingX;
}

void CharMetrics::SetHoriBearingY( Float32 _f32HoriBearingY )
{
    m_f32HoriBearingY = _f32HoriBearingY;
}

Float32 CharMetrics::GetHoriBearingY() const
{
    return m_f32HoriBearingY;
}

void CharMetrics::SetHoriAdvance( Float32 _f32HoriAdvance )
{
    m_f32HoriAdvance = _f32HoriAdvance;
}

Float32 CharMetrics::GetHoriAdvance() const
{
    return m_f32HoriAdvance;
}

void CharMetrics::SetVertBearingX( Float32 _f32VertBearingX )
{
    m_f32VertBearingX = _f32VertBearingX;
}

Float32 CharMetrics::GetVertBearingX() const
{
    return m_f32VertBearingX;
}

void CharMetrics::SetVertBearingY( Float32 _f32VertBearingY )
{
    m_f32VertBearingY = _f32VertBearingY;
}

Float32 CharMetrics::GetVertBearingY() const
{
    return m_f32VertBearingY;
}

void CharMetrics::SetVertAdvance( Float32 _f32VertAdvance )
{
    m_f32VertAdvance = _f32VertAdvance;
}

Float32 CharMetrics::GetVertAdvance() const
{
    return m_f32VertAdvance;
}

