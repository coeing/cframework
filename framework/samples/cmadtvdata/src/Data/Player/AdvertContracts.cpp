#include "cmadtv/Data/Player/AdvertContracts.h"

#include <algorithm>

#include "cfoundation/Serialization/Serializer.h"

#include "cmadtv/Data/Advert.h"
#include "cmadtv/Data/Player/AdvertContract.h"

using namespace CMadTV;

AdvertContracts::AdvertContracts()
{
}
        
void AdvertContracts::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32NumContracts = m_aspContracts.size();
    _Serializer.Serialize( u32NumContracts );
    if ( _Serializer.IsLoading() )
    {
        m_aspContracts.resize( u32NumContracts );
    }
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumContracts; ++u32Idx )
    {
        if ( _Serializer.IsLoading() )
        {
            m_aspContracts[ u32Idx ] = AdvertContractPtr( new AdvertContract() );
        }

        AdvertContractPtr spContract = m_aspContracts[ u32Idx ];
        spContract->Serialize( _Serializer );
    }
}

void AdvertContracts::StartContract( AdvertPtr _spAdvert, const CFoundation::Date& _clDate )
{
    // Create new contract
    AdvertContractPtr spContract( new AdvertContract( _spAdvert, _clDate ) );

    // Add to contracts
    m_aspContracts.push_back( spContract );
}

void AdvertContracts::RemoveContract( AdvertContractPtr _spContract )
{
    AdvertContractVec::iterator it = std::find( m_aspContracts.begin(), m_aspContracts.end(), _spContract );
    if ( it != m_aspContracts.end() )
    {
        m_aspContracts.erase( it );
    }
}

Unsigned32 AdvertContracts::GetNumContracts() const
{
    return m_aspContracts.size();
}

AdvertContractPtr AdvertContracts::GetContract( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < GetNumContracts() )
    {
        return m_aspContracts[ _u32Idx ];
    }
    else
    {
        return AdvertContractPtr();
    }
}
       
AdvertContractPtr AdvertContracts::GetContract( AdvertPtr _spAdvert ) const
{
    Unsigned32 u32NumContracts = m_aspContracts.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumContracts; ++u32Idx )
    {
        AdvertContractPtr spContract = m_aspContracts[ u32Idx ];
        if ( spContract->GetAdvert() == _spAdvert )
        {
            return spContract;
        }
    }
    return AdvertContractPtr();
}

