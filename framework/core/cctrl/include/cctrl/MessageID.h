#ifndef CCTRL_MESSAGEID_H
#define CCTRL_MESSAGEID_H

#include "cgui/Messages/MessageID.h"

namespace CCtrl
{
    enum EMessageID
    {
        MESSAGE_ID_BEGIN = CGui::MSG_END_GUI_MESSAGES,

        MSG_SCREEN_CLOSED = MESSAGE_ID_BEGIN,

        CMD_CLOSE_SCREEN,

        MESSAGE_ID_END
    };
}

#endif
