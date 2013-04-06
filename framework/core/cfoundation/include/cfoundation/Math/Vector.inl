#include "Vector.h"

#include <math.h>
#include "cfoundation/Serialization/Serializer.h"

namespace CFoundation
{
	template <class T>
	Vector2D<T>::Vector2D( const T& tX, const T& tY )
	:	m_tX( tX ),
		m_tY( tY )
	{
	}

	template <class T>
	Vector2D<T>::Vector2D( const Vector2D<T> &rhs )
	:	m_tX( rhs.m_tX ),
		m_tY( rhs.m_tY )
	{
	}

	template < class T >
    template < class T2 >
    Vector2D< T >::Vector2D( const Vector2D< T2 >& _clRhs )
	{
        m_tX = static_cast< T >( _clRhs.GetX() );
        m_tY = static_cast< T >( _clRhs.GetY() );
	}
		
	template < class T >	
	void Vector2D< T >::Serialize( Serializer& _Serializer )
	{
        _Serializer.Serialize( m_tX );
        _Serializer.Serialize( m_tY );
	}
        
	template < class T >
    bool Vector2D<T>::IsZero() const
    {
        return m_tX == 0 &&
               m_tY == 0;
    }
        
	template <class T>
    void Vector2D<T>::Reset()
    {
        Set( 0, 0 );
    }

	template <class T>
	void Vector2D<T>::Set( const T& tX, const T& tY )
	{
		m_tX = tX;
		m_tY = tY;
	}

	template <class T>
	void Vector2D<T>::SetX( const T& tX )
	{
		m_tX = tX;
	}

	template <class T>
	void Vector2D<T>::SetY( const T& tY )
	{
		m_tY = tY;
	}

	template <class T>
	void Vector2D<T>::Add( const T& tX, const T& tY )
	{
		m_tX += tX;
		m_tY += tY;
	}

	template <class T>
	void Vector2D<T>::AddX( const T& tX )
	{
		m_tX += tX;
	}

	template <class T>
	void Vector2D<T>::AddY( const T& tY )
	{
		m_tY += tY;
	}

	template <class T>
	void Vector2D<T>::Sub( const T& tX, const T& tY )
	{
		m_tX -= tX;
		m_tY -= tY;
	}

	template <class T>
	void Vector2D<T>::SubX( const T& tX )
	{
		m_tX -= tX;
	}

	template <class T>
	void Vector2D<T>::SubY( const T& tY )
	{
		m_tY -= tY;
	}

	template <class T>
	const T& Vector2D<T>::GetX() const
	{
		return m_tX;
	}

	template < class T >
	const T& Vector2D< T >::GetY() const
	{
		return m_tY;
    }

	template < class T >
    const T Vector2D< T >::GetLength() const
	{
        return std::sqrt( GetSquaredLength() );
    }

	template < class T >
    const T Vector2D< T >::GetSquaredLength() const
	{
		return m_tX * m_tX + m_tY * m_tY;
    }

	template <class T>
	Vector2D<T>& Vector2D<T>::operator+=( const Vector2D<T> &rhs )
	{
		m_tX += rhs.m_tX;
		m_tY += rhs.m_tY;

		return *this;
	}

	template <class T>
	Vector2D<T>& Vector2D<T>::operator-=( const Vector2D<T> &rhs )
	{
		m_tX -= rhs.m_tX;
		m_tY -= rhs.m_tY;

		return *this;
	}

	template <class T>
	Vector2D<T>& Vector2D<T>::operator*=( Float32 rhs )
	{
		m_tX *= rhs;
		m_tY *= rhs;

		return *this;
	}

	template <class T>
	Vector2D<T>& Vector2D<T>::operator/=( Float32 rhs )
	{
		m_tX = static_cast< T >( m_tX / rhs );
		m_tY = static_cast< T >( m_tY / rhs );

		return *this;
	}

	template <class T>
	bool Vector2D<T>::operator==( const Vector2D<T> &rhs ) const
	{
		return m_tX == rhs.m_tX &&
			   m_tY == rhs.m_tY;
	}

	template <class T>
	bool Vector2D<T>::operator!=( const Vector2D<T> &rhs ) const
	{
		return !( *this == rhs );
	}
		
	template <class T>	
	void Vector2D<T>::Save( Serializer &clSerializer ) const
	{
		clSerializer.AddData( &m_tX, sizeof( T ) );
		clSerializer.AddData( &m_tY, sizeof( T ) );
	}

	template <class T>		
	void Vector2D<T>::Load( const Serializer &clSerializer )
	{
		m_tX = *static_cast<const T*>( clSerializer.GetData( sizeof( T ) ) ); 
		m_tY = *static_cast<const T*>( clSerializer.GetData( sizeof( T ) ) ); 
	}

	template <class T>
	const Vector2D<T> operator+( const Vector2D<T> &lhs, const Vector2D<T> &rhs )
	{
		return Vector2D<T>( lhs ) += rhs;
	}

	template <class T>
	const Vector2D<T> operator-( const Vector2D<T> &lhs, const Vector2D<T> &rhs )
	{
		return Vector2D<T>( lhs ) -= rhs;
	}

	template <class T>
	const Vector2D<T> operator*( const Vector2D<T>& lhs, Float32 rhs )
	{
		return Vector2D<T>( lhs ) *= rhs;
	}

	template <class T>
	const Vector2D<T> operator/( const Vector2D<T>& lhs, Float32 rhs )
	{
		return Vector2D<T>( lhs ) /= rhs;
	}
}

namespace CFoundation
{
	template <class T>
	Vector3D<T>::Vector3D( const T& tX, const T& tY, const T& tZ )
	:	m_tX( tX ),
		m_tY( tY ),
		m_tZ( tZ )
	{
	}
		
	template < class T >	
	void Vector3D< T >::Serialize( Serializer& _Serializer )
	{
        _Serializer.Serialize( m_tX );
        _Serializer.Serialize( m_tY );
        _Serializer.Serialize( m_tZ );
	}

	template <class T>
	bool Vector3D<T>::IsZero() const
    {
        return m_tX == 0 &&
               m_tY == 0 &&
               m_tZ == 0;
    }

	template <class T>
	void Vector3D<T>::Reset()
    {
        Set( 0, 0, 0 );
    }

	template <class T>
	void Vector3D<T>::Set( const T& tX, const T& tY, const T& tZ )
	{
		m_tX = tX;
		m_tY = tY;
		m_tZ = tZ;
	}

	template <class T>
	void Vector3D<T>::SetX( const T& tX )
	{
		m_tX = tX;
	}

	template <class T>
	void Vector3D<T>::SetY( const T& tY )
	{
		m_tY = tY;
	}

	template <class T>
	void Vector3D<T>::SetZ( const T& tZ )
	{
		m_tZ = tZ;
	}

	template <class T>
	void Vector3D<T>::Set( Unsigned8 u8Idx, T t )
	{
		switch( u8Idx )
		{
		case 0:
			m_tX = t;
			break;
		case 1:
			m_tY = t;
			break;
		default:
			m_tZ = t;
			break;
		}
	}

	template <class T>
	void Vector3D<T>::Add( const T& tX, const T& tY, const T& tZ )
	{
		m_tX += tX;
		m_tY += tY;
		m_tZ += tZ;
	}

	template <class T>
	void Vector3D<T>::AddX( const T& tX )
	{
		m_tX += tX;
	}

	template <class T>
	void Vector3D<T>::AddY( const T& tY )
	{
		m_tY += tY;
	}

	template <class T>
	void Vector3D<T>::AddZ( const T& tZ )
	{
		m_tZ += tZ;
	}

	template <class T>
	const T& Vector3D<T>::GetX() const
	{
		return m_tX;
	}

	template <class T>
	const T& Vector3D<T>::GetY() const
	{
		return m_tY;
	}

	template <class T>
	const T& Vector3D<T>::GetZ() const
	{
		return m_tZ;
	}

	template <class T>
	const T& Vector3D<T>::Get( Unsigned8 u8Idx ) const
	{
		switch( u8Idx )
		{
		case 0:
			return m_tX;
		case 1:
			return m_tY;
		default:
			return m_tZ;
		}
	}

	template <class T>
	Vector3D<T>& Vector3D<T>::operator+=( const Vector3D<T> &rhs )
	{
		m_tX += rhs.m_tX;
		m_tY += rhs.m_tY;
		m_tZ += rhs.m_tZ;

		return *this;
	}

	template <class T>
	Vector3D<T>& Vector3D<T>::operator+=( const Vector2D<T> &rhs )
	{
		m_tX += rhs.GetX();
		m_tY += rhs.GetY();

		return *this;
	}

	template <class T>
	Vector3D<T>& Vector3D<T>::operator-=( const Vector3D<T> &rhs )
	{
		m_tX -= rhs.m_tX;
		m_tY -= rhs.m_tY;
		m_tZ -= rhs.m_tZ;

		return *this;
	}

	template <class T>
	Vector3D<T>& Vector3D<T>::operator-=( const Vector2D<T> &rhs )
	{
		m_tX -= rhs.GetX();
		m_tY -= rhs.GetY();

		return *this;
	}

	template <class T>
	Vector3D<T>& Vector3D<T>::operator*=( double d )
	{
		m_tX *= d;
		m_tY *= d;
		m_tZ *= d;

		return *this;
	}
			
	template <class T>	
	double Vector3D<T>::Dot( const Vector3D<T> &rhs ) const
	{
		return	m_tX * rhs.m_tX + 
				m_tY * rhs.m_tY + 
				m_tZ * rhs.m_tZ;
	}

	template <class T>	
	const Vector3D<T> Vector3D<T>::Cross( const Vector3D<T> &rhs ) const
	{
		T x = m_tY * rhs.m_tZ - m_tZ * rhs.m_tY;
		T y = m_tZ * rhs.m_tX - m_tX * rhs.m_tZ;
		T z = m_tX * rhs.m_tY - m_tY * rhs.m_tX;

		return Vector3D<T>( x, y, z );
	}

	template <class T>
	Float32 Vector3D<T>::Length() const
	{
		return sqrt( *this * *this );
	}

	template <class T>
	Vector3D<T> Vector3D<T>::Normalized() const
	{
		Vector3D<T> t( *this );
		t.Normalize();
		return t;
	}

	template <class T>
	void Vector3D<T>::Normalize()
	{
		Float32 fLength = Length();
		if( fLength != 0 )
		{
			m_tX = m_tX / fLength;
			m_tY = m_tY / fLength;
			m_tZ = m_tZ / fLength;
		}
	}
		
	template <class T>	
	Float32 Vector3D<T>::Cos( const Vector3D<T> &rhs ) const
	{
		return ( *this * rhs ) / ( Length() * rhs.Length() );
	}
			
	template <class T>	
	double Vector3D<T>::Dot( const Vector3D<T> &lhs, const Vector3D<T> &rhs )
	{
		return Vector3D<T>( lhs ).Dot( rhs );
	}

	template <class T>	
	const Vector3D<T> Vector3D<T>::Cross( const Vector3D<T> &lhs, const Vector3D<T> &rhs )
	{
		return Vector3D<T>( lhs ).Cross( rhs );
	}

	template <class T>
	const Vector3D<T> operator-( const Vector3D<T> &lhs )
	{
		return Vector3D<T>( lhs ) *= -1;
	}

	template <class T>
	const Vector3D<T> operator+( const Vector3D<T> &lhs, const Vector3D<T> &rhs )
	{
		return Vector3D<T>( lhs ) += rhs;
	}

	template <class T>
	const Vector3D<T> operator+( const Vector3D<T> &lhs, const Vector2D<T> &rhs )
	{
		return Vector3D<T>( lhs ) += rhs;
	}

	template <class T>
	const Vector3D<T> operator-( const Vector3D<T> &lhs, const Vector3D<T> &rhs )
	{
		return Vector3D<T>( lhs ) -= rhs;
	}

	template <class T>
	const Vector3D<T> operator-( const Vector3D<T> &lhs, const Vector2D<T> &rhs )
	{
		return Vector3D<T>( lhs ) -= rhs;
	}
		
	template <class T>
	const Vector3D<T> operator*( double d, const Vector3D<T> &rhs )
	{
		return Vector3D<T>( rhs ) *= d;
	}
		
	template <class T>
	const Vector3D<T> operator*( const Vector3D<T> &rhs, double d )
	{
		return Vector3D<T>( rhs ) *= d;
	}
		
	template <class T>
	double operator*( const Vector3D<T> &lhs, const Vector3D<T> &rhs )
	{
		return lhs.GetX() * rhs.GetX() + lhs.GetY() * rhs.GetY() + lhs.GetZ() * rhs.GetZ();
	}
}
