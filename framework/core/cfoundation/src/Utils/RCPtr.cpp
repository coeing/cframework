#include "cfoundation/Utils/RCPtr.h"

using namespace CFoundation;

template <class T>
RCPtr<T>::RCPtr( T *pT )
:	m_pT( pT )
{
	Init();
}

template <class T>
RCPtr<T>::RCPtr( const RCPtr &rhs )
:	m_pT( rhs.m_pT )
{
	Init();
}

template <class T>
RCPtr<T>::~RCPtr()
{
	if (m_pT != 0)
	{
		m_pT->DecReference();
	}
}

template <class T>
RCPtr<T>& RCPtr<T>::operator=( const RCPtr<T> &rhs )
{
	if (m_pT != rhs.m_pT)
	{
		if (m_pT != 0)
		{
			m_pT->DecReference();
		}
		m_pT = rhs.m_pT;
		Init();
	}
	return *this;
}

template <class T>
T* RCPtr<T>::operator->() const
{
	return m_pT;
}

template <class T>
T& RCPtr<T>::operator*() const
{
	return *m_pT;
}

template <class T>
void RCPtr<T>::Init()
{
	if (m_pT == 0)
	{
		return;
	}

	if (!m_pT->IsShareable())
	{
		m_pT = new T(*m_pT);
	}
	m_pT->IncReference();
}
