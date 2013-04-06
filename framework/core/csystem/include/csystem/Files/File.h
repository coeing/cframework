#ifndef CSYSTEM_FILE_H
#define CSYSTEM_FILE_H

#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CSystem
{
    class Directory;
    typedef SharedPtr< Directory > DirectoryPtr;
    typedef SharedPtr< const Directory > DirectoryCPtr;

    /**
	 * \brief
	 * Represents a file in a file system
	 * 
	 * This class represents a file in a file system.
	 * 
	 */
	class File
    {
    public:
        /// Constructor
        File( const CFoundation::String& _strPath );
            
        /// Constructor
        File( DirectoryCPtr _spDirectory, const CFoundation::String& _strFileName );

        /// Returns the directory the file is in
        DirectoryCPtr                       GetDirectory() const;

        /// Returns the name
        const CFoundation::String&          GetName() const;

        /// Returns the full name (directory + name)
        const CFoundation::String           GetFullName() const;

        /// Returns the extension of this file
        const CFoundation::String           GetExtension() const;

        /// Returns the directory name (without ending /) of the passed full name (directory + name)
        static const CFoundation::String    GetDirectoryName( const CFoundation::String& _strFullName );

        /// Returns the name of the passed full name (directory + name)
        /// _bExtension indicates if the extension should stay with the name (true) or be cut (false)
        static const CFoundation::String    GetFileName( const CFoundation::String& _strFullName, bool _bExtension = true );

        /// Returns the extension of the passed file name
        static const CFoundation::String    GetExtension( const CFoundation::String& _strFileName );

    private:
        /// Directory
        DirectoryCPtr           m_spDirectory;

        /// Name
        CFoundation::String     m_strName;
    };

    /// File pointer
    typedef SharedPtr< File > FilePtr;
}

#endif
