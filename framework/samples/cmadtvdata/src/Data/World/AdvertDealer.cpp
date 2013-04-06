#include "cmadtv/Data/World/AdvertDealer.h"

#include <algorithm>

#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Advert.h"

using namespace CMadTV;
        
////////////////////////////////////////////////////////////////////////////////////////////
///
/// AdvertDealer
///
////////////////////////////////////////////////////////////////////////////////////////////

void AdvertDealer::operator=( const AdvertDealer& _rhs )
{
    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_SLOTS; ++u32Idx )
    {
        SetAdvert( u32Idx, _rhs.m_aspAdverts[ u32Idx ] );
    }
}

void AdvertDealer::Serialize( CFoundation::Serializer& _Serializer )
{
    _Serializer.SerializeArray< CFoundation::SerializeWrapper_Context >( m_aspAdverts, NUM_SLOTS );
}

void AdvertDealer::SetAdvert( Unsigned32 _u32Slot, AdvertPtr _spAdvert )
{
    if ( _spAdvert == NULL )
    {
        RemoveAdvert( _u32Slot );
        return;
    }

    if ( _u32Slot >= NUM_SLOTS )
    {
        return;
    }

    if ( _spAdvert == m_aspAdverts[ _u32Slot ] )
    {
        return;
    }

    // Remove old advert
    if ( m_aspAdverts[ _u32Slot ] != NULL )
    {
        RemoveAdvert( _u32Slot );
    }

    m_aspAdverts[ _u32Slot ] = _spAdvert;

    // Dispatch event
    DispatchEvent( ADVERT_DEALER_EVENT_ADVERT_ADDED, _spAdvert, _u32Slot );
}
        
bool AdvertDealer::AddAdvert( AdvertPtr _spAdvert )
{
    for ( Unsigned32 u32Idx = 0; u32Idx != NUM_SLOTS; ++u32Idx )
    {
        if ( m_aspAdverts[ u32Idx ] == NULL )
        {
            SetAdvert( u32Idx, _spAdvert );
            return true;
        }
    }
    return false;
}
        
void AdvertDealer::RemoveAdvert( Unsigned32 _u32Slot )
{
    if ( _u32Slot >= NUM_SLOTS )
    {
        return;
    }

    AdvertPtr spAdvert = m_aspAdverts[ _u32Slot ];
    if ( spAdvert == NULL )
    {
        return;
    }

    // Remove advert
    m_aspAdverts[ _u32Slot ].reset();

    // Dispatch event
    DispatchEvent( ADVERT_DEALER_EVENT_ADVERT_REMOVED, spAdvert, _u32Slot );    
}
        
void AdvertDealer::RemoveAdvert( AdvertCPtr _spAdvert )
{
    for ( Unsigned32 u32Idx = 0; u32Idx != NUM_SLOTS; ++u32Idx )
    {
        if ( m_aspAdverts[ u32Idx ] == _spAdvert )
        {
            SetAdvert( u32Idx, AdvertPtr() );
            break;
        }
    }
}
       
void AdvertDealer::RemoveAllAdverts()
{
    for ( Unsigned32 u32Idx = 0; u32Idx != NUM_SLOTS; ++u32Idx )
    {
        m_aspAdverts[ u32Idx ].reset();
    }
}
       
Unsigned32 AdvertDealer::GetNumAdverts() const
{
    Unsigned32 u32NumAdverts = 0;
    for ( Unsigned32 u32Idx = 0; u32Idx < NUM_SLOTS; ++u32Idx )
    {
        if ( m_aspAdverts[ u32Idx ] != NULL )
        {
            ++u32NumAdverts;
        }
    }
    return u32NumAdverts;
}

AdvertPtr AdvertDealer::GetAdvert( Unsigned32 _u32Slot ) const
{
    if ( _u32Slot >= NUM_SLOTS )
    {
        return AdvertPtr();
    }

    return m_aspAdverts[ _u32Slot ];
}

bool AdvertDealer::HasAdvert( AdvertCPtr _spAdvert ) const
{
    return std::find( m_aspAdverts, m_aspAdverts + NUM_SLOTS, _spAdvert ) != ( m_aspAdverts + NUM_SLOTS );
}
               
Unsigned32 AdvertDealer::GetNumFreeSlots() const
{
    return NUM_SLOTS - GetNumAdverts();
}
