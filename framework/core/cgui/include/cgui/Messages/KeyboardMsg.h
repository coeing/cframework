#ifndef CGUI_KEYBOARDMSG_H
#define CGUI_KEYBOARDMSG_H

#include "cgui/Messages/GuiMessageTemplate.h"
#include "cgui/Messages/MessageID.h"

namespace CGui
{
	class KeyboardMsg : public GuiMessageTemplate< KeyboardMsg >
	{
	public:
		KeyboardMsg( GuiObjectPtr _spObject, const wchar_t& _wChar );

		static Unsigned32   GetType() 
        { 
            return MSG_KEYBOARD; 
        };

		const wchar_t&	    GetChar() const;

	private:
		wchar_t			    m_wChar;
	};
}

#endif
