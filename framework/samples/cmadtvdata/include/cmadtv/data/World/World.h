#ifndef CMADTV_WORLD_H
#define CMADTV_WORLD_H

#include "cfoundation/DateTime/DateTime.h"
#include "cfoundation/Events/DelegateInvoker.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Database );
    INCLUDE_SHARED_CLASS( Game );
    class GameSettings;
    INCLUDE_SHARED_CLASS( Player );

    /// The world
    /** Contains the data of the MadTV world
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 18.01.2009
     */
    class World : public CFoundation::SharedPtrBase< World >
    {
    public:
        /// Constructor
        World();

        /// Destructor
        ~World();

        /// Serialization
        virtual void                    Serialize( CFoundation::Serializer& _Serializer );

        /// Per frame update
        void                            Update( Float32 _f32TimeDifference );

        /// Starts a game with the passed settings
        void                            StartGame( const GameSettings& _Settings );

        /// Returns the database
        DatabasePtr                     GetDatabase() const;

        /// Returns the current game
        GamePtr                         GetGame() const;

    private:
        /////////////////////////////////////////////////////////////////
        // Data
        /////////////////////////////////////////////////////////////////

        /// Database
        DatabasePtr             m_spDatabase;

        /// Current game
        GamePtr                 m_spGame;
    };
}

#endif
