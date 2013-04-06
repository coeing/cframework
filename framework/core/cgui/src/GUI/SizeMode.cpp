#include "cgui/Gui/SizeMode.h"

using namespace CGui;

void CFoundation::EnumConverter< SizeMode >::Initialize()
{
    Assign( SIZE_MODE_CUSTOM,   "custom" );
    Assign( SIZE_MODE_CONTENT,  "content" );
    Assign( SIZE_MODE_STRETCH,  "stretch" );
}
