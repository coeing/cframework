#ifndef CCTRL_GUIRECORD_H
#define CCTRL_GUIRECORD_H

#include <vector>

#include "cfoundation/Serialization/Serializable.h"

#include "cgui/Gui/GuiObject.h"

#include "cctrl/GuiEvent.h"

namespace CCtrl
{
	class GuiRecord : public CFoundation::Serializable
	{
	public:
		GuiRecord( CGui::GuiObjectCPtr _spObject );
		virtual ~GuiRecord();

		void Clear();

		void AddEvent( GuiEvent &clEvent );

		Unsigned32 GetNumEvents() const;
		GuiEvent* GetEvent( Unsigned32 u32Idx );
		const GuiEvent* GetEvent( Unsigned32 u32Idx ) const;

		void RemoveEvent( Unsigned32 u32Idx );

		virtual void Save( CFoundation::Serializer &clSerializer ) const;
		virtual void Load( const CFoundation::Serializer &clSerializer );

	private:
		GuiRecord& operator=( const GuiRecord& rhs );

		CGui::GuiObjectCPtr     m_spObject;

		typedef std::vector<GuiEvent*> EventVec;
		EventVec			    m_vEvents;
	};
}

#endif
