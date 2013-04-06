#ifndef CFOUNDATION_RCOBJECT_H
#define CFOUNDATION_RCOBJECT_H

#include "cfoundation/Base/Types.h"

namespace CFoundation
{
	//! Implementation of a reference count object
	/*! A RCObject is an object which has a reference
     *  counter to count how often it is used in the 
     *  application.
     *  There should be no normal pointers of RCObjects
     *  instead the SmartPtr class should be used 
     *  which takes care of incrementing and decrementing
     *  the reference counting and deleting the object
     *  if there's no reference left.
	 *
	 *  \author	Christian Oeing
	 * 
	 *  \date	25.12.08
	 *
	 */
	class RCObject
	{
	public:
        //! Constructor
		RCObject();

        //! Copy constructor
		RCObject( const RCObject& rhs );

        //! Destructor
		virtual ~RCObject() = 0;

        //! Increments the reference count
		void        IncRefCount() const;

        //! Decrements the reference count
		void        DecRefCount() const;

        //! Returns the current reference count
        Unsigned32  GetRefCount() const;

		void        SetUnshareable();
		bool        IsShareable();

		bool        IsShared();

	private:
        //! The current reference count
		mutable Unsigned32  m_u32RefCount;

        //! Indicates if the object is shareable
		bool		        m_bShareable;
	};
};

#endif
