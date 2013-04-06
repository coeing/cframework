#ifndef CGUI_BORDERFACTORY_H
#define CGUI_BORDERFACTORY_H

#include "cfoundation/Utils/Singleton.h"

#include "cgui/GUI/BorderType.h"
#include "cgui/GUI/Border.h"

namespace CGui
{
	class BorderFactory : public CFoundation::Singleton<BorderFactory>
	{
	public:
		virtual Border& CreateBorder( const BorderType& eType ) const;
	};
}

extern CGui::BorderFactory& gBorderFactory;

#endif
