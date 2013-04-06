#include "csystem/Processes/ProcessID.h"

using namespace CSystem;

////////////////////////////////////////////////////////////////////////////////////////////
///
/// Process::ID
///
////////////////////////////////////////////////////////////////////////////////////////////

Process::ID::ID( const boost::thread::id& _threadID )
:   m_threadID( _threadID )
{
}

const Process::ID& Process::ID::operator=( const ID& _rhs )
{
    m_threadID = _rhs.m_threadID;
    return *this;
}

bool Process::ID::operator==( const ID& _rhs )
{
    return m_threadID == _rhs.m_threadID;
}
