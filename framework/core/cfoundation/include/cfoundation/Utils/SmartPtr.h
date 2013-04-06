#ifndef CFOUNDATION_SMARTPTR_H
#define CFOUNDATION_SMARTPTR_H

#include "cfoundation/Base/Defines.h"

namespace CFoundation
{
	//! Implementation of a smart pointer
	/*! A smart pointer replaces the normal c++ pointers.
	 *  It is more handy. You can assign a pointer to
	 *  a SmartPtr and it will take care about its
	 *  destruction.
     *  The object must implement a reference count
     *  so the smart pointer nows when to destroy the
     *  object.
	 *
	 *  \author	Christian Oeing
	 * 
	 *  \date	21.10.06
	 *
	 */
    /* NOTE: Not fast enough at the moment
	template < class T >
	class SmartPtr
	{
	public:
        //! Constructor
		SmartPtr( T* pT = NULL )
		{
            Lock( pT );
		}

        //! Copy constructor
		SmartPtr( const SmartPtr< T >& rhs )
		{
            Lock( rhs.m_pT );
		}

        //! Destructor
		~SmartPtr()
		{
            // Release current object
            Release( m_pT );
		}
		
        //! Assign operator
		SmartPtr< T >& operator=( T* _pT )
		{
			if ( m_pT == _pT )
			{
				return *this;
			}

            T* pOldT = m_pT;

            // Lock the new object
            Lock( _pT );
            
            // Release old object
            Release( pOldT );

			return *this;
		}

        //! Assign operator
		SmartPtr< T >& operator=( const SmartPtr< T >& rhs )
		{
			if ( this == &rhs )
			{
				return *this;
			}

            T* pOldT = m_pT;

            // Lock the object from the passed pointer
            Lock( rhs.m_pT );
            
            // Release old object
            Release( pOldT );

			return *this;
		}

		T& operator*() const
		{
			return *m_pT;
		}

		T* operator->() const
		{
			return m_pT;
		}

        operator T* ()
        {
            return m_pT;
        }

        operator T* () const
        {
            return m_pT;
        }

        //! Indicates if the pointer is valid
		bool    IsValid() const
		{
			return m_pT != NULL;
		}

		template <class newType>
		operator SmartPtr<newType>()
		{
			return SmartPtr<newType>( m_pT );
		}

		operator SmartPtr<const T>()
		{
			return SmartPtr<const T>( m_pT );
		}

	private:
        //! Locks the passed object
        void    Lock( T* _pT )
        {
			m_pT = _pT;	

            // Increment the reference counter
            if ( m_pT != NULL )
            {
                m_pT->IncRefCount();
            }
        }

        //! Releases the current pointer
        void    Release( T* _pT )
        {
            if ( _pT != NULL )
            {
                _pT->DecRefCount();

                // If last reference delete object
                if ( _pT->GetRefCount() == 0 )
                {
				    delete _pT;
                }
            }
        }

        //! The object pointer
		T*      m_pT;
	};*/
};

#endif
