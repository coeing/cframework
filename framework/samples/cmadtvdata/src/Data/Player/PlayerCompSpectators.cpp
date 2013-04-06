#include "cmadtv/Data/Player/PlayerCompSpectators.h"

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Database/Database.h"
#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Spectators/Map.h"
#include "cmadtv/Data/Spectators/Transmitter.h"
#include "cmadtv/Data/Player/Player.h"

using namespace CMadTV;

PlayerCompSpectators::PlayerCompSpectators()
:   m_u32CachedNumSpectators( MAX_U32 )
{
}

PlayerCompSpectators::~PlayerCompSpectators()
{
}

template < class Object >
class CompareByID
{
public:
    CompareByID( SharedPtr< Object > _spObject )
    :   m_spObject( _spObject )
    {
    }

    bool    operator()( const SharedPtr< Object >& _rhs ) const
    {
        if ( m_spObject == _rhs )
        {
            return true;
        }
        else if ( m_spObject == NULL )
        {
            return false;
        }
        else
        {
            return m_spObject->GetID() == _rhs->GetID();
        }
    }

private:
    /// Object
    SharedPtr< Object >   m_spObject;
};
       
void PlayerCompSpectators::operator=( const PlayerCompSpectators& _rhs )
{
    // Remove non-existing transmitters
    TransmitterVec aspTransmitters = m_aspTransmitters;
    Unsigned32 u32NumTransmitters = aspTransmitters.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumTransmitters; ++u32Idx )
    {
        TransmitterPtr spTransmitter = aspTransmitters[ u32Idx ];
        if ( std::find_if( _rhs.m_aspTransmitters.begin(), _rhs.m_aspTransmitters.end(), CompareByID< Transmitter >( spTransmitter ) ) == _rhs.m_aspTransmitters.end() )
        {
            RemoveTransmitter( spTransmitter );
        }
    }

    // Add new broadcasts
    u32NumTransmitters = _rhs.m_aspTransmitters.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumTransmitters; ++u32Idx )
    {
        TransmitterPtr spTransmitter = _rhs.m_aspTransmitters[ u32Idx ];
        if ( std::find_if( m_aspTransmitters.begin(), m_aspTransmitters.end(), CompareByID< Transmitter >( spTransmitter ) ) == m_aspTransmitters.end() )
        {
            AddTransmitter( spTransmitter );
        }
    }
}
        
void PlayerCompSpectators::Init( PlayerPtr _spPlayer )
{
    m_spManager = _spPlayer;
}
        
void PlayerCompSpectators::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.SerializeVector< CFoundation::SerializeWrapper_Context >( m_aspTransmitters );

    m_u32CachedNumSpectators = MAX_U32;
}

void PlayerCompSpectators::AddTransmitter( TransmitterPtr _spTransmitter )
{
    if ( _spTransmitter == NULL )
    {
        return;
    }

    m_aspTransmitters.push_back( _spTransmitter );
    m_u32CachedNumSpectators = MAX_U32;

    // Dispatch event
    DispatchEvent( EVENT_TRANSMITTER_ADDED, _spTransmitter );
}
       
bool PlayerCompSpectators::HasTransmitter( TransmitterPtr _spTransmitter ) const
{
    TransmitterVec::const_iterator it = std::find( m_aspTransmitters.begin(), m_aspTransmitters.end(), _spTransmitter );
    return it != m_aspTransmitters.end();
}

void PlayerCompSpectators::RemoveTransmitter( TransmitterPtr _spTransmitter )
{
    TransmitterVec::iterator it = std::find( m_aspTransmitters.begin(), m_aspTransmitters.end(), _spTransmitter );
    if ( it != m_aspTransmitters.end() )
    {
        m_aspTransmitters.erase( it );
        m_u32CachedNumSpectators = MAX_U32;

        // Dispatch event
        DispatchEvent( EVENT_TRANSMITTER_REMOVED, _spTransmitter );
    }
}

const std::vector< TransmitterPtr >& PlayerCompSpectators::GetTransmitters() const
{
    return m_aspTransmitters;
}
        
TransmitterPtr PlayerCompSpectators::GetTransmitter( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < m_aspTransmitters.size() )
    {
        return m_aspTransmitters[ _u32Idx ];
    }
    else
    {
        return TransmitterPtr();
    }
}

Unsigned32 PlayerCompSpectators::GetTransmitterIdx( TransmitterPtr _spTransmitter ) const
{
    TransmitterVec::const_iterator it = std::find( m_aspTransmitters.begin(), m_aspTransmitters.end(), _spTransmitter );
    if ( it != m_aspTransmitters.end() )
    {
        return it - m_aspTransmitters.begin();
    }
    else
    {
        return MAX_U32;
    }
}
        
Unsigned32 PlayerCompSpectators::GetMaxNumSpectators( MapPtr _spMap ) const
{
    if ( m_u32CachedNumSpectators == MAX_U32 )
    {
        m_u32CachedNumSpectators = _spMap->ComputeNumSpectators( m_aspTransmitters );
    }
    return m_u32CachedNumSpectators;
}
 