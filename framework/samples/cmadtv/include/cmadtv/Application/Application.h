#ifndef CMADTV_APPLICATION_H
#define CMADTV_APPLICATION_H

#include "CFoundation/Utils/Singleton.h"

#include "cctrl/App.h"

#include "cmadtv/Data/Room.h"
#include "cmadtv/Data/Game/Game.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( InfoWindow );
    INCLUDE_SHARED_CLASS( NavigationWindow );
    INCLUDE_SHARED_CLASS( World );

    //! The CMadTV application
    class Application : public CCtrl::App, public CFoundation::Singleton< Application >
    {
    public:
        //! Constructor
        Application();

        /// Destructor
        ~Application();

        //! Sets the active room
        void                                    SetActiveRoom( Room _eRoom );

        //! Returns the active room
        Room                                    GetActiveRoom() const;

        /// Returns the number of available savegames
        Unsigned32                              GetNumSaveGames() const;

        /// Returns the savegame with the passed index
        //SaveGamePtr                             GetSaveGame( Unsigned32 _u32Idx ) const;

        /// Exits the game
        virtual void                            Exit();

    protected:
        /// Initialization
		virtual bool                            Init();

        /// Deinitialization
		virtual void                            Deinit();

        //! Creates the start windows
        virtual void                            CreateStartWindows();

        //! Returns the caption of the render window
		virtual const CFoundation::String       GetWindowCaption() const;

        //! Returns the application dimensions
        virtual CFoundation::Vector2Du          GetDimensions() const;

        //! Returns the default gui style
        virtual CGui::GuiStylePtr               GetDefaultStyle() const;

        /// Per frame update
        virtual void                            Update( Float32 _f32TimeDifference );

    private:
        /// Returns the active player 
        PlayerPtr                               GetActivePlayer() const;

        /// Callbacks
        void                                    OnGameStarted( GamePtr _spGame );
        void                                    OnGameLost( GamePtr _spGame );
        void                                    OnGameWon( GamePtr _spGame );
        void                                    OnGameEnded( GamePtr _spGame, bool _bWon );

        /// Delegates
        WorldEventGameDelegate1Ptr      m_spDelegateGameStarted;
        WorldEventGameDelegate1Ptr      m_spDelegateGameLost;
        WorldEventGameDelegate1Ptr      m_spDelegateGameWon;

        /// World
        WorldPtr                        m_spWorld;

        /// Active player
        Unsigned32                      m_u32ActivePlayer;

        /// Active room
        Room                            m_eRoom;

        /// Active main window
        CCtrl::WindowControllerPtr      m_spMainWindow;

        /// Info window
        SharedPtr< InfoWindow >         m_spInfoWindow;

        /// Navigation window
        SharedPtr< NavigationWindow >   m_spNavWindow;
    };
}

#endif
