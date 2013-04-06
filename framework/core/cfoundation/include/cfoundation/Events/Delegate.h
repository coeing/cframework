#ifndef CFOUNDATION_DELEGATE_H
#define CFOUNDATION_DELEGATE_H

#include "cfoundation/Events/FastDelegate.h"
#include "cfoundation/Utils/SharedPtr.h"

#define Delegate0       fastdelegate::FastDelegate0
typedef SharedPtr< Delegate0<> > Delegate0Ptr;
#define Delegate1       fastdelegate::FastDelegate1
#define Delegate2       fastdelegate::FastDelegate2
#define Delegate3       fastdelegate::FastDelegate3
#define Delegate4       fastdelegate::FastDelegate4
#define DelegateVoid    fastdelegate::detail::DefaultVoid

#define TYPEDEF_DELEGATE0( Enum )   \
    typedef Delegate0<> Enum##Delegate0; \
    typedef SharedPtr< Enum##Delegate0 > Enum##Delegate0Ptr;
#define TYPEDEF_DELEGATE1( Enum, Param1 )   \
    typedef Delegate0<> Enum##Delegate0; \
    typedef SharedPtr< Enum##Delegate0 > Enum##Delegate0Ptr; \
    typedef Delegate1< Param1 > Enum##Delegate1; \
    typedef SharedPtr< Enum##Delegate1 > Enum##Delegate1Ptr;
#define TYPEDEF_DELEGATE2( Enum, Param1, Param2 )   \
    typedef Delegate0<> Enum##Delegate0; \
    typedef SharedPtr< Enum##Delegate0 > Enum##Delegate0Ptr; \
    typedef Delegate1< Param1 > Enum##Delegate1; \
    typedef SharedPtr< Enum##Delegate1 > Enum##Delegate1Ptr; \
    typedef Delegate2< Param1, Param2 > Enum##Delegate2; \
    typedef SharedPtr< Enum##Delegate2 > Enum##Delegate2Ptr;
#define TYPEDEF_DELEGATE3( Enum, Param1, Param2, Param3 )   \
    typedef Delegate0<> Enum##Delegate0; \
    typedef SharedPtr< Enum##Delegate0 > Enum##Delegate0Ptr; \
    typedef Delegate1< Param1 > Enum##Delegate1; \
    typedef SharedPtr< Enum##Delegate1 > Enum##Delegate1Ptr; \
    typedef Delegate2< Param1, Param2 > Enum##Delegate2; \
    typedef SharedPtr< Enum##Delegate2 > Enum##Delegate2Ptr; \
    typedef Delegate3< Param1, Param2, Param3 > Enum##Delegate3; \
    typedef SharedPtr< Enum##Delegate3 > Enum##Delegate3Ptr;
#define TYPEDEF_DELEGATE4( Enum, Param1, Param2, Param3, Param4 )   \
    typedef Delegate0<> Enum##Delegate0; \
    typedef SharedPtr< Enum##Delegate0 > Enum##Delegate0Ptr; \
    typedef Delegate1< Param1 > Enum##Delegate1; \
    typedef SharedPtr< Enum##Delegate1 > Enum##Delegate1Ptr; \
    typedef Delegate2< Param1, Param2 > Enum##Delegate2; \
    typedef SharedPtr< Enum##Delegate2 > Enum##Delegate2Ptr; \
    typedef Delegate3< Param1, Param2, Param3 > Enum##Delegate3; \
    typedef SharedPtr< Enum##Delegate3 > Enum##Delegate3Ptr; \
    typedef Delegate4< Param1, Param2, Param3, Param4 > Enum##Delegate4; \
    typedef SharedPtr< Enum##Delegate4 > Enum##Delegate4Ptr;

namespace CFoundation
{
    template< class T, class RetType >
    SharedPtr< Delegate0<> > CreateDelegate( T* _pThis, RetType (T::*CallbackFunction)() )
    {
        typedef Delegate0<> Delegate;
        typedef SharedPtr< Delegate > DelegatePtr;
        return DelegatePtr( new Delegate( _pThis, CallbackFunction ) );
    }

    template< class T, class A1, class RetType >
    SharedPtr< Delegate1< A1 > > CreateDelegate( T* _pThis, RetType (T::*CallbackFunction)( A1 a1 ) )
    {
        typedef Delegate1< A1 > Delegate;
        typedef SharedPtr< Delegate > DelegatePtr;
        return DelegatePtr( new Delegate( _pThis, CallbackFunction ) );
    }

    template< class T, class A1, class A2, class RetType >
    SharedPtr< Delegate2< A1, A2 > > CreateDelegate( T* _pThis, RetType (T::*CallbackFunction)( A1 a1, A2 a2 ) )
    {
        typedef Delegate2< A1, A2 > Delegate;
        typedef SharedPtr< Delegate > DelegatePtr;
        return DelegatePtr( new Delegate( _pThis, CallbackFunction ) );
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// DelegateConnection
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Returned when registering a delegate
    /// Could be used to disconnect the delegate

    class DelegateConnection
    {
    public:
        /// Disconnects the delegate from the invoker
        virtual void        Disconnect() = 0;
    };

    INCLUDE_SHARED_CLASS( DelegateConnection );     
}

/// Makro to disconnect a delegate connection and check/reset pointer
#define DISCONNECT_DELEGATE_CONNECTION( spDelegateConnectionPtr )   if ( spDelegateConnectionPtr != NULL ) {    \
                                                                        spDelegateConnectionPtr->Disconnect();  \
                                                                        spDelegateConnectionPtr.reset();        \
                                                                    }   

#endif
