/**
 * <File comment goes here!!>
 * 
 * Copyright (c) 200x by <your name/ organization here>
 */
#ifndef CSYSTEM_CORE_H
#define CSYSTEM_CORE_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Utils/Singleton.h"

namespace CSystem
{
    /////////////////////////////////////////////////
    /// Core
    /////////////////////////////////////////////////

    /// Provides basic system functions
    ///

    /// @Author Christian Oeing
    /// @Date   23.01.10 

	class Core : public CFoundation::Singleton< Core >
	{
	public:
        /// Destructor
        ~Core();

        /// Pauses the execution of the process for the passed amount of milliseconds
        static void         Sleep( Unsigned64 _u64Milliseconds );

        /// Retrieves the number of milliseconds that have elapsed since the system was started
        static Unsigned64   GetTickCount();

	private:
		friend class CFoundation::Singleton< Core >;

        /// Constructor
		Core();
	};
};

#endif
