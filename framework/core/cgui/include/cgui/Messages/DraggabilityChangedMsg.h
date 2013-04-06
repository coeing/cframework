#ifndef CGUI_CHANGEDRAGGABILITYMSG_H
#define CGUI_CHANGEDRAGGABILITYMSG_H

#include "cgui/Messages/GuiMessageTemplate.h"
#include "cgui/Messages/MessageID.h"

namespace CGui
{
	//! The message which is send when draggability is changed
	/*!
	 *
	 *  This class specifies the message which is send when
	 *  a GuiObject is made draggable/not draggable
	 *
	 *  \author Christian Oeing
	 *
	 *  \date 2007-08-17
	 *
	 */
	class DraggabilityChangedMsg : public GuiMessageTemplate< DraggabilityChangedMsg >
	{
	public:
		DraggabilityChangedMsg( GuiObjectPtr _spObject, bool bDraggable, bool bOldValue );

		static Unsigned32   GetType() 
        { 
            return MSG_DRAGGABILITY_CHANGED; 
        };

		bool                IsDraggable() const;
		bool                WasDraggable() const;

	private:
		bool	            m_bDraggable;
		bool	            m_bOldValue;
	};
}

#endif
