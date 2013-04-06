#include "cgui/GUI/RenderParams.h"

using namespace CGui;

RenderParams::RenderParams()
:	m_clRenderRect( 0, 0, 0, 0 ),
    m_f16Angle( 0.0f ),
    m_f16Alpha( 1.0f )
{
}

void RenderParams::SetRenderWindow( RenderWindowPtr _spWindow )
{
	m_spRenderWindow = _spWindow;
}

void RenderParams::SetRenderRect( const CFoundation::RectI32 &rRect )
{
	m_clRenderRect = rRect;
}

RenderWindowPtr RenderParams::GetRenderWindow()
{
	return m_spRenderWindow;
}

RenderWindowCPtr RenderParams::GetRenderWindow() const
{
	return m_spRenderWindow;
}

CFoundation::RectI32& RenderParams::GetRenderRect()
{
	return m_clRenderRect;
}

const CFoundation::RectI32& RenderParams::GetRenderRect() const
{
	return m_clRenderRect;
}

void RenderParams::AddToPosition( Integer32 i32DeltaX, Integer32 i32DeltaY )
{
	AddToPosition( CFoundation::Vector2Di( i32DeltaX, i32DeltaY ) );
}

void RenderParams::AddToPosition( const CFoundation::Vector2Di &rDelta )
{
	m_clPosition += rDelta;
}

const CFoundation::Vector2Di& RenderParams::GetPosition() const
{
	return m_clPosition;
}

const CFoundation::Vector3Di RenderParams::GetRotation() const
{
	return CFoundation::Vector3Di( m_clRotation.GetX(), m_clRotation.GetY(), m_clRotation.GetZ() );
	//return m_clRotation;
}
        
void RenderParams::SetAngle( Float16 _f16Angle )
{
    m_f16Angle = _f16Angle;
}

void RenderParams::AddAngle( Float16 _f16Angle )
{
    m_f16Angle += _f16Angle;
}

Float16 RenderParams::GetAngle() const
{
    return m_f16Angle;
}
        
void RenderParams::SetAlpha( Float16 _f16Alpha )
{
    m_f16Alpha = _f16Alpha;
}
        
void RenderParams::MultiplyAlpha( Float16 _f16Alpha )
{
    m_f16Alpha *= _f16Alpha;
}
        
Float16 RenderParams::GetAlpha() const
{
    return m_f16Alpha;
}
