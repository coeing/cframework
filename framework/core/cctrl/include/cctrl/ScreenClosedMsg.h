#ifndef CCTRL_CLOSESCREENMSG_H
#define CCTRL_CLOSESCREENMSG_H

#include "cfoundation/Events/Message.h"

#include "cctrl/MessageID.h"

namespace CCtrl
{
    INCLUDE_SHARED_CLASS( WindowController );

    class ScreenClosedMsg : public CFoundation::MessageTemplate< ScreenClosedMsg >
	{
	public:
		ScreenClosedMsg( WindowControllerPtr _spScreen );
		const ScreenClosedMsg& operator=( const ScreenClosedMsg& rhs );

		static Unsigned32       GetType() 
        { 
            return MSG_SCREEN_CLOSED; 
        };

		WindowControllerPtr     GetScreen() const;

	private:
        WindowControllerPtr     m_spScreen;
	};
}

#endif
