#ifndef CMADTV_PLAYER_H
#define CMADTV_PLAYER_H

#include "CFoundation/Text/String.h"
#include "cfoundation/Utils/Color.h"

#include "cmadtv/Data/Player/AdvertBag.h"
#include "cmadtv/Data/Player/AdvertContracts.h"
#include "cmadtv/Data/Player/BlockStat.h"
#include "cmadtv/Data/Player/BroadcastBag.h"
#include "cmadtv/Data/Player/BroadcastRack.h"
#include "cmadtv/Data/Player/Budget.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( PlayerCompBetty );
    INCLUDE_SHARED_CLASS( PlayerCompNews );
    INCLUDE_SHARED_CLASS( PlayerCompSpectators );
    INCLUDE_SHARED_CLASS( PlayerCompStats );
    INCLUDE_SHARED_CLASS( Scheduling );

    /// Represents a player
    /**
     *  \author Christian Oeing (oeing@gmx.at)
     *  \date 18.01.2009
     */
    class Player : public CFoundation::SharedPtrBase< Player >
    {
    public:
        /// Constructor
        Player();

        /// Destructor
        ~Player();

        /// Assignment operator
        void                    operator=( const Player& _rhs );

        /// Initialization
        void                    Init( GamePtr _spGame );

        /// Serialization
        void                    Serialize( CFoundation::Serializer& _Serializer );

        /// Returns the game this player belongs to
        GamePtr                 GetGame() const;

        /// Sets the name of the player
        void                    SetName( const CFoundation::String& _strName );

        /// Returns the name of the player
        const CFoundation::String&  GetName() const;

        /// Sets the player color
        void                        SetColor( const CFoundation::Color& _colColor );

        /// Returns the player color
        const CFoundation::Color&   GetColor() const;

        /// Returns the player color in a lighter shade
        CFoundation::Color          GetColorLight() const;

        /// Returns the image
        Unsigned8               GetImage() const;

        /// Sets the image
        void                    SetImage( Unsigned8 _u8Image );

        /// Adds the passed amount of image
        /// Takes care that image is inside borders (0-100)
        void                    AddImage( Integer8 _i8Image );

        /// Returns the player's broadcast rack
        BroadcastRack&          GetBroadcastRack();

        /// Returns the player's broadcast bag
        BroadcastBag&           GetBroadcastBag();

        /// Returns the player's accepted adverts
        AdvertContracts&        GetAdvertContracts();

        /// Returns the player's advert bag
        AdvertBag&              GetAdvertBag();

        /// Returns the scheduling of the player
        SchedulingPtr           GetScheduling();

        /// Returns the budget
        Budget&                 GetBudget();

        /// Returns the news component
        PlayerCompNewsPtr       GetCompNews() const;

        /// Returns the spectators component
        PlayerCompSpectatorsPtr GetCompSpectators() const;

        /// Returns the stats component
        PlayerCompStatsPtr      GetCompStats() const;

        /// Returns the betty component
        PlayerCompBettyPtr      GetCompBetty() const;

    private:
        /// Game this player belongs to
        GamePtr                 m_spGame;

        /// Name
        CFoundation::String     m_strName;

        /// Color
        CFoundation::Color      m_colColor;

        /// Image
        Unsigned8               m_u8Image;

        /// Broadcast rack
        BroadcastRack           m_clBroadcastRack;

        /// Broadcast bag
        BroadcastBag            m_clBroadcastBag;

        /// Advert contracts
        AdvertContracts         m_clAdvertContracts;

        /// Advert bag
        AdvertBag               m_clAdvertBag;

        /// Scheduling
        SchedulingPtr           m_spScheduling;

        /// Cash
        Budget                  m_clBudget;

        /// Components
        PlayerCompNewsPtr       m_spNews;
        PlayerCompSpectatorsPtr m_spSpectators;
        PlayerCompStatsPtr      m_spStats;
        PlayerCompBettyPtr      m_spBetty;
    };
}

#endif
