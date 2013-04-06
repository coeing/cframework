#ifndef CGUI_MESSAGESYSTEM_H
#define CGUI_MESSAGESYSTEM_H

#include "cfoundation/Utils/Singleton.h"
#include "cfoundation/Math/Vector.h"

#include "csystem/Input/MouseInputEvent.h"

#include "cgui/GUI/GuiObject.h"
#include "cgui/Input/InputEvents.h"

namespace CGui
{
	/// Uses the user input to influence the gui
    /// Executes the user actions on the gui
    class Input : public CFoundation::Singleton< Input >, public CFoundation::Delegate2Invoker< InputEvent, GuiObjectPtr, GuiObjectPtr >
	{
	public:
        DELEGATE2_INVOKER( InputEvent, GuiObjectPtr, GuiObjectPtr );

        //! Destructor
        virtual ~Input();

        //! Per frame update
        void                            Update( Float32 _fTimeDifference );

        //! Sets the input root object
        void                            SetInputRoot( GuiObjectPtr _spInputRoot );

        //! Returns the input root object
        GuiObjectPtr                    GetInputRoot() const;

        /// Sets the focus object
		void                            SetFocusObject( GuiObjectPtr _spFocusObject );

        //! Returns the focus object
		GuiObjectPtr                    GetFocusObject() const;

        /// Returns the pointed object
		GuiObjectPtr                    GetPointedObject() const;

        /// Sets a gui object as a custom cursor
        /// The hot spot is set to the center of the custom cursor
        void                            SetCustomCursor( GuiObjectPtr _spCustomCursor );

        /// Sets a gui object as a custom cursor
        /// _vHotSpot defines the hot spot offset of the custom cursor. (0,0) is top left corner
        void                            SetCustomCursor( GuiObjectPtr _spCustomCursor, const CFoundation::Vector2Di32& _vHotSpot );

        /// Removes the custom cursor
        void                            RemoveCustomCursor();

	private:
		friend class CFoundation::Singleton< Input >;

        //! Constructor
		Input();
        
        /// Handles the mouse input
        void                            HandleMouseInput();

        /// Sets the pointed object
		void                            SetPointedObject( GuiObjectPtr _spPointedObject );

        /// Callback when the left mouse button was double clicked
        void                            OnDoubleClickedLeft();

        /// Callback when the right mouse button was double clicked
        void                            OnDoubleClickedRight();

        /// Delegate when the left mouse button was double clicked
        CSystem::InputEventDelegatePtr  m_spDelegateDoubleClickedLeft;

        /// Delegate when the right mouse button was double clicked
        CSystem::InputEventDelegatePtr  m_spDelegateDoubleClickedRight;

        //! Input root object
        GuiObjectPtr        m_spInputRoot;

        //! Pointed object
		GuiObjectPtr        m_spPointedObject;

        //! Focus object
		GuiObjectPtr        m_spFocusObject;

        /// Pressed object
        GuiObjectPtr        m_spPressedObject;

        /// Custom cursor
        GuiObjectPtr        m_spCustomCursor;

        /// Custom cursor hot spot
        CFoundation::Vector2Di32    m_vCustomCursorHotSpot;

        /// Stores if cursor was visible before window was deactivated
        bool                m_bCursorVisible;

        /// Stores if cursor was clipped before window was deactivated
        bool                m_bCursorClipped;

        /// Stores the cursor clip rect before window was deactivated
        CFoundation::RectI32 m_rectCursorClipRect;
	};
};

#endif
