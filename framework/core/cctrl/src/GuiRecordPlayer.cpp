#include "cctrl/GuiRecordPlayer.h"

using namespace CCtrl;

void GuiRecordPlayer::Play( const GuiRecord &clRecord )
{
	for( Unsigned32 u32Idx = 0; u32Idx < clRecord.GetNumEvents(); ++u32Idx )
	{
		const GuiEvent *pEvent = clRecord.GetEvent( u32Idx );
		//CFoundation::Message *pMsg = pEvent->CreateMsg();
		//pMsg->SendToReceiver();
	}
}
