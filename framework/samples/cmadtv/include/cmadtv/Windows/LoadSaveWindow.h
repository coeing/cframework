#ifndef CMADTV_LOAD_SAVE_WINDOW_H
#define CMADTV_LOAD_SAVE_WINDOW_H

#include "cctrl/WindowController.h"

#include "csystem/Input/MouseInputEvent.h"

#include "cmadtv/Windows/Window.h"

namespace CMadTV
{
    enum Room;

    class LoadSaveWindow : public Window
    {
    public:
        enum Mode
        {
            MODE_NONE,
            MODE_LOAD,
            MODE_SAVE,
            MODE_END
        };

        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        LoadSaveWindow();

        /// Destructor
        ~LoadSaveWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Initialization
        void                    Init( Mode _eMode, Room _ePrevRoom );

        /// Processes input (when window is active)
        virtual void            ProcessInput( const CSystem::InputState& _State );

    private:
        /// Sets the mode
        void                    SetMode( Mode _eMode );

        /// Enables/Disables the input of the slots
        void                    EnableSlots( bool _bEnable );

        /// Callback when window should be closed
        void                    OnClose();

        /// Delegates
        CSystem::InputEventDelegate0Ptr m_spDelegatePressedMouseRight;

        /// Mode
        Mode                    m_eMode;

        /// Previous room
        Room                    m_ePrevRoom;

        /// Slots
        std::vector< CGui::EditBoxPtr > m_aspEbSlots;
    };
}

#endif
