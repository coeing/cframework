#include <algorithm>
#include <math.h>

namespace CFoundation
{
	template < class T >
	Circle< T >::Circle()
	{
	}

	template < class T >
	Circle< T >::Circle( T tX, T tY, T tRadius )
	:	m_vPosition( tX, tY ),
        m_Radius( tRadius )
	{
	}

	template < class T >
	Circle< T >::Circle( const Vector2D< T >& _vPosition, T tRadius )
    :	m_vPosition( _vPosition ),
		m_Radius( tRadius )
	{
	}

	template < class T >
    template < class T2 >
    Circle< T >::Circle( const Circle< T2 >& _clRhs )
	{
        m_vPosition = _clRhs.GetPosition();
        m_Radius = _clRhs.GetRadius();
	}

	template < class T >
	void Circle< T >::Reset()
	{
        m_vPosition.Reset();
        m_Radius = 0;
	}

	template <class T>
	void Circle<T>::SetX( T _tX )
	{
		m_vPosition.SetX( _tX );
	}

	template < class T >
	void Circle< T >::SetY( T _tY )
	{
		m_vPosition.SetY( _tY );
	}

	template < class T >
	void Circle< T >::SetRadius( T _tRadius )
	{
		m_Radius = _tRadius;
	}

	template <class T>
	T Circle<T>::GetRadius() const
	{
		return m_Radius;
	}

	template < class T >
	void Circle< T >::SetPosition( T _tX, T _tY )
	{
		m_vPosition.Set( _tX, _tY );
	}

	template <class T>
	void Circle<T>::SetPosition( const Vector2D<T> &clPosition )
	{
		SetPosition( clPosition.GetX(), clPosition.GetY() );
	}

	template <class T>
	T Circle<T>::GetX() const
	{
		return m_vPosition.GetX();
	}

	template <class T>
	T Circle<T>::GetY() const
	{
		return m_vPosition.GetY();
	}

	template < class T >
	const Vector2D< T >& Circle< T >::GetPosition() const
	{
		return m_vPosition;
	}

	template <class T>
	bool Circle<T>::IsPointIn( const Vector2D< T >& _vPos ) const
	{
		return IsPointIn( _vPos.GetX(), _vPos.GetY() );
	}

	template <class T>
	bool Circle<T>::IsPointIn( T tX, T tY ) const
	{
        T tDiffX = tX - GetX();
        T tDiffY = tY - GetY();
		return tDiffX * tDiffX + tDiffY * tDiffY <= GetRadius() * GetRadius();
	}

    //! Casts an Integer32 Circleangle to a Float32 Circleangle
    template < class T >
    template < class T2 >
    const Circle< T >& Circle< T >::operator = ( const Circle< T2 >& _clRhs )
    {
        m_vPosition = _clRhs.m_vPosition;
        m_Radius = _clRhs.m_Radius;

        return *this;
    }
}
