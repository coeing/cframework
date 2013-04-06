#ifndef CCTRL_GUIRECORDPLAYER_H
#define CCTRL_GUIRECORDPLAYER_H

#include "cfoundation/Utils/Singleton.h"

#include "cctrl/GuiRecord.h"

namespace CCtrl
{
	class GuiRecordPlayer : public CFoundation::Singleton<GuiRecordPlayer>
	{
	public:
		void Play( const GuiRecord &clRecord );
	};
}

#endif
