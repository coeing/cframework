#ifndef CCTRL_FOCUSEVENT_H
#define CCTRL_FOCUSEVENT_H

#include "cgui/Messages/FocusMsg.h"

#include "cctrl/GuiEvent.h"

namespace CCtrl
{
    class FocusEvent : public GuiEvent
    {
    public:
        FocusEvent( CGui::GuiObjectCPtr _spObject );

        virtual Unsigned32 GetID() const
        {
            return FocusEvent::GetId();
        };
        static Unsigned32 GetId()
        {
            return 3;
        };
        FocusEvent* Clone() const
        {
            return new FocusEvent( *this );
        }

        void                        SetFocusType( CGui::FocusMsg::FocusType eFocusType );
        CGui::FocusMsg::FocusType   GetFocusType() const;

        virtual CFoundation::String     GetName();

        virtual void Save( CFoundation::Serializer &clSerializer ) const;
        virtual void Load( const CFoundation::Serializer &clSerializer );

    protected:
        CFoundation::Message* MakeMsg( CGui::GuiObjectPtr _spObject ) const
        {
            return new CGui::FocusMsg( _spObject, m_eFocusType, GetMousePosition() );
        };

    private:
		FocusEvent& operator=( const FocusEvent& rhs );

        CGui::FocusMsg::FocusType m_eFocusType;
    };
}

#endif
