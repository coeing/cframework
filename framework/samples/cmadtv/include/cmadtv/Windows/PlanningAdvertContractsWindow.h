#ifndef CMADTV_PLANNING_ADVERT_CONTRACTS_WINDOW_H
#define CMADTV_PLANNING_ADVERT_CONTRACTS_WINDOW_H

#include "cfoundation/Events/DelegateInvoker.h"

#include "csystem/Input/Input.h"

#include "cgui/DragDrop/DragEvent.h"

#include "cmadtv/Windows/Window.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( AdvertContract );
    INCLUDE_SHARED_CLASS( Player );

    class PlanningAdvertContractsWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        PlanningAdvertContractsWindow();

        /// Destructor
        ~PlanningAdvertContractsWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
		virtual void            DeinitWindow();

        /// Processes input (when window is active)
        virtual void            ProcessInput( const CSystem::InputState& _state );

        /// Callback when context changed
        virtual void            OnContextChanged();

    private:
        enum AdvertContractEvent
        {
            ADVERT_CONTRACT_EVENT_DRAG_STARTED,
            ADVERT_CONTRACT_EVENT_DRAG_ENDED
        };
        typedef Delegate0<> AdvertContractEventDelegate;
        typedef SharedPtr< AdvertContractEventDelegate > AdvertContractEventDelegatePtr;

        /// Advert contract panel
        INCLUDE_SHARED_CLASS( AdvertContractPanel );
        typedef std::vector< AdvertContractPanelPtr > AdvertContractPanelVec;

        /// Sets the player
        void                            SetPlayer( PlayerPtr _spPlayer );

        /// Creates a new advert panel
        AdvertContractPanelPtr          CreateAdvertContractPanel();

        /// Removes all advert panels
        void                            RemoveAllAdvertContractPanels();

        /// Callback when advert contract was started to drag
        void                            OnAdvertContractDragStarted();

        /// Callback when advert contract was ended to drag
        void                            OnAdvertContractDragEnded();

        /// Delegate when advert contract was started to drag
        AdvertContractEventDelegatePtr  m_spDelegateAdvertContractDragStarted;

        /// Delegate when advert contract was ended to drag
        AdvertContractEventDelegatePtr  m_spDelegateAdvertContractDragEnded;

        /// Player
        PlayerPtr                       m_spPlayer;

        /// Advert panel template
        CGui::PanelPtr                  m_spPnlTemplateAdvert;

        /// Advert anchor
        CGui::GuiObjectPtr              m_spObjAdvertsAnchor;

        /// Advert panels
        AdvertContractPanelVec          m_aspAdvertContractPanels;
    };
    
    INCLUDE_SHARED_CLASS( AdvertDragController );
    INCLUDE_SHARED_CLASS( PlanningAdvertContractsWindow );

    class PlanningAdvertContractsWindow::AdvertContractPanel : public CFoundation::Delegate0Invoker< AdvertContractEvent >
    {
    public:
        /// Constructor
        AdvertContractPanel();

        /// Destructor
        ~AdvertContractPanel();

        /// Initialization
        void                Init( CGui::PanelPtr _spGuiPanel );

        /// Deinitialization
        void                Deinit();

        /// Sets the context
        void                SetContext( PlayerContextPtr _spContext );

        /// Sets the advert
        void                SetContract( AdvertContractPtr _spContract );

    private:
        /// Callback when advert contract was started to drag
        void                OnDragStarted( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData );

        /// Callback when advert contract was ended to drag
        void                OnDragEnded( CGui::DragControllerPtr _spDragController, CGui::GuiObjectPtr _spGuiObject, CGui::DragDataPtr _spDragData );

        /// Delegate when advert contract was started to drag
        CGui::DragEventDelegatePtr  m_spDelegateDragStarted;

        /// Delegate when advert contract was ended to drag
        CGui::DragEventDelegatePtr  m_spDelegateDragEnded;        

        /// Context
        PlayerContextPtr        m_spContext;

        /// Gui panel
        CGui::PanelPtr          m_spPnlRoot;

        /// Name TextBox
        CGui::TextBoxPtr        m_spTbName;

        /// Advert 
        AdvertContractPtr       m_spContract;

        /// Drag controller
        AdvertDragControllerPtr m_spDragController;
    };
}

#endif
