#ifndef CSYSTEM_DIRECTORY_MANAGER_H
#define CSYSTEM_DIRECTORY_MANAGER_H

#include "cfoundation/Utils/SharedPtr.h"
#include "cfoundation/Utils/Singleton.h"

namespace CSystem
{
    class Directory;
    typedef SharedPtr< Directory > DirectoryPtr;

    /**
	 * \brief
	 * Write brief comment for DirectoryManager here.
	 * 
	 * Write detailed description for DirectoryManager here.
	 * 
	 * \remarks
	 * Write remarks for DirectoryManager here.
	 * 
	 * \see
	 * Separate items with the '|' character.
	 */
	class DirectoryManager : public CFoundation::Singleton< DirectoryManager >
    {
    public:
        /// Returns the current working directory
        DirectoryPtr    GetCurrentWorkingDirectory() const;

    private:
        friend class CFoundation::Singleton< DirectoryManager >;

        /// Constructor
        DirectoryManager();

        /// Current working directory
        DirectoryPtr    m_spCurrentWorkingDirectory;
    };
}

#endif
