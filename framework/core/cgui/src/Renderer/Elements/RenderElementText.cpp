#include "cgui/Renderer/Elements/RenderElementText.h"

#ifdef MULTIPLE_RENDER_ELEMENTS

#include "cgui/Font/Font.h"

using namespace CGui;

///////////////////////////////////////////////////////////////////////////////
///
/// RenderElementText
///
///////////////////////////////////////////////////////////////////////////////

RenderElementText::RenderElementText()
{
}

RenderElementText::~RenderElementText()
{
}

void RenderElementText::SetText( const CFoundation::String& _strText )
{
    m_strText = _strText;
}

const CFoundation::String& RenderElementText::GetText() const
{
    return m_strText;
}
       
void RenderElementText::SetFont( FontCPtr _spFont )
{
    m_spFont = _spFont;
}

FontCPtr RenderElementText::GetFont() const
{
    return m_spFont;
}
        
void RenderElementText::SetColor( CFoundation::Color _Color )
{
    m_Color = _Color;
}

CFoundation::Color RenderElementText::GetColor() const
{
    return m_Color;
}

#endif
