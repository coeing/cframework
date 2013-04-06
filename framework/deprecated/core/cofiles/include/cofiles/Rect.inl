#include <algorithm>
#include <math.h>

namespace CoFiles
{
	template < class T >
	Rect< T >::Rect()
	{
	}

	template < class T >
	Rect< T >::Rect( T tX, T tY, T tWidth, T tHeight )
	:	m_vPosition( tX, tY ),
        m_vSize( tWidth, tHeight )
	{
	}

	template < class T >
	Rect< T >::Rect( const Vector2D< T >& _vPosition, const Vector2D< T >& _vSize )
    :	m_vPosition( _vPosition ),
		m_vSize( _vSize )
	{
	}

	template < class T >
    template < class T2 >
    Rect< T >::Rect( const Rect< T2 >& _clRhs )
	{
        m_vPosition = _clRhs.GetPosition();
        m_vSize = _clRhs.GetSize();
	}

	template < class T >
	void Rect< T >::Reset()
	{
        m_vPosition.Reset();
        m_vSize.Reset();
	}

	template <class T>
	void Rect<T>::SetX( T _tX )
	{
		m_vPosition.SetX( _tX );
	}

	template < class T >
	void Rect< T >::SetY( T _tY )
	{
		m_vPosition.SetY( _tY );
	}

	template < class T >
	void Rect< T >::SetPosition( T _tX, T _tY )
	{
		m_vPosition.Set( _tX, _tY );
	}

	template <class T>
	void Rect<T>::SetPosition( const Vector2D<T> &clPosition )
	{
		SetPosition( clPosition.GetX(), clPosition.GetY() );
	}

	template <class T>
	T Rect<T>::GetX() const
	{
		return m_vPosition.GetX();
	}

	template <class T>
	T Rect<T>::GetY() const
	{
		return m_vPosition.GetY();
	}

	template < class T >
	const Vector2D< T >& Rect< T >::GetPosition() const
	{
		return m_vPosition;
	}

	template <class T>
	T Rect<T>::GetTop() const
	{
		return GetY();
	}

	template <class T>
	T Rect<T>::GetLeft() const
	{
		return GetX();
	}

	template <class T>
	T Rect<T>::GetBottom() const
	{
		return GetY() + GetHeight();
	}

	template <class T>
	T Rect<T>::GetRight() const
	{
		return GetX() + GetWidth();
	}

	template <class T>
    Vector2D< T > Rect<T>::GetTopLeft() const
	{
		return Vector2D< T >( GetTop(), GetLeft() );
	}

	template <class T>
    Vector2D< T > Rect<T>::GetTopRight() const
	{
		return Vector2D< T >( GetTop(), GetRight() );
	}
    
	template <class T>
    Vector2D< T > Rect<T>::GetBottomLeft() const
	{
		return Vector2D< T >( GetBottom(), GetLeft() );
	}
    
	template <class T>
    Vector2D< T > Rect<T>::GetBottomRight() const
	{
		return Vector2D< T >( GetBottom(), GetRight() );
	}

	template < class T >
	void Rect< T >::SetSize( const CoFiles::Vector2D< T >& _vSize )
	{
        m_vSize = _vSize;
	}

	template < class T >
	void Rect< T >::SetSize( T _tWidth, T _tHeight )
	{
        m_vSize.Set( _tWidth, _tHeight );
	}

	template <class T>
	T Rect<T>::GetWidth() const
	{
		return m_vSize.GetX();
	}

	template < class T >
	T Rect<T>::GetHeight() const
	{
		return m_vSize.GetY();
	}

	template < class T >
	const Vector2D< T >& Rect< T >::GetSize() const
	{
		return m_vSize;
	}

	template <class T>
	bool Rect<T>::IsPointIn( const CoFiles::Vector2D< T >& _vPos ) const
	{
		return IsPointIn( _vPos.GetX(), _vPos.GetY() );
	}

	template <class T>
	bool Rect<T>::IsPointIn( T tX, T tY ) const
	{
		return (	tX >= GetX()                && tY >= GetY() &&
					tX <= GetX() + GetWidth()   && tY <= GetY() + GetHeight() );
	}

	template <class T>
	BorderType Rect<T>::GetBordersPointIsOn( const CoFiles::Vector2D< T > _tPos, T _tBorderWidth ) const
	{
        return GetBordersPointIsOn( _tPos.GetX(), _tPos.GetY(), _tBorderWidth );
    }

	template< class T >
	BorderType Rect< T >::GetBordersPointIsOn( T tX, T tY, T tBorderWidth ) const
	{
        // Check if inside
        if ( !IsPointIn( tX, tY ) )
        {
            return BORDER_NONE;
        }

        // Get position
        T tPosX = GetX();
        T tPosY = GetY();

        // Get size
        T tWidth = GetWidth();
        T tHeight = GetHeight();

		bool bOnLeftBorder =	tX >= tPosX &&
								tX <= tPosX + tBorderWidth;
		bool bOnRightBorder =	tX >= tPosX + tWidth - tBorderWidth &&
								tX <= tPosX + tWidth;
		bool bOnTopBorder =		tY >= tPosY &&
								tY <= tPosY + tBorderWidth;
		bool bOnBottomBorder =	tY >= tPosY + tHeight - tBorderWidth &&
								tY <= tPosY + tHeight;

		Unsigned8 u8Border = BORDER_NONE;
		if( bOnLeftBorder )
		{
			u8Border |= BORDER_LEFT;
		}
		if( bOnRightBorder )
		{
			u8Border |= BORDER_RIGHT;
		}
		if( bOnTopBorder )
		{
			u8Border |= BORDER_TOP;
		}
		if( bOnBottomBorder )
		{
			u8Border |= BORDER_BOTTOM;
		}

		return BorderType( u8Border );
	}
        
	template <class T>
    void Rect<T>::Expand( const T& _tAmount )
    {
        m_vPosition.Set( m_vPosition.GetX() - _tAmount, m_vPosition.GetY() - _tAmount );
        m_vSize.Set( m_vSize.GetX() + _tAmount * 2, m_vSize.GetY() + _tAmount * 2 );
    }

	template <class T>
	const Rect<T> Rect<T>::Intersect( const Rect<T> &rhs ) const
	{
		Rect clIntersectRect;

		clIntersectRect.SetPosition( std::max( GetX(), rhs.GetX() ), std::max( GetY(), rhs.GetY() ) );

		T tRight = std::min( GetX() + GetWidth(), rhs.GetX() + rhs.GetWidth() );
		T tBottom = std::min( GetY() + GetHeight(), rhs.GetY() + rhs.GetHeight() );

		T tWidth = tRight - clIntersectRect.GetX();
		T tHeight = tBottom - clIntersectRect.GetY();
		tWidth = std::max( 0, tWidth );
		tHeight = std::max( 0, tHeight );
		clIntersectRect.SetSize( tWidth, tHeight );

		return clIntersectRect;
	}

	template < class T >
	void Rect< T >::Expand( const Rect< T >& rhs )
	{
        T tLeft = std::min( GetLeft(), rhs.GetLeft() );
        T tRight = std::max( GetRight(), rhs.GetRight() );
        T tTop = std::min( GetTop(), rhs.GetTop() );
        T tBottom = std::max( GetBottom(), rhs.GetBottom() );

        m_vPosition.Set( tLeft, tTop );
        m_vSize.Set( tRight - tLeft, tBottom - tTop );
	}

    //! Casts an Integer32 rectangle to a Float32 rectangle
    template < class T >
    template < class T2 >
    const Rect< T >& Rect< T >::operator = ( const Rect< T2 >& _clRhs )
    {
        m_vPosition = _clRhs.m_vPosition;
        m_vSize = _clRhs.m_vSize;

        return *this;
    }
}
