#include "cofiles/RCObject.h"

#include "cofiles/Makros.h"

using namespace CoFiles;

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
    CO_ERROR_IF( m_u32RefCount == 0, "RCObject::DecRefCount: Counter already zero" );
    --m_u32RefCount;
}

Unsigned32 RCObject::GetRefCount() const
{
    return m_u32RefCount;
}