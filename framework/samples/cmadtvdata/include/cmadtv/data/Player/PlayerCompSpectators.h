#ifndef CMADTV_PLAYER_COMP_SPECTATORS_H
#define CMADTV_PLAYER_COMP_SPECTATORS_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cmadtv/Data/World/Hour.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Map );
    INCLUDE_SHARED_CLASS( Transmitter );
    INCLUDE_SHARED_CLASS( PlayerCompSpectators );
    INCLUDE_SHARED_CLASS( Player );

    /// Events
    enum PlayerCompSpectatorsEventTransmitter
    {
        EVENT_TRANSMITTER_ADDED,
        EVENT_TRANSMITTER_REMOVED
    };
    TYPEDEF_DELEGATE1( PlayerCompSpectatorsEventSpectators, TransmitterPtr );

    /// PlayerCompSpectators of one broadcast station for one day
    class PlayerCompSpectators : public CFoundation::Delegate1Invoker< PlayerCompSpectatorsEventTransmitter, TransmitterPtr >
    {
    public:
        DELEGATE1_INVOKER( PlayerCompSpectatorsEventTransmitter, TransmitterPtr );

        /// Constructor
        PlayerCompSpectators();

        /// Destructor
        ~PlayerCompSpectators();

        /// Assignment operator
        void                operator=( const PlayerCompSpectators& _rhs );

        /// Initialization
        void                Init( PlayerPtr _spPlayer );

        /// Serialization
        void                Serialize( CFoundation::Serializer& _Serializer );

        /// Adds a transmitter
        void                AddTransmitter( TransmitterPtr _spTransmitter );

        /// Indicates if the player has the passed transmitter
        bool                HasTransmitter( TransmitterPtr _spTransmitter ) const;

        /// Removes a transmitter
        void                RemoveTransmitter( TransmitterPtr _spTransmitter );

        /// Returns all transmitters
        const std::vector< TransmitterPtr >&    GetTransmitters() const;

        /// Returns the transmitter with the passed index
        TransmitterPtr      GetTransmitter( Unsigned32 _u32Idx ) const;

        /// Returns the index of the passed transmitter
        Unsigned32          GetTransmitterIdx( TransmitterPtr _spTransmitter ) const;

        /// Returns the maximum number of spectators
        Unsigned32          GetMaxNumSpectators( MapPtr _spMap ) const;
        
    private:
        typedef std::vector< TransmitterPtr > TransmitterVec;

        /// Player
        PlayerPtr           m_spManager;

        /// Transmitters
        TransmitterVec      m_aspTransmitters;

        /// Cached number of spectators
        mutable Unsigned32  m_u32CachedNumSpectators;
    };
}

#endif
