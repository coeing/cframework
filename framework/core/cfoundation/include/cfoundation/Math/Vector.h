#ifndef CFOUNDATION_VECTOR_H
#define CFOUNDATION_VECTOR_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Serialization/Serializable.h"

namespace CFoundation
{
	template <class T>
	class Vector2D
	{
	public:
        //! Constructor
		explicit Vector2D( const T& tX = 0, const T& tY = 0 );

        //! Assign constructor
		Vector2D( const Vector2D< T >& _clRhs );
        
        //! Assign constructor
        template < class T2 >
        Vector2D( const Vector2D< T2 >& _clRhs );
		
        /// Serialization
        void        Serialize( Serializer& _Serializer );

        /// Indicates if all members are zero
        bool        IsZero() const;

        /// Sets the vector to zero
        void        Reset();

		void        Set( const T& tX, const T& tY );
		void        SetX( const T& tX );
		void        SetY( const T& tY );

		void        Add( const T& tX, const T& tY );
		void        AddX( const T& tX );
		void        AddY( const T& tY );

		void        Sub( const T& tX, const T& tY );
		void        SubX( const T& tX );
		void        SubY( const T& tY );

		const T&    GetX() const;
		const T&    GetY() const;

        //! Returns the length of the vector
        const T     GetLength() const;

        //! Returns the squared length of the vector
        const T     GetSquaredLength() const;

		Vector2D<T>& operator+=( const Vector2D<T>& rhs );
		Vector2D<T>& operator-=( const Vector2D<T>& rhs );
		Vector2D<T>& operator*=( Float32 rhs );
		Vector2D<T>& operator/=( Float32 rhs );

		bool operator==( const Vector2D<T>& rhs ) const;
		bool operator!=( const Vector2D<T>& rhs ) const;

		virtual void Save( Serializer& clSerializer ) const;
		virtual void Load( const Serializer& clSerializer );

	private:
		T	m_tX;
		T	m_tY;
	};
		
	template <class T>
	const Vector2D<T> operator+( const Vector2D<T>& lhs, const Vector2D<T>& rhs );
		
	template <class T>
	const Vector2D<T> operator-( const Vector2D<T>& lhs, const Vector2D<T>& rhs );
		
	template <class T>
	const Vector2D<T> operator*( const Vector2D<T>& lhs, Float32 rhs );
		
	template <class T>
	const Vector2D<T> operator/( const Vector2D<T>& lhs, Float32 rhs );

	typedef Vector2D<Unsigned8>		Vector2Du8;
	typedef Vector2D<Unsigned16>	Vector2Du16;
	typedef Vector2D<Unsigned32>	Vector2Du32;

	typedef Vector2D<Integer8>		Vector2Di8;
	typedef Vector2D<Integer16>		Vector2Di16;
	typedef Vector2D<Integer32>		Vector2Di32;

	typedef Vector2D<Unsigned32>	Vector2Du;
	typedef Vector2D<Integer32>		Vector2Di;

	typedef Vector2D<Float32>		Vector2Df;

	template <class T>
	class Vector3D
	{
	public:
		Vector3D( const T& tX = 0, const T& tY = 0, const T& tZ = 0 );
		
        /// Serialization
        void        Serialize( Serializer& _Serializer );

        /// Indicates if all members are zero
        bool        IsZero() const;

        /// Sets the vector to zero
        void        Reset();

		void Set( const T& tX, const T& tY, const T& tZ );
		void SetX( const T& tX );
		void SetY( const T& tY );
		void SetZ( const T& tZ );

		void Set( Unsigned8 u8Idx, T t );

		void Add( const T& tX, const T& tY, const T& tZ );
		void AddX( const T& tX );
		void AddY( const T& tY );
		void AddZ( const T& tZ );

		const T& GetX() const;
		const T& GetY() const;
		const T& GetZ() const;

		const T& Get( Unsigned8 u8Idx ) const;

		Vector3D<T>& operator+=( const Vector3D<T> &rhs );
		Vector3D<T>& operator+=( const Vector2D<T> &rhs );

		Vector3D<T>& operator-=( const Vector3D<T> &rhs );
		Vector3D<T>& operator-=( const Vector2D<T> &rhs );

		Vector3D<T>& operator*=( double d );
		
		double Dot( const Vector3D<T> &rhs ) const;
		const Vector3D<T> Cross( const Vector3D<T> &rhs ) const;

		Float32 Length() const;
		Vector3D<T> Normalized() const;
		void Normalize();

		Float32 Cos( const Vector3D<T> &rhs ) const;
		
		static double Dot( const Vector3D<T> &lhs, const Vector3D<T> &rhs );
		static const Vector3D<T> Cross( const Vector3D<T> &lhs, const Vector3D<T> &rhs );

	private:
		T	m_tX;
		T	m_tY;
		T	m_tZ;
	};

	template <class T>
	const Vector3D<T> operator-( const Vector3D<T> &lhs );
		
	template <class T>
	const Vector3D<T> operator+( const Vector3D<T> &lhs, const Vector3D<T> &rhs );
		
	template <class T>
	const Vector3D<T> operator+( const Vector3D<T> &lhs, const Vector2D<T> &rhs );
		
	template <class T>
	const Vector3D<T> operator-( const Vector3D<T> &lhs, const Vector3D<T> &rhs );
		
	template <class T>
	const Vector3D<T> operator-( const Vector3D<T> &lhs, const Vector2D<T> &rhs );
		
	template <class T>
	const Vector3D<T> operator*( double d, const Vector3D<T> &rhs );
		
	template <class T>
	const Vector3D<T> operator*( const Vector3D<T> &rhs, double d );
		
	template <class T>
	double operator*( const Vector3D<T> &lhs, const Vector3D<T> &rhs );

	typedef Vector3D<Integer32>	Vector3Di;
	typedef Vector3D<Float32>	Vector3Df;
};

#include "cfoundation/Math/Vector.inl"

#endif
