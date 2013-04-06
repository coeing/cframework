#ifndef CGUI_CHILDITERATOR_H
#define CGUI_CHILDITERATOR_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Utils/Iterator.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cgui/GUI/GuiObject.h"

namespace CGui
{
    class Panel;

	class ChildIterator : public CFoundation::Iterator< GuiObjectPtr >
	{
	public:
		ChildIterator( PanelPtr _spPanel );
		void operator=( const ChildIterator &rhs );

		GuiObjectPtr        GetFirst();
		GuiObjectPtr        GetNext();
		const GuiObjectPtr  GetFirst() const;
		const GuiObjectPtr  GetNext() const;
		bool                IsDone() const;
		GuiObjectPtr        GetCurrent() const;

	private:
        PanelPtr            m_spPanel;
		mutable Unsigned32	m_u32Idx;
	};
}

#endif
