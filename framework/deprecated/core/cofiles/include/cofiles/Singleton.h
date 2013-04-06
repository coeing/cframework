#ifndef COFILES_CLIBSINGLETON_H
#define COFILES_CLIBSINGLETON_H

#include "cofiles/Utils/SharedPtr.h"

namespace CoFiles
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
		//! Returns the only instance of this class
		static T&       GetInstance();

        //! Initializes the instance
        virtual void    Initialize();

	protected:
		Singleton<T>();

	private:
		static T*       m_pInstance;
	};	

	template< class T > 
	Singleton< T >::Singleton()
	{
	}

	template< class T >
	T& Singleton< T >::GetInstance()
	{
		if ( m_pInstance == NULL )
		{
			m_pInstance = new T();
            m_pInstance->Initialize();
		}
		return *m_pInstance;
	}

	template< class T >
    void Singleton< T >::Initialize()
    {
    }

	template< class T >
	T* Singleton< T >::m_pInstance = NULL;

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
