#ifndef CSYSTEM_DIRECTORY_H
#define CSYSTEM_DIRECTORY_H

#include <vector>

#include "cfoundation/Utils/SharedPtr.h"
#include "cfoundation/Text/String.h"

namespace CSystem
{
    class Directory;
    class File;
    typedef SharedPtr< File > FilePtr;

    /// Directory pointer
    typedef SharedPtr< Directory > DirectoryPtr;
    typedef SharedPtr< const Directory > DirectoryCPtr;

    class Directory : public CFoundation::SharedPtrBase< Directory >
    {
    public:
        /// Constructor
        Directory( const CFoundation::String& _strPath );

        /// Constructor
        Directory( DirectoryPtr _spParentDirectory, const CFoundation::String& _strName );

        /// Returns the parent directory
        DirectoryPtr                        GetParentDirectory() const;

        /// Returns the files in this directory
        const std::vector< FilePtr >&       GetFiles() const;

        /// Returns the directories in this directory
        const std::vector< DirectoryPtr >&  GetSubDirectories() const;

        /// Returns the name of the directory
        const CFoundation::String           GetName() const;

        /// Returns the path to this directory
        const CFoundation::String&          GetPath() const;

    private:
        /// Initializes the files and sub directories
        void                                Init() const;

        /// Indicates if it's initialized
        mutable bool                        m_bInitialized;

        /// Path
        CFoundation::String                 m_strPath;

        /// Files
        mutable std::vector< FilePtr >      m_aspFiles;

        /// Sub directories
        mutable std::vector< DirectoryPtr > m_aspSubDirectories;
    };
}

#endif
