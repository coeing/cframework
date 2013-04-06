#ifndef COFILES_RECT_H
#define COFILES_RECT_H

#include "cofiles/BorderType.h"
#include "cofiles/Vector.h"

namespace CoFiles
{
    //! Template class for rectangles
    /*! Allows to create a rectangle out of every possible
     *  type (like integers, floats,...)
     */
	template < class T >
	class Rect
	{
	public:
        //! Default constructor
		Rect();

        //! Constructor
		Rect( const Vector2D< T >& _vPosition, const Vector2D< T >& _vSize );

        //! Constructor
		Rect( T tX, T tY, T tWidth, T tHeight );
        
        //! Assign constructor
        template < class T2 >
        Rect( const Rect< T2 >& _clRhs );

		void                    Reset();

		void                    SetX( T tX );
		void                    SetY( T tY );

        //! Sets the rectangle position
		void                    SetPosition( T tX, T tY );

        //! Sets the rectangle position
		void                    SetPosition( const Vector2D< T >& _clPosition );

        //! Returns the rectangle position
		const Vector2D< T >&    GetPosition() const;

		T                       GetX() const;
		T                       GetY() const;

		T                       GetTop() const;
		T                       GetLeft() const;
		T                       GetBottom() const;
		T                       GetRight() const;

        //! Returns the top left point
        Vector2D< T >           GetTopLeft() const;

        //! Returns the top right point
        Vector2D< T >           GetTopRight() const;
        
        //! Returns the bottom left point
        Vector2D< T >           GetBottomLeft() const;
        
        //! Returns the bottom right point
        Vector2D< T >           GetBottomRight() const;

        //! Sets the rectangle size
        void                    SetSize( const CoFiles::Vector2D< T >& _vSize );

        //! Sets the rectangle size
		void                    SetSize( T tWidth, T tHeight );

        //! Returns the rectangle width
		T                       GetWidth() const;

        //! Returns the rectangle height
		T                       GetHeight() const;

        const Vector2D< T >&    GetSize() const;

		bool                    IsPointIn( T tX, T tY ) const;
		bool                    IsPointIn( const CoFiles::Vector2D< T >& _vPoint ) const;

		BorderType              GetBordersPointIsOn( T tX, T tY, T tBorderWidth ) const;
		BorderType              GetBordersPointIsOn( const CoFiles::Vector2D< T > _tPos, T _tBorderWidth ) const;

        //! Returns the intersection rectangle between this
        //! and the passed rectangle
		const Rect< T >         Intersect( const Rect< T > &rhs ) const;

        //! Expands this rectangle so that the passed 
        //! rectangle fits in
		void                    Expand( const Rect< T >& _rhs );

        //! Expands this rectangle by the passed amount on each side
        void                    Expand( const T& _tAmount );
        
        //! Casts a rectangle to another type
        template < class T2 >
        const Rect< T >&        operator = ( const Rect< T2 >& _clRhs );

	private:
        //! Position
		Vector2D< T >           m_vPosition;

        //! Size
        Vector2D< T >           m_vSize;
	};

	typedef Rect< Integer16 >	RectI16;
	typedef Rect< Integer32 >	RectI32;
	typedef Rect< Float16 >	    RectF16;
	typedef Rect< Float32 >	    RectF32;
}

#include "Rect.inl"

#endif
