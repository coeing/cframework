#include "cmadtv/Data/Player/PlayerCompBetty.h"

#include "cfoundation/Base/Defines.h"
#include "cfoundation/Math/Utils.h"
#include "cfoundation/Serialization/Serializer.h"
#include "cfoundation/Serialization/SerializationContext.h"

#include "cmadtv/Data/Betty/Present.h"

using namespace CMadTV;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// PlayerCompBetty
///
////////////////////////////////////////////////////////////////////////////////////////////

PlayerCompBetty::PlayerCompBetty()
:   m_u8Love( 0 )
{
}

PlayerCompBetty::~PlayerCompBetty()
{
}
        
void PlayerCompBetty::Serialize( CFoundation::Serializer& _Serializer )
{
    Unsigned32 u32Version = 0;
    _Serializer.SerializeVersion( u32Version, 2 );

    _Serializer.SerializeProxy< CFoundation::SerializeWrapper_Context >( m_spPresent );
    _Serializer.Serialize( m_bPresentReturnEnabled );
    if ( u32Version >= 2 )
    {
        _Serializer.Serialize( m_u8Love );
    }
}
       
void PlayerCompBetty::SetPresent( PresentPtr _spPresent )
{
    if ( _spPresent == m_spPresent )
    {
        return;
    }

    m_spPresent = _spPresent;

    // Dispatch event
    DispatchEvent( EVENT_BETTY_PRESENT_CHANGED, m_spPresent );
}

PresentPtr PlayerCompBetty::GetPresent() const
{
    return m_spPresent;
}

void PlayerCompBetty::EnablePresentReturn( bool _bEnabled )
{
    m_bPresentReturnEnabled = _bEnabled;
}

bool PlayerCompBetty::IsPresentReturnEnabled() const
{
    return m_bPresentReturnEnabled;
}
       
void PlayerCompBetty::AddLove( Integer8 _i8Love )
{
    Integer8 i8NewLove = m_u8Love;
    i8NewLove += _i8Love;
    CLAMP( i8NewLove, 0, 100 );
    SetLove( i8NewLove );
}

Unsigned8 PlayerCompBetty::GetLove() const
{
    return m_u8Love;
}
      
void PlayerCompBetty::SetLove( Unsigned8 _u8Love )
{
    if ( _u8Love == m_u8Love )
    {
        return;
    }
    
    Unsigned8 u8OldLove = m_u8Love;
    m_u8Love = _u8Love;

    // Dispatch event
    DispatchEvent( EVENT_BETTY_LOVE_CHANGED, m_u8Love, u8OldLove );
}
