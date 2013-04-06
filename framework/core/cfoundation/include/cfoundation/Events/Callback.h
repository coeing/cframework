#ifndef CFOUNDATION_CALLBACK_H
#define CFOUNDATION_CALLBACK_H

#include "cfoundation/Utils/RCObject.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    //! Base class of simple callbacks
    class CallbackBase
    {
    public:
        //! Callback
        virtual void    DoCallback() = 0;
    };

    typedef SharedPtr< CallbackBase > CallbackBasePtr;

    //! A simple callback
    template < class T >
    class Callback : public CallbackBase
    {
    public:
        typedef void (T::*CallbackFunction)();

        //! Constructor
        Callback( T* _pReceiver, CallbackFunction _pCallback )
        :   m_pReceiver( _pReceiver ),
            m_pCallback( _pCallback )
        {
        }

        //! Callback
        virtual void    DoCallback()
        {
            (m_pReceiver->*m_pCallback)();
        }
        
    private:
        //! Callback receiver
        T*                  m_pReceiver;

        //! Callback function
        CallbackFunction    m_pCallback;
    };

    //! Static simple callback
    class StaticCallback : public CallbackBase
    {
    public:
        typedef void (*CallbackFunction)();

        //! Constructor
        StaticCallback( CallbackFunction _pCallback )
        :   m_pCallback( _pCallback )
        {
        }

        //! Callback
        virtual void    DoCallback()
        {
            (*m_pCallback)();
        }
        
    private:
        //! Callback function
        CallbackFunction    m_pCallback;
    };
}

#endif
