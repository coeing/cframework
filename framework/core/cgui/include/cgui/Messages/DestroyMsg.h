#ifndef CGUI_DESTROYMSG_H
#define CGUI_DESTROYMSG_H

#include "cfoundation/Events/Message.h"

#include "cgui/GUI/GuiObject.h"
#include "cgui/Messages/MessageID.h"

namespace CGui
{
	class DestroyMsg : public CFoundation::MessageTemplate< DestroyMsg >
	{
	public:
		DestroyMsg( GuiObject& _clObject );
        DestroyMsg( const DestroyMsg& _rhs );

		static Unsigned32 GetType() 
        { 
            return MSG_DESTROY; 
        };

		GuiObject&      GetObject() const;

	private:
        DestroyMsg& operator=( const DestroyMsg& );

		GuiObject&      m_clObject;
	};
}

#endif
