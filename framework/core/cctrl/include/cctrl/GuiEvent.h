#ifndef CCTRL_GUIEVENT_H
#define CCTRL_GUIEVENT_H

#include "cfoundation/Events/Message.h"
#include "cfoundation/Serialization/Serializable.h"
#include "cfoundation/DateTime/Time.h"
#include "cfoundation/Math/Vector.h"

#include "cgui/Gui/GuiObject.h"

namespace CCtrl
{
	class GuiEvent : public CFoundation::Serializable
	{
	public:
		explicit GuiEvent( CGui::GuiObjectCPtr _spObject );
		GuiEvent( CGui::GuiObjectCPtr _spObject, const CFoundation::Time &clTimeStamp );
		GuiEvent( CGui::GuiObjectCPtr _spObject, const CFoundation::Time &clTimeStamp, const CFoundation::Vector2Di &clMousePosition );

		virtual Unsigned32 GetID() const = 0;
		virtual GuiEvent* Clone() const = 0;

		void SetReceiver( CGui::GuiObject *pObject );
		CGui::GuiObject* GetReceiver();

		void SetSender( CGui::GuiObject *pObject );
		CGui::GuiObject* GetSender();

		void SetTimeStamp( const CFoundation::Time &clTimeStamp );
		const CFoundation::Time& GetTimeStamp() const;

		void SetMousePosition( const CFoundation::Vector2Di &clMousePosition );
		const CFoundation::Vector2Di& GetMousePosition() const;

		CFoundation::Message* CreateMsg( CGui::GuiObjectPtr _spObject ) const;

		virtual CFoundation::String GetName() { return "GuiEvent"; };

		virtual void Save( CFoundation::Serializer &clSerializer ) const;
		virtual void Load( const CFoundation::Serializer &clSerializer );

	protected:
		virtual CFoundation::Message* MakeMsg( CGui::GuiObjectPtr _spObject ) const = 0;
		virtual void FillMsg( CFoundation::Message *pMsg ) const;

	private:
		GuiEvent& operator=( const GuiEvent& rhs );

	    CGui::GuiObjectCPtr m_spObject;
		CGui::GuiObject*        m_pReceiver;
		CGui::GuiObject*        m_pSender;

		CFoundation::Time		    m_clTimeStamp;
		CFoundation::Vector2Di	    m_clMousePosition;
	};
}

#endif
