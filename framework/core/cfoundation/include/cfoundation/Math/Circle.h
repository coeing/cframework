#ifndef CFOUNDATION_CIRCLE_H
#define CFOUNDATION_CIRCLE_H

#include "cfoundation/Math/BorderType.h"
#include "cfoundation/Math/Vector.h"

namespace CFoundation
{
    /// Template class for Circles
    /** Allows to create a Circles out of every possible
     *  type (like integers, floats,...)
     */
	template < class T >
	class Circle
	{
	public:
        /// Default constructor
		Circle();

        /// Constructor
		Circle( const Vector2D< T >& _vPosition, T tRadius );

        /// Constructor
		Circle( T tX, T tY, T tRadius );
        
        /// Assign constructor
        template < class T2 >
        Circle( const Circle< T2 >& _clRhs );

		void                    Reset();

		void                    SetX( T tX );
		void                    SetY( T tY );

        /// Sets the radius
        void                    SetRadius( T _tWidth );

        /// Returns the radius
		T                       GetRadius() const;

        /// Sets the Circleangle position
		void                    SetPosition( T tX, T tY );

        /// Sets the Circleangle position
		void                    SetPosition( const Vector2D< T >& _clPosition );

        /// Returns the Circleangle position
		const Vector2D< T >&    GetPosition() const;

        /// Checks if the passed point is inside the circle
		bool                    IsPointIn( T tX, T tY ) const;

        /// Checks if the passed point is inside the circle
		bool                    IsPointIn( const Vector2D< T >& _vPoint ) const;
        
        /// Casts a Circleangle to another type
        template < class T2 >
        const Circle< T >&      operator = ( const Circle< T2 >& _clRhs );

	private:
        /// Position
		Vector2D< T >           m_vPosition;

        /// Radius
        T                       m_Radius;
	};

	typedef Circle< Integer16 >	    CircleI16;
	typedef Circle< Integer32 >	    CircleI32;
	typedef Circle< Float16 >	    CircleF16;
	typedef Circle< Float32 >	    CircleF32;
}

#include "Circle.inl"

#endif
