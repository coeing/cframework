#ifndef CGUI_RESIZABILITYCHANGEDMSG_H
#define CGUI_RESIZABILITYCHANGEDMSG_H

#include "cgui/Messages/GuiMessageTemplate.h"
#include "cgui/Messages/MessageID.h"

namespace CGui
{
	//! The message which is send when
	/*!
	 *
	 *  This class specifies the message which is send when
	 *
	 *
	 *  \author Christian Oeing
	 *
	 *  \date 2007-08-17
	 *
	 */
	class ResizabilityChangedMsg : public GuiMessageTemplate< ResizabilityChangedMsg >
	{
	public:
		ResizabilityChangedMsg( GuiObjectPtr _spObject, bool bNewValue, bool bOldValue );

		static Unsigned32   GetType() 
        { 
            return MSG_RESIZABILITY_CHANGED; 
        };

		bool	            IsResizable() const;
		bool	            WasResizable() const;

	private:
		bool	            m_bNewValue;
		bool	            m_bOldValue;
	};
}

#endif
