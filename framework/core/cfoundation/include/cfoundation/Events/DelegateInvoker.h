#ifndef CFOUNDATION_DELEGATE_INVOKER_H
#define CFOUNDATION_DELEGATE_INVOKER_H

#include <map>
#include <vector>

#include "cfoundation/Events/Delegate.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    template < class Enum, class Delegate >
    class DelegateInvoker;

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// DelegateInvokerConnection
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Returned when registering a delegate on a delegate invoker
    /// Could be used to disconnect the delegate

    template < class Enum, class Delegate >
    class DelegateInvokerConnection : public DelegateConnection
    {
    public:
        /// Constructor
        DelegateInvokerConnection( DelegateInvoker< Enum, Delegate >* _pInvoker, Enum _eEvent, SharedPtr< Delegate > _spDelegate );

        /// Destructor
        ~DelegateInvokerConnection();

        /// Disconnects the delegate from the invoker
        virtual void        Disconnect();

    private:
        /// Invoker
        DelegateInvoker< Enum, Delegate >*              m_pInvoker;

        /// Event
        Enum                                            m_eEvent;

        /// Delegate
        SharedPtr< Delegate >                           m_spDelegate;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// DelegateInvoker
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// Base class for all delegate invokers

    template < class Enum, class Delegate >
    class DelegateInvoker
    {
    public:
        /// Destructor
        virtual ~DelegateInvoker();

        /// Registers a delegate for the passed event
        SharedPtr< DelegateInvokerConnection< Enum, Delegate > >    RegisterDelegate( Enum _eEvent, SharedPtr< Delegate > _spDelegate );

        /// Removes a delegate
        void    RemoveDelegate( Enum _eEvent, SharedPtr< Delegate > _spDelegate );

    protected:
        typedef std::vector< SharedPtr< Delegate > > DelegateVec;

        /// Returns the delegates for the passed event
        DelegateVec*            GetDelegates( Enum _eEvent );

    private:
        typedef std::map< Enum, DelegateVec* > EventDelegatesMap;

        /// Delegates
        EventDelegatesMap       m_mapEventDelegates;
    };

    /// Delegate invoker for 0 params
    template < class Enum, class RetType = DelegateVoid >
    class Delegate0Invoker : public DelegateInvoker< Enum, Delegate0< RetType > >
    {
    public:
        /// Dispatches an event
        /*! Invokes the registered delegates
         */
        void    DispatchEvent( Enum _eEvent );
    };

    /// Delegate invoker for 1 param
    template < class Enum, class Param1, class RetType = DelegateVoid >
    class Delegate1Invoker : public DelegateInvoker< Enum, Delegate1< Param1, RetType > >, 
                             public DelegateInvoker< Enum, Delegate0< RetType > >
    {
    public:
        /// Dispatches an event
        /*! Invokes the registered delegates
         */
        void    DispatchEvent( Enum _eEvent, Param1 _param1 );

    private:
        typedef DelegateInvoker< Enum, Delegate1< Param1, RetType > > Delegate1Base;
        typedef DelegateInvoker< Enum, Delegate0< RetType > > Delegate0Base;
    };

    /// Delegate invoker for 2 params
    template < class Enum, class Param1, class Param2, class RetType = DelegateVoid >
    class Delegate2Invoker : public DelegateInvoker< Enum, Delegate2< Param1, Param2, RetType > >,
                             public DelegateInvoker< Enum, Delegate1< Param1, RetType > >, 
                             public DelegateInvoker< Enum, Delegate0< RetType > >
    {
    public:
        /// Dispatches an event
        /*! Invokes the registered delegates
         */
        void    DispatchEvent( Enum _eEvent, Param1 _param1, Param2 _param2 );

    private:
        typedef DelegateInvoker< Enum, Delegate2< Param1, Param2, RetType > >   Delegate2Base;
        typedef DelegateInvoker< Enum, Delegate1< Param1, RetType > >           Delegate1Base;
        typedef DelegateInvoker< Enum, Delegate0< RetType > >                   Delegate0Base;
    };

    /// Delegate invoker for 3 params
    template < class Enum, class Param1, class Param2, class Param3, class RetType = DelegateVoid >
    class Delegate3Invoker : public DelegateInvoker< Enum, Delegate3< Param1, Param2, Param3, RetType > >,
                             public DelegateInvoker< Enum, Delegate2< Param1, Param2, RetType > >,
                             public DelegateInvoker< Enum, Delegate1< Param1, RetType > >, 
                             public DelegateInvoker< Enum, Delegate0< RetType > >
    {
    public:
        /// Dispatches an event
        /*! Invokes the registered delegates
         */
        void    DispatchEvent( Enum _eEvent, Param1 _param1, Param2 _param2, Param3 _param3 );

    private:
        typedef DelegateInvoker< Enum, Delegate3< Param1, Param2, Param3, RetType > >   Delegate3Base;
        typedef DelegateInvoker< Enum, Delegate2< Param1, Param2, RetType > >           Delegate2Base;
        typedef DelegateInvoker< Enum, Delegate1< Param1, RetType > >                   Delegate1Base;
        typedef DelegateInvoker< Enum, Delegate0< RetType > >                           Delegate0Base;
    };

    /// Delegate invoker for 4 params
    template < class Enum, class Param1, class Param2, class Param3, class Param4, class RetType = DelegateVoid >
    class Delegate4Invoker : public DelegateInvoker< Enum, Delegate4< Param1, Param2, Param3, Param4, RetType > >,
                             public DelegateInvoker< Enum, Delegate3< Param1, Param2, Param3, RetType > >,
                             public DelegateInvoker< Enum, Delegate2< Param1, Param2, RetType > >,
                             public DelegateInvoker< Enum, Delegate1< Param1, RetType > >, 
                             public DelegateInvoker< Enum, Delegate0< RetType > >
    {
    public:
        /// Dispatches an event
        /*! Invokes the registered delegates
         */
        void    DispatchEvent( Enum _eEvent, Param1 _param1, Param2 _param2, Param3 _param3, Param4 _param4 );

    private:
        typedef DelegateInvoker< Enum, Delegate4< Param1, Param2, Param3, Param4, RetType > >   Delegate4Base;
        typedef DelegateInvoker< Enum, Delegate3< Param1, Param2, Param3, RetType > >           Delegate3Base;
        typedef DelegateInvoker< Enum, Delegate2< Param1, Param2, RetType > >                   Delegate2Base;
        typedef DelegateInvoker< Enum, Delegate1< Param1, RetType > >                           Delegate1Base;
        typedef DelegateInvoker< Enum, Delegate0< RetType > >                                   Delegate0Base;
    };

    INCLUDE_SHARED_CLASS( DelegateConnection );
}

#define DELEGATE0_INVOKER( _Enum ) void DispatchEvent( _Enum _eEvent ) { CFoundation::Delegate0Invoker< _Enum >::DispatchEvent( _eEvent ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate0<> > _delegate )                                       { return Delegate0Invoker< _Enum >::RegisterDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate0<> > _delegate ) { CFoundation::Delegate0Invoker< _Enum >::RemoveDelegate( _eEvent, _delegate ); }
#define DELEGATE1_INVOKER( _Enum, _Param1 ) void DispatchEvent( _Enum _eEvent, _Param1 _param1 ) { CFoundation::Delegate1Invoker< _Enum, _Param1 >::DispatchEvent( _eEvent, _param1 ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate1< _Param1 > > _delegate )                              { return DelegateInvoker< _Enum, Delegate1< _Param1 > >::RegisterDelegate( _eEvent, _delegate ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate0<> > _delegate )                                       { return DelegateInvoker< _Enum, Delegate0<> >::RegisterDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate1< _Param1 > > _delegate ) { CFoundation::DelegateInvoker< _Enum, Delegate1< _Param1 > >::RemoveDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate0<> > _delegate ) { CFoundation::DelegateInvoker< _Enum, Delegate0<> >::RemoveDelegate( _eEvent, _delegate ); }
#define DELEGATE2_INVOKER( _Enum, _Param1, _Param2 ) void DispatchEvent( _Enum _eEvent, _Param1 _param1, _Param2 _param2 ) { CFoundation::Delegate2Invoker< _Enum, _Param1, _Param2 >::DispatchEvent( _eEvent, _param1, _param2 ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate2< _Param1, _Param2 > > _delegate )                     { return DelegateInvoker< _Enum, Delegate2< _Param1, _Param2 > >::RegisterDelegate( _eEvent, _delegate ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate1< _Param1 > > _delegate )                              { return DelegateInvoker< _Enum, Delegate1< _Param1 > >::RegisterDelegate( _eEvent, _delegate ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate0<> > _delegate )                                       { return DelegateInvoker< _Enum, Delegate0<> >::RegisterDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate2< _Param1, _Param2 > > _delegate ) { CFoundation::DelegateInvoker< _Enum, Delegate2< _Param1, _Param2 > >::RemoveDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate1< _Param1 > > _delegate ) { CFoundation::DelegateInvoker< _Enum, Delegate1< _Param1 > >::RemoveDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate0<> > _delegate ) { CFoundation::DelegateInvoker< _Enum, Delegate0<> >::RemoveDelegate( _eEvent, _delegate ); }
#define DELEGATE3_INVOKER( _Enum, _Param1, _Param2, _Param3 ) void DispatchEvent( _Enum _eEvent, _Param1 _param1, _Param2 _param2, _Param3 _param3 ) { CFoundation::Delegate3Invoker< _Enum, _Param1, _Param2, _Param3 >::DispatchEvent( _eEvent, _param1, _param2, _param3 ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate3< _Param1, _Param2, _Param3 > > _delegate )            { return DelegateInvoker< _Enum, Delegate3< _Param1, _Param2, _Param3 > >::RegisterDelegate( _eEvent, _delegate ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate2< _Param1, _Param2 > > _delegate )                     { return DelegateInvoker< _Enum, Delegate2< _Param1, _Param2 > >::RegisterDelegate( _eEvent, _delegate ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate1< _Param1 > > _delegate )                              { return DelegateInvoker< _Enum, Delegate1< _Param1 > >::RegisterDelegate( _eEvent, _delegate ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate0<> > _delegate )                                       { return DelegateInvoker< _Enum, Delegate0<> >::RegisterDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate3< _Param1, _Param2, _Param3 > > _delegate )      { CFoundation::DelegateInvoker< _Enum, Delegate3< _Param1, _Param2, _Param3 > >::RemoveDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate2< _Param1, _Param2 > > _delegate )               { CFoundation::DelegateInvoker< _Enum, Delegate2< _Param1, _Param2 > >::RemoveDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate1< _Param1 > > _delegate )                        { CFoundation::DelegateInvoker< _Enum, Delegate1< _Param1 > >::RemoveDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate0<> > _delegate )                                 { CFoundation::DelegateInvoker< _Enum, Delegate0<> >::RemoveDelegate( _eEvent, _delegate ); }
#define DELEGATE4_INVOKER( _Enum, _Param1, _Param2, _Param3, _Param4 ) \
                                   void DispatchEvent( _Enum _eEvent, _Param1 _param1, _Param2 _param2, _Param3 _param3, _Param4 _param4 )          { Delegate4Invoker< _Enum, _Param1, _Param2, _Param3, _Param4 >::DispatchEvent( _eEvent, _param1, _param2, _param3, _param4 ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate4< _Param1, _Param2, _Param3, _Param4 > > _delegate )   { return DelegateInvoker< _Enum, Delegate4< _Param1, _Param2, _Param3, _Param4 > >::RegisterDelegate( _eEvent, _delegate ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate3< _Param1, _Param2, _Param3 > > _delegate )            { return DelegateInvoker< _Enum, Delegate3< _Param1, _Param2, _Param3 > >::RegisterDelegate( _eEvent, _delegate ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate2< _Param1, _Param2 > > _delegate )                     { return DelegateInvoker< _Enum, Delegate2< _Param1, _Param2 > >::RegisterDelegate( _eEvent, _delegate ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate1< _Param1 > > _delegate )                              { return DelegateInvoker< _Enum, Delegate1< _Param1 > >::RegisterDelegate( _eEvent, _delegate ); } \
                                   CFoundation::DelegateConnectionPtr   RegisterDelegate( _Enum _eEvent, SharedPtr< Delegate0<> > _delegate )                                       { return DelegateInvoker< _Enum, Delegate0<> >::RegisterDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate4< _Param1, _Param2, _Param3, _Param4 > > _delegate )     { DelegateInvoker< _Enum, Delegate4< _Param1, _Param2, _Param3, _Param4 > >::RemoveDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate3< _Param1, _Param2, _Param3 > > _delegate )              { DelegateInvoker< _Enum, Delegate3< _Param1, _Param2, _Param3 > >::RemoveDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate2< _Param1, _Param2 > > _delegate )                       { DelegateInvoker< _Enum, Delegate2< _Param1, _Param2 > >::RemoveDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate1< _Param1 > > _delegate )                                { DelegateInvoker< _Enum, Delegate1< _Param1 > >::RemoveDelegate( _eEvent, _delegate ); } \
                                   void RemoveDelegate( _Enum _eEvent, SharedPtr< Delegate0<> > _delegate )                                         { DelegateInvoker< _Enum, Delegate0<> >::RemoveDelegate( _eEvent, _delegate ); }

#include "cfoundation/Events/DelegateInvoker.inl"

#endif
