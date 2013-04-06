#include "cmadtv/Data/News/NewsInstance.h"

#include <math.h>

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Math/Utils.h"
#include "cfoundation/Finance/Money.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/News/News.h"
#include "cmadtv/Data/Game/Game.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// NewsInstance
///
////////////////////////////////////////////////////////////////////////////////////////////
RTTI_DEF_INST_EASY( NewsInstance )

NewsInstance::NewsInstance( Unsigned32 _u32ID )
:   m_u32ID( _u32ID )
{
}

NewsInstance::~NewsInstance()
{
}
       
void NewsInstance::Init( GamePtr _spGame, NewsPtr _spNews, const CFoundation::DateTime& _DateTime )
{
    m_spGame = _spGame;
    m_spNews = _spNews;
    m_DateTime = _DateTime;
}
        
void NewsInstance::Deinit()
{
}

void NewsInstance::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 1 );

    _Serializer.Serialize( m_u32ID );
    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_Context >( m_spNews );
    m_DateTime.Serialize( _Serializer );
}

GamePtr NewsInstance::GetGame() const
{
    return m_spGame;
}
       
Unsigned32 NewsInstance::GetID() const
{
    return m_u32ID;
}

NewsPtr NewsInstance::GetNews() const
{
    return m_spNews;
}

const CFoundation::DateTime& NewsInstance::GetDateTime() const
{
    return m_DateTime;
}
        
void NewsInstance::GetAge( Integer64& _i64Days, Integer32& _i32Hours ) const
{
    if ( m_spGame == NULL )
    {
        _i64Days = 0;
        _i32Hours = 0;
        return;
    }

    // Get current date/time
    const CFoundation::DateTime& currentDateTime = m_spGame->GetDateTime();

    // Compute age
    _i64Days = currentDateTime.GetDate() - m_DateTime.GetDate();
    _i32Hours = ( currentDateTime.GetTime() - m_DateTime.GetTime() ) / 3600;
}

CFoundation::Money NewsInstance::GetCurrentPrice() const
{
    if ( m_spNews == NULL )
    {
        return CFoundation::Money();
    }

    // Get base price
    const CFoundation::Money& basePrice = m_spNews->GetPrice();

    // Get age (in hours)
    Integer64 i64Days = 0;
    Integer32 i32Hours = 0;
    GetAge( i64Days, i32Hours );

    // Compute current price
    if ( i64Days <= 0 &&
         i32Hours <= 0 )
    {
        CF_WARN_IF( i64Days < 0 || i32Hours < 0, "NewsInstance: Date is before current date." );
        return basePrice;
    }
    else if ( i64Days > 0 ||
              i32Hours > 9 )
    {
        return CFoundation::Money( 1000, CFoundation::CURRENCY_DOLLAR );
    }
    else
    {
        Float32 f32Amount = ( 1 - log10f( i32Hours + 1 ) ) * basePrice.GetAmount( CFoundation::CURRENCY_DOLLAR );
        f32Amount = CFoundation::Math::Round( f32Amount / 1000 ) * 1000;
        return CFoundation::Money( std::max( f32Amount, 1000.0 ), CFoundation::CURRENCY_DOLLAR );
    }
}
 