#ifndef CMADTV_SAMMY_H
#define CMADTV_SAMMY_H

#include <vector>

#include "cfoundation/Base/Types.h"
#include "cfoundation/DateTime/Date.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    class Serializer;
    class String;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( Sammy );
    enum Hour;

    /// Events
    enum SammyEventPlayer
    {
        EVENT_SAMMY_POINTS_CHANGED
    };
    TYPEDEF_DELEGATE1( SammyEventPlayer, PlayerPtr );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// Sammy
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class Sammy : public CFoundation::Delegate1Invoker< SammyEventPlayer, PlayerPtr >
    {
    public:
        DELEGATE1_INVOKER( SammyEventPlayer, PlayerPtr );

        /// Types
        enum Type
        {
            TYPE_NONE,
            TYPE_NEWS,
            TYPE_CULTURAL,
            //TYPE_SELF_SERIES,
            //TYPE_SELF_SHOW,
            TYPE_END
        };

        /// Creates a sammy of the passed type
        static SammyPtr         Create( Type _eType );

        /// Constructor
        Sammy( Type _eType );

        /// Serialization
        virtual void            Serialize( CFoundation::Serializer& _Serializer );

        /// Returns the type
        Type                    GetType() const;

        /// Sets the game
        void                    SetGame( GameCPtr _spGame );

        /// Returns the game
        GameCPtr                GetGame() const;

        /// Returns the points for the player with the passed index
        Unsigned32              GetPoints( Unsigned32 _u32PlayerIdx ) const;

        /// Returns the maximum points
        Unsigned32              GetMaxPoints() const;

        /// Returns all points summed
        Unsigned32              GetSummedPoints() const;

        /// Called after the news have been started
        virtual void            OnNewsStarted( const CFoundation::Date& _Date, Hour _eHour );

        /// Called after the broadcasts have been started
        virtual void            OnBroadcastsStarted( const CFoundation::Date& _Date, Hour _eHour );

    protected:
        /// Adds the passed amount of points for the player with the passed index
        void                    AddPoints( Unsigned32 _u32PlayerIdx, Unsigned32 _u32Points );

    private:
        /// Type
        Type                        m_eType;

        /// Game
        GameCPtr                    m_spGame;

        /// Player points
        std::vector< Unsigned32 >   m_au32Points;
    };
}

#endif
