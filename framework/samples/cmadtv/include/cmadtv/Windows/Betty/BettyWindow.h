#ifndef CMADTV_BETTY_WINDOW_H
#define CMADTV_BETTY_WINDOW_H

#include "cgui/DragDrop/DragController.h"

#include "cmadtv/Windows/Window.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( PlanningArchivWindow );
    INCLUDE_SHARED_CLASS( Present );
    INCLUDE_SHARED_CLASS( PresentDropController );
    INCLUDE_SHARED_CLASS( PresentObject );
    typedef std::vector< PresentObjectPtr > PresentObjectVec;
    INCLUDE_SHARED_CLASS( Sammy );
    INCLUDE_SHARED_CLASS( SammyTooltipController );
    
    enum PlayerCompBettyEventPresent;
    TYPEDEF_DELEGATE1( PlayerCompBettyEventPresent, PresentPtr );
    enum PlayerCompBettyEventU8U8;
    TYPEDEF_DELEGATE2( PlayerCompBettyEventU8U8, Unsigned8, Unsigned8 );
    enum SammyEventPlayer;
    TYPEDEF_DELEGATE1( SammyEventPlayer, PlayerPtr );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BettyWindow
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BettyWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        BettyWindow();

        /// Destructor
        ~BettyWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
		virtual void            DeinitWindow();

        /// Callback when context changed
        virtual void            OnContextChanged();

    private:
        static const Unsigned32 MAX_PLAYERS = 3;

        /// Sets the player
        void                    SetPlayer( PlayerPtr _spPlayer );

        /// Sets the game   
        void                    SetGame( GamePtr _spGame );

        /// Returns the game
        GamePtr                 GetGame() const;

        /// Sets the sammy
        void                        SetSammy( SammyPtr _spSammy );
        
        /// Updates the sammy controls
        void                        UpdateSammy();

        /// Sets the bag present
        void                        SetBagPresent( PresentPtr _spPresent );
        
        /// Updates the player pictures
        void                        UpdatePictures();

        /// Callbacks
        void                        OnBettyPresentChanged( PresentPtr _spPresent );
        void                        OnBettyLoveChanged();
        void                        OnSammyPointsChanged();

        /// Delegates
        PlayerCompBettyEventPresentDelegate1Ptr m_spDelegateBettyPresentChanged;
        PlayerCompBettyEventU8U8Delegate0Ptr    m_spDelegateBettyLoveChanged;
        SammyEventPlayerDelegate0Ptr            m_spDelegateSammyPointsChanged;
        
        /// Game
        GamePtr                     m_spGame;

        /// Sammy  
        SammyPtr                    m_spSammy;

        /// Player
        PlayerPtr                   m_spPlayer;

        /// Bag
        CGui::GuiObjectPtr          m_spObjBag;

        /// Present object
        PresentObjectPtr            m_spBagPresentObject;

        /// Betty drop controller
        PresentDropControllerPtr    m_spBettyDropController;

        /// Pictures
        std::vector< CGui::PanelPtr >   m_aspPictures;

        /// Initial y picture offset
        Integer32                       m_i32InitialPictureOffset;

        /// Sammy
        CGui::PanelPtr                      m_spPnlSammy;

        /// Sammy point bars
        std::vector< CGui::ProgressBarPtr > m_aspPbSammyPoints;

        /// Sammy tooltip controller
        SammyTooltipControllerPtr           m_spSammyTooltipController;
    };
}

#endif
