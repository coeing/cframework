#ifndef CED_DOCUMENT_H
#define CED_DOCUMENT_H

#include "csystem/Files/File.h"
#include "cgui/GUI/Window.h"

namespace Ced
{
	class Document
	{
	public:
        /// Constructor
		Document( CGui::WindowPtr _spWindow );

        /// Opens the passed file
        bool    Open( CSystem::FilePtr _spFile );

        /// Opens the passed file
		bool    Open( const CFoundation::String& _strFileName );

        /// Saves the document to its file
		bool    Save();

        /// Saves the document to the passed file
        bool    SaveAs( CSystem::FilePtr _spFile );

        /// Returns the file of the document
        CSystem::FilePtr    GetFile() const;

	private:
		void operator=( const Document& );

		/// Current file
        CSystem::FilePtr    m_spFile;

		/// Document window
		CGui::WindowPtr     m_spWindow;
	};
}

#endif
