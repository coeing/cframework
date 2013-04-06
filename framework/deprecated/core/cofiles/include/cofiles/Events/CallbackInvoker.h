#ifndef COFILES_CALLBACKINVOKER_H
#define COFILES_CALLBACKINVOKER_H

#include <map>
#include <vector>

#include "cofiles/RCObject.h"
#include "cofiles/Utils/SharedPtr.h"

namespace CoFiles
{
    /// Template class for a class which invokes callbacks
    /** Other classes can register and remove callbacks of
     *  a type T for an event E. When the event happens
     *  this class invokes the callbacks which are currently
     *  registered.
     */
    template< class T, class Event >
    class CallbackInvoker
    {
    public:
        /// Registers the passed callback for the passed event
        void        RegisterCallback( SharedPtr< T > _spCallback, Event _eEvent )
        {
            std::vector< SharedPtr< T > >& aspCallbacks = m_mapCallbacks[ _eEvent ];
            aspCallbacks.push_back( _spCallback );
        }

        /// Removes the passed callback for the passed event
        void        RemoveCallback( SharedPtr< T > _spCallback, Event _eEvent )
        {
        }

        /// Does the callback
        void        DoCallback( Event _eEvent, ... )
        {
            std::vector< SharedPtr< T > >& aspCallbacks = m_mapCallbacks[ _eEvent ];
            for ( std::vector< SharedPtr< T > >::iterator it = aspCallbacks.begin(); it != aspCallbacks.end(); ++it )
            {
                SharedPtr< T > spCallback = *it;
                spCallback->DoCallback( __VA_ARGS__ );
            }
        }

    private:
        /// Callbacks
        std::map< Event, std::vector< SharedPtr< T > > >    m_mapCallbacks;
    };

}

#endif
