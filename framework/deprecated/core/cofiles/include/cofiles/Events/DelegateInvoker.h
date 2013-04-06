#ifndef COFILES_DELEGATE_INVOKER_H
#define COFILES_DELEGATE_INVOKER_H

#include <map>
#include <vector>

#include "cofiles/Events/Delegate.h"
#include "cofiles/Utils/SharedPtr.h"

namespace CoFiles
{
    /// Base class for all delegate invokers
    template < class Enum, class Delegate >
    class DelegateInvoker
    {
    public:
        //! Registers a delegate for the passed event
        void    RegisterDelegate( Enum _eEvent, SharedPtr< Delegate > _spDelegate );

        //! Removes a delegate
        void    RemoveDelegate( Enum _eEvent, SharedPtr< Delegate > _spDelegate );

    protected:
        typedef std::vector< SharedPtr< Delegate > > DelegateVector;

        /// Returns the delegates for the passed event
        DelegateVector*         GetDelegates( Enum _eEvent );

    private:
        typedef std::map< Enum, DelegateVector* > EventDelegatesMap;

        /// Delegates
        EventDelegatesMap       m_mapEventDelegates;
    };

    /// Delegate invoker for 0 params
    template < class Enum, class RetType = DelegateVoid >
    class Delegate0Invoker : public DelegateInvoker< Enum, Delegate0< RetType > >
    {
    public:
        //! Dispatches an event
        /*! Invokes the registered delegates
         */
        void    DispatchEvent( Enum _eEvent );
    };

    /// Delegate invoker for 1 param
    template < class Enum, class Param1, class RetType = DelegateVoid >
    class Delegate1Invoker : public DelegateInvoker< Enum, Delegate1< Param1, RetType > >
    {
    public:
        //! Dispatches an event
        /*! Invokes the registered delegates
         */
        void    DispatchEvent( Enum _eEvent, Param1 _param1 );
    };

    /// Delegate invoker for 2 params
    template < class Enum, class Param1, class Param2, class RetType = DelegateVoid >
    class Delegate2Invoker : public DelegateInvoker< Enum, Delegate2< Param1, Param2, RetType > >
    {
    public:
        //! Dispatches an event
        /*! Invokes the registered delegates
         */
        void    DispatchEvent( Enum _eEvent, Param1 _param1, Param2 _param2 );
    };

    /// Delegate invoker for 3 params
    template < class Enum, class Param1, class Param2, class Param3, class RetType = DelegateVoid >
    class Delegate3Invoker : public DelegateInvoker< Enum, Delegate3< Param1, Param2, Param3, RetType > >
    {
    public:
        //! Dispatches an event
        /*! Invokes the registered delegates
         */
        void    DispatchEvent( Enum _eEvent, Param1 _param1, Param2 _param2, Param3 _param3 );
    };
}

#define DELEGATE0_INVOKER( _Enum ) void DispatchEvent( _Enum _eEvent ) { CoFiles::Delegate0Invoker< _Enum >::DispatchEvent( _eEvent ); } \
                                   void RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate0<> > _delegate ) { CoFiles::Delegate0Invoker< _Enum >::RegisterDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate0<> > _delegate ) { CoFiles::Delegate0Invoker< _Enum >::RemoveDelegate( _eEvent, _delegate ); }
#define DELEGATE1_INVOKER( _Enum, _Param1 ) void DispatchEvent( _Enum _eEvent, _Param1 _param1 ) { CoFiles::Delegate1Invoker< _Enum, _Param1 >::DispatchEvent( _eEvent, _param1 ); } \
                                   void RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate1< _Param1 > > _delegate ) { CoFiles::Delegate1Invoker< _Enum, _Param1 >::RegisterDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate1< _Param1 > > _delegate ) { CoFiles::Delegate1Invoker< _Enum, _Param1 >::RemoveDelegate( _eEvent, _delegate ); }
#define DELEGATE2_INVOKER( _Enum, _Param1, _Param2 ) void DispatchEvent( _Enum _eEvent, _Param1 _param1, _Param2 _param2 ) { CoFiles::Delegate2Invoker< _Enum, _Param1, _Param2 >::DispatchEvent( _eEvent, _param1, _param2 ); }

#include "cofiles/Events/DelegateInvoker.inl"

#endif
