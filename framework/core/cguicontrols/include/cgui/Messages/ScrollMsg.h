#ifndef CLIB_SCROLLMSG_H
#define CLIB_SCROLLMSG_H

#include "cgui/Messages/GuiMessageTemplate.h"

#include "cgui/Gui/ScrollBar.h"
#include "cgui/Messages/MessageID.h"

namespace CGui
{
    class ScrollMsg : public CGui::GuiMessageTemplate< ScrollMsg >
	{
	public:
		ScrollMsg( GuiObjectPtr _spScrollBar, Float32 f32Position, Unsigned32 u32Offset, ScrollBar::Orientation eOrientation );

		static Unsigned32       GetType() 
        { 
            return MSG_SCROLL_POSITION_CHANGED; 
        };

		void		            SetScrollPosition( Float32 f32ScrollPosition );
		Float32		            GetScrollPosition() const;

		void		            SetScrollOffset( Unsigned32 u32ScrollOffset );
		Unsigned32	            GetScrollOffset() const;

		void		            SetOrientation( ScrollBar::Orientation eOrientation );
		ScrollBar::Orientation	GetOrientation() const;

	private:
		Float32		            m_f32ScrollPosition;
		Unsigned32	            m_u32ScrollOffset;
		ScrollBar::Orientation	m_eOrientation;
	};
}

#endif
