#ifndef CED_MAINWINDOW_H
#define CED_MAINWINDOW_H

#include "cctrl/WindowController.h"

#include "ced/PropertyPanel.h"
#include "ced/Document.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( Border );
    TYPEDEF_DELEGATE2( TreeEvent, Unsigned32, Integer64 );
}

namespace Ced
{
    class FileDialog;
    typedef SharedPtr< FileDialog > FileDialogPtr;

	class MainWindow : public CCtrl::WindowController
	{
	public:
		MainWindow();

		static const CFoundation::String GetXMLFileName();

		virtual void InitWindow();

        /// Processes input (when window is active)
        virtual void    ProcessInput( const CSystem::InputState& _State );

		virtual void    OnCloseScreen( const CCtrl::ScreenClosedMsg &clMessage );

	private:
		void    SetSelected( CGui::GuiObjectPtr _spObject );

        /// Updates the tree view
        void            UpdateTreeView();

        /// Adds the childs of the passed element as tree node to the passed tree node
        void            AddChildTreeNodes( CGui::GuiObjectPtr _spObject, CGui::TreePtr _spTree, Unsigned32 _u32Parent );

        /// Called when the open button was clicked
        void    OnClickedOpen();

        /// Called when the save button was clicked
        void    OnClickedSave();

        /// Called when the settings button was clicked
        void    OnClickedSettings();

        /// Called when a create button was clicked
        void    OnClickedCreateButton( CGui::GuiObjectPtr _spObject );

        /// Called when editable object was clicked
        void    OnClickedEditableObject( CGui::GuiObjectPtr _spObject );

        /// Callbacks
        void    OnChildAdded( CGui::PanelPtr _spPanel, CGui::GuiObjectPtr _spChild );
        void    OnChildRemoved( CGui::PanelPtr _spPanel, CGui::GuiObjectPtr _spChild );
        void    OnSelectedTreeNode( Unsigned32 _u32Node, Integer64 _i64UserData );
        void    OnMovedTreeNode( Unsigned32 _u32Node, Integer64 _i64UserData );

        /// Called when the file dialog was closed
        void    OnClosedFileDialog( CCtrl::WindowControllerPtr _spWindow, Unsigned32 _u32ReturnValue );
        void    OnClosedOpenDialog( Unsigned32 _u32ReturnValue );
        void    OnClosedSaveDialog( Unsigned32 _u32ReturnValue );

        /// Called when the current selected object should be deleted
        void    OnDeleteSelectedObject();

        /// Makes the passed object editable
		void    MakeEditable( CGui::GuiObjectPtr _spObject, bool _bCompletely );

        /// Makes the passed object non-editable
        void    MakeNonEditable( CGui::GuiObjectPtr _spObject, bool _bCompletely );

        /// Delegates
        CGui::ObjectInputEventDelegate0Ptr      m_spDelegateClickedOpen;
        CGui::ObjectInputEventDelegate0Ptr      m_spDelegateClickedSave;
        CGui::ObjectInputEventDelegate0Ptr      m_spDelegateClickedSettings;
        CGui::ObjectInputEventDelegate1Ptr      m_spDelegateClickedCreateButton;
        CGui::ObjectInputEventDelegate1Ptr      m_spDelegateClickedEditableObject;
        CGui::PanelEventDelegate2Ptr            m_spDelegateChildAdded;
        CGui::PanelEventDelegate2Ptr            m_spDelegateChildRemoved;
        CGui::TreeEventDelegate2Ptr             m_spDelegateSelectedNode;
        CGui::TreeEventDelegate2Ptr             m_spDelegateMovedNode;
        CCtrl::WindowEventDelegatePtr           m_spDelegateClosedFileDialog;

        /// Open file dialog
        FileDialogPtr                   m_spOpenFileDialog;

        /// Save file dialog
        FileDialogPtr                   m_spFileDialogSave;

        /// Last directory in file dialog
        CSystem::DirectoryPtr           m_spLastDirectory;

		// Data
		CGui::WindowPtr                 m_spEditorWindow;
		SharedPtr< PropertyPanel >  m_spPropPanel;
		Document		*m_pDoc;

		CGui::GuiObjectPtr  m_spSelObject;

        /// Highlight border
        CGui::BorderPtr                 m_spBorderHighlight;

		// Controls
        
        //! Button to open the project settings
        CGui::ButtonPtr     m_pBtProjectSettings;

		enum CtrlType
		{
			CTRL_BUTTON,
			CTRL_IMAGE,
			CTRL_LISTBOX,
			CTRL_CHECKBOX,
			CTRL_EDITBOX,
			NUM_CTRLS
		};

        CGui::ButtonPtr     m_pBtNew[ NUM_CTRLS ];
		CFoundation::String m_sCtrlName[ NUM_CTRLS ];

		CGui::ButtonPtr     m_pBtOpen;
		CGui::EditBoxPtr    m_pEbOpen;

		CGui::ButtonPtr     m_pBtSave;

        /// Control tree view
        CGui::TreePtr       m_spTrControls;

        /// Node/Object map
        std::map< Unsigned32, CGui::GuiObjectPtr >  m_mapNodeObject;
        std::map< CGui::GuiObjectPtr, Unsigned32 >  m_mapObjectNode;
	};
}

#endif
