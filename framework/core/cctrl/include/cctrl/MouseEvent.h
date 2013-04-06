#ifndef CCTRL_MOUSEEVENT_H
#define CCTRL_MOUSEEVENT_H

#include "cctrl/GuiEvent.h"

namespace CCtrl
{
    //class MouseEvent : public GuiEvent
    //{
    //public:
    //    MouseEvent( CGui::GuiObjectCPtr _spObject );

    //    virtual Unsigned32 GetID() const
    //    {
    //        return MouseEvent::GetId();
    //    };
    //    static Unsigned32 GetId()
    //    {
    //        return 2;
    //    };
    //    MouseEvent* Clone() const
    //    {
    //        return new MouseEvent( *this );
    //    }

    //    void SetEvent( CGui::MouseEvent eEvent );
    //    CGui::MouseEvent GetEvent() const;

    //    void SetTrigger( CGui::MouseButton eTrigger );
    //    CGui::MouseButton GetTrigger() const;

    //    virtual CFoundation::String GetName();

    //    virtual void Save( CFoundation::Serializer &clSerializer ) const;
    //    virtual void Load( const CFoundation::Serializer &clSerializer );

    //protected:
    //    CFoundation::Message* MakeMsg( CGui::GuiObjectPtr _spObject ) const
    //    {
    //        return new CGui::MouseEventMsg( _spObject, m_eEvent, m_eTrigger );
    //    };

    //private:
    //    CGui::MouseEvent m_eEvent;
    //    CGui::MouseButton m_eTrigger;
    //};
}

#endif
