#ifndef CFOUNDATION_SHAREDPTR_H
#define CFOUNDATION_SHAREDPTR_H

#include <memory>

using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;

namespace CFoundation
{
	/// Define for the used shared pointer implementation
    #define SharedPtr std::tr1::shared_ptr

    template< class T >
    class SharedPtrBase : private std::tr1::enable_shared_from_this< T >
    {
    public:
        /// Returns a shared pointer from this class
        SharedPtr< T >          GetSharedPtr()
        {
            return shared_from_this();
        }

        /// Returns a shared pointer from the passed class
        /// There's no check if both classes are compatible.
        template < class T2 >
        SharedPtr< T2 >         GetSharedPtr()
        {
            return static_pointer_cast< T2 >( shared_from_this() );
        }

        /// Returns a shared pointer from this class
        SharedPtr< const T >    GetSharedPtr() const
        {
            return shared_from_this();
        }

        /// Returns a shared pointer from the passed class
        /// There's no check if both classes are compatible.
        template < class T2 >
        SharedPtr< const T2 >   GetSharedPtr() const
        {
            return static_pointer_cast< const T2 >( shared_from_this() );
        }
    };

	/// Define for the used weak pointer implementation
    #define WeakPtr std::tr1::weak_ptr
}

#define INCLUDE_SHARED_CLASS( Class )   class Class;\
                                        typedef SharedPtr< Class > Class##Ptr;\
                                        typedef SharedPtr< const Class > Class##CPtr;

#endif
