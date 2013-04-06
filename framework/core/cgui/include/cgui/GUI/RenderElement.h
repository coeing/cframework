#ifndef CGUI_RENDERELEMENT_H
#define CGUI_RENDERELEMENT_H

#include "cgui/Texture/Texture.h"

namespace CGui
{
    struct RenderElement
    {
        CFoundation::Vector2Di  m_vPosition;
        CFoundation::Vector2Du  m_vSize;
        CFoundation::Color      m_clColor;
        Texture                 m_clTexture;
    };
}

#endif
