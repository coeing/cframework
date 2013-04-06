#ifndef COFILES_BORDERTYPE_H
#define COFILES_BORDERTYPE_H

namespace CoFiles
{
	enum BorderType
	{
		BORDER_NONE			= 0,
		BORDER_LEFT			= 1,
		BORDER_RIGHT		= 2,
		BORDER_TOP			= 4,
		EDGE_TOP_LEFT		= 5,
		EDGE_TOP_RIGHT		= 6,
		BORDER_BOTTOM		= 8,
		EDGE_BOTTOM_LEFT	= 9,
		EDGE_BOTTOM_RIGHT	= 10,
		BORDER_ALL			= 15
	};
}

#endif
