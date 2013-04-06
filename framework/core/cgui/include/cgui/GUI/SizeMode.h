#ifndef CGUI_SIZE_MODE_H
#define CGUI_SIZE_MODE_H

#include "cfoundation/Enum/EnumConverter.h"

namespace CGui
{
    enum SizeMode
    {
        SIZE_MODE_NONE,

        SIZE_MODE_CUSTOM,               /// Uses the custom size of the object
        SIZE_MODE_CONTENT,              /// Derives the object's size from its content
        SIZE_MODE_STRETCH,              /// Stretches the object's size to parent's border

        SIZE_MODE_END
    };
}

template <>
void CFoundation::EnumConverter< CGui::SizeMode >::Initialize();

#endif
