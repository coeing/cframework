#ifndef CGUI_PARENTCHANGEDMSG_H
#define CGUI_PARENTCHANGEDMSG_H

#include "cgui/Messages/GuiMessageTemplate.h"
#include "cgui/Messages/MessageID.h"

namespace CGui
{
	//! The message which is send when the parent of a GuiObject changes
	/*!
	 *
	 *  This class specifies the message which is send when
	 *
	 *
	 *  \author Christian Oeing
	 *
	 *  \date 2007-08-21
	 *
	 */
	class ParentChangedMsg : public GuiMessageTemplate< ParentChangedMsg >
	{
	public:
		ParentChangedMsg( GuiObjectPtr _spObject, GuiObjectPtr _spNewParent, GuiObjectPtr _spOldParent );

		static Unsigned32 GetType() 
        { 
            return MSG_PARENT_CHANGED; 
        };

		void		    SetOldParent( GuiObjectPtr _spOldParent );
		GuiObjectPtr	GetOldParent() const;

		void		    SetNewParent( GuiObjectPtr _spNewParent );
		GuiObjectPtr	GetNewParent() const;

	private:
        //! Old parent object
		GuiObjectPtr    m_spOldParent;

        //! New parent object
		GuiObjectPtr    m_spNewParent;
	};
}

#endif
