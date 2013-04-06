#ifndef CCTRL_BUTTONGROUP_H
#define CCTRL_BUTTONGROUP_H

#include <set>

#include "cfoundation/Base/Types.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Events/MessageReceiver.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( Button );
    INCLUDE_SHARED_CLASS( GuiObject );
    TYPEDEF_DELEGATE1( StateEvent, GuiObjectPtr );
}

namespace CCtrl
{
    INCLUDE_SHARED_CLASS( ButtonGroup );

    enum ButtonGroupButtonEvent
    {
        BUTTON_GROUP_EVENT_NONE,
        BUTTON_GROUP_EVENT_SELECTION_CHANGED,
        BUTTON_GROUP_EVENT_END
    };
    TYPEDEF_DELEGATE3( ButtonGroupButtonEvent, ButtonGroupPtr, CGui::ButtonPtr, CGui::ButtonPtr );

    class ButtonGroup : public CFoundation::MessageReceiver, CFoundation::Delegate3Invoker< ButtonGroupButtonEvent, ButtonGroupPtr, CGui::ButtonPtr, CGui::ButtonPtr >
    {
    public:
        DELEGATE3_INVOKER( ButtonGroupButtonEvent, ButtonGroupPtr, CGui::ButtonPtr, CGui::ButtonPtr );

        /// Constructor
        ButtonGroup();

        /// Processes messages
        virtual void    ProcessMessage( const CFoundation::Message& _clMessage );

        /// Adds a button to the button group
        bool            AddButton( CGui::ButtonPtr _spButton );

        /// Removes a button from the button group
        void            RemoveButton( CGui::ButtonPtr _spButton );

        /// Removes all buttons
        void            RemoveAllButtons();

        //! Returns the number of buttons that belong to this group
        Unsigned32      GetNumButtons() const;

        /// Returns the button with the passed index
        CGui::ButtonPtr GetButton( Unsigned32 _u32Idx ) const;

        //! Set the active button (passed button must belong to the button group)
        void            SetActiveButton( CGui::ButtonPtr _spButton );

        //! Set the active button by index
        void            SetActiveButton( Integer32 _i32Idx );

        //! Returns the current active button
        CGui::ButtonPtr GetActiveButton() const;

        /// Returns the index of the current active button
        Integer32       GetActiveButtonIdx() const;

    private:
        typedef std::vector< CGui::ButtonPtr > ButtonVec;
        
        /// Callbacks
        void                    OnButtonActivated( CGui::GuiObjectPtr _spObject );
        void                    OnButtonDeactivated( CGui::GuiObjectPtr _spObject );

        /// Delegates
        CGui::StateEventDelegate1Ptr  m_spDelegateButtonActivated;
        CGui::StateEventDelegate1Ptr  m_spDelegateButtonDeactivated;

        //! Buttons that belong to this button group
        ButtonVec               m_aspButtons;

        //! Index of the current active button
        Integer32               m_i32ActiveButton;
    };

    typedef SharedPtr< ButtonGroup > ButtonGroupPtr;
}

#endif
