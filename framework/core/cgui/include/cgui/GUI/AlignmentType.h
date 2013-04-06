#ifndef CGUI_ALIGNMENT_H
#define CGUI_ALIGNMENT_H

#include "cfoundation/Enum/EnumConverter.h"

namespace CGui
{
	enum AlignmentType
	{
        ALIGNMENT_TYPE_NONE,

		ALIGN_VERTICAL,
		ALIGN_HORIZONTAL,

		ALIGNMENT_TYPE_END
	};

	enum Alignment
	{
		ALIGN_UNDEFINED,

		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT,
		ALIGN_TOP,
		ALIGN_MIDDLE,
		ALIGN_BOTTOM,

		ALIGNMENT_END
	};
}

template <>
void CFoundation::EnumConverter< CGui::Alignment >::Initialize();

#endif
