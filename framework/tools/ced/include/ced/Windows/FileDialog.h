#ifndef CED_FILEDIALOG_H
#define CED_FILEDIALOG_H

#include "csystem/Files/Directory.h"
#include "cctrl/WindowController.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( ListBox );

    TYPEDEF_DELEGATE0( ListBoxEvent );
    TYPEDEF_DELEGATE2( ListBoxEventI32, ListBoxPtr, Integer32 );
}

namespace Ced
{
	class FileDialog : public CCtrl::WindowController
	{
	public:
        /// Constructor
        FileDialog();

        //! Returns the xml file name for this window
		static const CFoundation::String    GetXMLFileName();

        //! Initializes the window
		virtual void                        InitWindow();

        /// Sets the current directory
        void                                SetCurrentDirectory( CSystem::DirectoryPtr _spDirectory );

        /// Returns the current directory
        CSystem::DirectoryPtr               GetCurrentDirectory() const;

        /// Returns the current selected directory
        CSystem::DirectoryPtr               GetSelectedDirectory() const;

        /// Returns the current selected file
        CSystem::FilePtr                    GetSelectedFile() const;

    private:
        /// Updates the file listing
        void                            UpdateFileListing();

        /// Callbacks
        void                            OnSelectedRow( CGui::ListBoxPtr _spListBox, Integer32 _i32SelectedRow );

        /// Callback when a cell in the list box was double clicked
        void                            OnDoubleClickedCell();

        /// Callback when the ok button was activated
        void                            OnOk();

        /// Callback when the cancel button was activated
        void                            OnCancel();

        /// Delegates
        CGui::ListBoxEventI32Delegate2Ptr   m_spDelegateSelectedRow;

        /// Delegate when the ok button was clicked
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateClickedOk;

        /// Delegate when the cancel button was clicked
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateClickedCancel;

        /// Delegate when a cell in the list box was double clicked
        CGui::ListBoxEventDelegate0Ptr      m_spDelegateDoubleClickedCell;

        /// Current directory
        CSystem::DirectoryPtr           m_spCurDirectory;

        /// File List Box
        CGui::ListBoxPtr                m_spLbFiles;

        /// Ok button
        CGui::ButtonPtr                 m_spBtOk;

        /// Cancel button
        CGui::ButtonPtr                 m_spBtCancel;

        /// Edit field for file name
        CGui::EditBoxPtr                m_spEbFileName;

        /// Indicates if new file is allowed
        bool                            m_bAllowNewFile;
    };
}

#endif
