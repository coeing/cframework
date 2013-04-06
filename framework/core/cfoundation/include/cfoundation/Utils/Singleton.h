#ifndef CFOUNDATION_SINGLETON_H
#define CFOUNDATION_SINGLETON_H

#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
	//! Template to implement a class as an singleton
	/*! If a class should only have at most one instance
	 *  you can derive the class from this Singleton class.
	 *  The constructor of your class should be made private
	 *  to have full control.
	 *
	 *	\author	Christian Oeing
	 *
	 *	\date	20.10.06
	 *
	 */
	template < class T >
	class Singleton
	{
	public:
		/// Returns the only instance of this class
		static T&       GetInstance();

        /// Destroys the instance
        static void     DestroyInstance();

        /// Initializes the instance
        virtual void    Initialize();

        /// Deinitializes the instance
        virtual void    Deinitialize();

	protected:
		Singleton<T>();

	private:
		static T*       s_pInstance;
	};	

	template< class T > 
	Singleton< T >::Singleton()
	{
	}

	template< class T >
	T& Singleton< T >::GetInstance()
	{
		if ( s_pInstance == NULL )
		{
			s_pInstance = new T();
            s_pInstance->Initialize();
		}
		return *s_pInstance;
	}

	template< class T >
	void Singleton< T >::DestroyInstance()
	{
		if ( s_pInstance != NULL )
		{
            s_pInstance->Deinitialize();
            delete s_pInstance;
            s_pInstance = NULL;
		}
	}

	template< class T >
    void Singleton< T >::Initialize()
    {
    }

	template< class T >
    void Singleton< T >::Deinitialize()
    {
    }

	template< class T >
	T* Singleton< T >::s_pInstance = NULL;

	//! Template to implement a class with reference count as an singleton
	/*! If a class should only have at most one instance
	 *  you can derive the class from this Singleton class.
	 *  The constructor of your class should be made private
	 *  to have full control.
	 *
	 *	\author	Christian Oeing
	 *
	 *	\date	13.01.09
	 *
	 */
	template < class T >
    class RCSingleton : public SharedPtrBase< T >
	{
	public:
		//! Returns the only instance of this class
		static T&       GetInstance();

        //! Initializes the instance
        virtual void    Initialize();

	protected:
		RCSingleton< T >();

	private:
		static SharedPtr< T >   m_spInstance;
	};	

	template< class T > 
	RCSingleton< T >::RCSingleton()
	{
	}

	template< class T >
	T& RCSingleton< T >::GetInstance()
	{
		if ( m_spInstance == NULL )
		{
			m_spInstance = SharedPtr< T >( new T() );
            m_spInstance->Initialize();
		}
		return *m_spInstance;
	}

	template< class T >
    void RCSingleton< T >::Initialize()
    {
    }

	template< class T >
	SharedPtr< T > RCSingleton< T >::m_spInstance;
};

#endif
