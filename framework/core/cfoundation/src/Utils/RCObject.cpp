#include "cfoundation/Utils/RCObject.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Text/String.h"

using namespace CFoundation;

RCObject::RCObject()
:   m_u32RefCount( 0 )
{
}

RCObject::RCObject( const RCObject& /*_rhs*/ )
:   m_u32RefCount( 0 )
{
}

RCObject::~RCObject()
{
}

void RCObject::IncRefCount() const
{
    ++m_u32RefCount;
}

void RCObject::DecRefCount() const
{
    CF_ERROR_IF( m_u32RefCount == 0, "RCObject::DecRefCount: Counter already zero" );
    --m_u32RefCount;
}

Unsigned32 RCObject::GetRefCount() const
{
    return m_u32RefCount;
}