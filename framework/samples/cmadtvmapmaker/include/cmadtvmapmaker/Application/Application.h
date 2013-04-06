#ifndef CMADTV_APPLICATION_H
#define CMADTV_APPLICATION_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/Singleton.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Map );

    class Application : public CFoundation::Singleton< Application >
    {
    public:
        /// Constructor
        Application();

        /// Destructor
        ~Application();

        /// Initializes and starts the application
        void                                    Start( const CFoundation::String& _strFile );

        /// Exits the game
        virtual void                            Exit();

    protected:
        /// Initialization
		virtual bool                            Init();

        /// Deinitialization
		virtual void                            Deinit();

        /// Creates a map from the passed map image
        MapPtr                                  CreateDensityData( const CFoundation::String& _strFile );
    };
}

#endif
