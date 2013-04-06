#ifndef CGUI_FOCUSMSG_H
#define CGUI_FOCUSMSG_H

#include "cgui/Messages/GuiMessageTemplate.h"
#include "cgui/Messages/MessageID.h"

namespace CGui
{
	class FocusMsg : public GuiMessageTemplate< FocusMsg >
	{
	public:
		enum FocusType
		{
			TYPE_LOST,
			TYPE_GAINED
		};

		FocusMsg( GuiObjectPtr _spObject, FocusType eFocusType, const CFoundation::Vector2Di &clMousePos );
		FocusMsg( const FocusMsg& rhs );

		static Unsigned32           GetType() 
        { 
            return MSG_FOCUS_CHANGED; 
        };

		FocusType                   GetFocusType() const;

		const CFoundation::Vector2Di&	GetMousePosition() const;

	private:
		FocusMsg& operator=( const FocusMsg& rhs );

		FocusType			m_eFocusType;
		CFoundation::Vector2Di	m_clMousePos;
	};
}

#endif
