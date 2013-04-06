#include "cgui/GUI/AlignmentType.h"

using namespace CGui;

template <>
void CFoundation::EnumConverter< Alignment >::Initialize()
{
    Assign( ALIGN_LEFT, "left" );
    Assign( ALIGN_MIDDLE, "middle" );
    Assign( ALIGN_RIGHT, "right" );
    Assign( ALIGN_TOP, "top" );
    Assign( ALIGN_CENTER, "center" );
    Assign( ALIGN_BOTTOM, "bottom" );
}
