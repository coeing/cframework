#ifndef CFOUNDATION_EVENTDISPATCHER_H
#define CFOUNDATION_EVENTDISPATCHER_H

#include <map>
#include <vector>

#include "cfoundation/Events/Callback.h"

namespace CFoundation
{
    //! Interface for objects which dispatch events
    template < class Enum >
    class EventDispatcher
    {
    public:
        //! Dispatches an event
        /*! Calls the registered callbacks
         */
        void    DispatchEvent( Enum _eEvent );

        //! Registers a callback for the passed event
        void    RegisterCallback( Enum _eEvent, CallbackBasePtr _spCallback );

        //! Removes a callback
        void    RemoveCallback( Enum _eEvent, CallbackBasePtr _spCallback );

    private:
        typedef std::vector< CallbackBasePtr > CallbackVector;
        typedef std::map< Enum, CallbackVector* > EventCallbacksMap;

        //! Returns the callbacks for the passed event
        CallbackVector*         GetCallbacks( Enum _eEvent )
        {
            EventCallbacksMap::iterator it = m_mapEventCallbacks.find( _eEvent );
            if ( it != m_mapEventCallbacks.end() )
            {
                return it->second;
            }
            else
            {
                return NULL;
            }
        }

        //! Callbacks
        EventCallbacksMap       m_mapEventCallbacks;
    };
}

#include "CFoundation/Events/EventDispatcher.inl"

#endif
