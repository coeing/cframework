#include "ced/MainWindow.h"

#include "cfoundation/Debug/Logger.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/DragDrop/FreeDragController.h"
#include "cgui/DragDrop/FreeDropController.h"

#include "cgui/Gui/EditBox.h"
#include "cgui/Gui/SimpleBorder.h"
#include "cgui/Gui/Tree.h"
#include "cgui/Gui/TreeNode.h"

#include "cgui/Input/Input.h"

#include "ced/Input/InputTranslator.h"
#include "ced/Windows/FileDialog.h"
#include "ced/Windows/SettingsWindow.h"

using namespace Ced;

CFoundation::Logger gLogger( "ced.log" );

MainWindow::MainWindow()
:	m_spPropPanel( new PropertyPanel() ),
    m_pDoc( NULL )
{
    // Create delegates
    m_spDelegateClickedOpen = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &MainWindow::OnClickedOpen ) );
    m_spDelegateClickedSave = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &MainWindow::OnClickedSave ) );
    m_spDelegateClickedSettings = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &MainWindow::OnClickedSettings ) );
    m_spDelegateClickedCreateButton = CGui::ObjectInputEventDelegate1Ptr( new CGui::ObjectInputEventDelegate1( this, &MainWindow::OnClickedCreateButton ) );
    m_spDelegateClickedEditableObject = CGui::ObjectInputEventDelegate1Ptr( new CGui::ObjectInputEventDelegate1( this, &MainWindow::OnClickedEditableObject ) );
    m_spDelegateChildAdded = CGui::PanelEventDelegate2Ptr( new CGui::PanelEventDelegate2( this, &MainWindow::OnChildAdded ) );
    m_spDelegateChildRemoved = CGui::PanelEventDelegate2Ptr( new CGui::PanelEventDelegate2( this, &MainWindow::OnChildRemoved ) );
    m_spDelegateSelectedNode = CGui::TreeEventDelegate2Ptr( new CGui::TreeEventDelegate2( this, &MainWindow::OnSelectedTreeNode ) );
    m_spDelegateMovedNode = CGui::TreeEventDelegate2Ptr( new CGui::TreeEventDelegate2( this, &MainWindow::OnMovedTreeNode ) );
    m_spDelegateClosedFileDialog = CCtrl::WindowEventDelegatePtr( new CCtrl::WindowEventDelegate( this, &MainWindow::OnClosedFileDialog ) );
}

const CFoundation::String MainWindow::GetXMLFileName()
{
	return "data/screens/MainWindow.xml";
}

void MainWindow::InitWindow()
{
	m_pBtOpen					= GetButton( "BtOpen" );
    m_pBtOpen->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedOpen );
	m_pEbOpen					= GetEditBox( "EbOpen" );
	m_pBtSave					= GetButton( "BtSave" );
    m_pBtSave->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedSave );

    m_pBtProjectSettings        = GetButton( "BtProjectSettings" );
    m_pBtProjectSettings->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedSettings );

	m_pBtNew[ CTRL_BUTTON ]		= GetButton( "BtNewButton" );
    m_pBtNew[ CTRL_BUTTON ]->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedCreateButton );
	m_sCtrlName[ CTRL_BUTTON ]	= "button";
	m_pBtNew[ CTRL_IMAGE ]		= GetButton( "BtNewImage" );
    m_pBtNew[ CTRL_IMAGE ]->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedCreateButton );
	m_sCtrlName[ CTRL_IMAGE ]	= "image";
	m_pBtNew[ CTRL_LISTBOX ]	= GetButton( "BtNewListBox" );
    m_pBtNew[ CTRL_LISTBOX ]->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedCreateButton );
	m_sCtrlName[ CTRL_LISTBOX ]	= "listbox";
	m_pBtNew[ CTRL_CHECKBOX ]	= GetButton( "BtNewCheckBox" );
    m_pBtNew[ CTRL_CHECKBOX ]->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedCreateButton );
	m_sCtrlName[ CTRL_CHECKBOX ] = "checkbox";
	m_pBtNew[ CTRL_EDITBOX ]	= GetButton( "BtNewEditBox" );
    m_pBtNew[ CTRL_EDITBOX ]->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedCreateButton );
	m_sCtrlName[ CTRL_EDITBOX ] = "editbox";

	// Init Property Panel
    CGui::PanelPtr spPnlProperties = GetPanel( "PnlProperties" );
    m_spPropPanel->Init( spPnlProperties );

    // Get tree controls view
    m_spTrControls = GetTree( "TrControls" );
    if ( m_spTrControls != NULL )
    {
        m_spTrControls->RegisterDelegate( CGui::EVENT_TREE_SELECTED_NODE, m_spDelegateSelectedNode );
        m_spTrControls->RegisterDelegate( CGui::EVENT_TREE_MOVED_NODE, m_spDelegateMovedNode );
    }

	// Init Document
	m_spEditorWindow = GetWindow( "EditorPanel" );
	if( m_spEditorWindow != NULL )
	{
        CGui::DragDropSystem& clDragDropSystem = CGui::DragDropSystem::GetInstance();
        clDragDropSystem.SetDropController( m_spEditorWindow, CGui::DropControllerPtr( new CGui::FreeDropController() ) );

        // Register delegate
        m_spEditorWindow->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedEditableObject );

		m_pDoc = new Document( m_spEditorWindow );
	}

    // Create highlight border
    CGui::SimpleBorderPtr spHighlightBorder = CGui::GuiMain::GetInstance().CreateDefault< CGui::SimpleBorder >( "BorderHighlight" );
    m_spBorderHighlight = spHighlightBorder;

    CCtrl::WindowController::InitWindow();
}
        
void MainWindow::ProcessInput( const CSystem::InputState& _State )
{
    // Check if the current selected object should be deleted
    if ( m_spSelObject != NULL &&
         ( m_spSelObject == CGui::Input::GetInstance().GetFocusObject() ||
           m_spSelObject->IsDescendant( CGui::Input::GetInstance().GetFocusObject() ) ) &&
         InputTranslator::GetInstance().WasExecuted( INPUT_COMMAND_DELETE_SELECTED_OBJECT, _State ) )
    {
        OnDeleteSelectedObject();
    }
}

//void MainWindow::OnKeyPressed( const CGui::KeyboardEventMsg &clMessage )
//{
//	if( clMessage.GetTrigger() == CGui::KEY_DEL &&
//		m_spSelObject != NULL &&
//		m_spSelObject != m_spEditorWindow )
//	{
//	}
//}

void MainWindow::OnCloseScreen( const CCtrl::ScreenClosedMsg &clMessage )
{
}
       
void MainWindow::UpdateTreeView()
{
    if ( m_spTrControls == NULL )
    {
        return;
    }

    // Clear content
//    m_spTrControls->ClearContent();

    // Set root node
    m_spTrControls->GetRoot()->SetText( "Root" );
   // CGui::TreeNodePtr spRoot = CGui::GuiMain::GetInstance().CreateDefault< CGui::TreeNode >( "TrControlsRoot" );
    m_mapNodeObject[ 0 ] = m_spEditorWindow;
    m_mapObjectNode[ m_spEditorWindow ] = 0;

    // Add child nodes
    AddChildTreeNodes( m_spEditorWindow, m_spTrControls, 0 );
}
       
void MainWindow::AddChildTreeNodes( CGui::GuiObjectPtr _spObject, CGui::TreePtr _spTree, Unsigned32 _u32Parent )
{
    if ( _spObject == NULL ||
         _spTree == NULL )
    {
        return;
    }

    // Add childs
    CGui::PanelPtr spPanel = dynamic_pointer_cast< CGui::Panel >( _spObject );
    if ( spPanel != NULL )
    {
        Unsigned32 u32NumChildren = spPanel->GetNumChildren();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumChildren; ++u32Idx )
        {
            CGui::GuiObjectPtr spChild = spPanel->GetChild( u32Idx );
            if ( !spChild->IsIndependent() )
            {
                continue;
            }

            // Create tree node
            Unsigned32 u32Node = _spTree->CreateNode( _u32Parent );
            CGui::TreeNodePtr spNode = _spTree->GetNode( u32Node );
            if ( spNode != NULL )
            {
                m_mapNodeObject[ u32Node ] = spChild;
                m_mapObjectNode[ spChild ] = u32Node;

                const CFoundation::String& strName = spChild->GetName();
                if ( strName.IsEmpty() )
                {
                    spNode->SetText( "<Unnamed>" );
                }
                else
                {
                    spNode->SetText( strName );
                }
            }

            // Add child nodes
            AddChildTreeNodes( spChild, _spTree, u32Node );
        }
    }
}

void MainWindow::SetSelected( CGui::GuiObjectPtr pObject )
{
	m_spSelObject = pObject;

	if( m_spSelObject != NULL )
	{
		wprintf( L"Selected: %s\n", m_spSelObject->GetName().wc_str() );
	}
	else
	{
		std::cout << "Nothing selected: " << std::endl;
	}

    // Select node in tree
    m_spTrControls->SelectNode( m_mapObjectNode[ m_spSelObject ], true );

    m_spBorderHighlight->SetReference( m_spSelObject );

	// Set node to property panel
	m_spPropPanel->SetSelected( m_spSelObject );
}
       
void MainWindow::OnClickedOpen()
{
    // Open file dialog
    m_spOpenFileDialog = CreateWindow< FileDialog >( GetSharedPtr() );
    m_spOpenFileDialog->RegisterDelegate( CCtrl::EVENT_WINDOW_CLOSED, m_spDelegateClosedFileDialog );

    // Set last directory
    if ( m_spLastDirectory != NULL )
    {
        m_spOpenFileDialog->SetCurrentDirectory( m_spLastDirectory );
    }
}
       
void MainWindow::OnClickedSave()
{
	gLogger.Log( "Save" );

    // Check if document has a file assigned
    if ( m_pDoc->GetFile() != NULL )
    {
	    // Make all elements not draggable and resizable
	    MakeNonEditable( m_spEditorWindow, false );

        // Save
        m_pDoc->Save();

	    // Make all elements draggable and resizable again
	    MakeEditable( m_spEditorWindow, false );
    }
    else
    {
        // Open file dialog
        m_spFileDialogSave = CreateWindow< FileDialog >( GetSharedPtr() );
        m_spFileDialogSave->RegisterDelegate( CCtrl::EVENT_WINDOW_CLOSED, m_spDelegateClosedFileDialog );

        // Set last directory
        if ( m_spLastDirectory != NULL )
        {
            m_spFileDialogSave->SetCurrentDirectory( m_spLastDirectory );
        }
    }
}
       
void MainWindow::OnClickedSettings()
{
    // Open settings window
    CreateWindow< SettingsWindow >( GetSharedPtr() );
}
        
void MainWindow::OnClickedCreateButton( CGui::GuiObjectPtr _spObject )
{
	// Check which create button was clicked
	CGui::GuiObjectPtr pNewObject;
	for ( Unsigned32 u32Idx = 0; u32Idx != NUM_CTRLS; ++u32Idx )
	{
		if ( _spObject == m_pBtNew[ u32Idx ] )
		{
			std::cout << "NewCtrl " << m_sCtrlName[ u32Idx ] << std::endl;

            pNewObject = CGui::GuiMain::GetInstance().CreateDefault( m_sCtrlName[ u32Idx ], "NewCtrl" );
		}
	}

	if( pNewObject != NULL )
	{
        pNewObject->SetCustomSize( CFoundation::Vector2Du( 100, 100 ) );
		m_spEditorWindow->AddChildLast( pNewObject );

        MakeEditable( pNewObject, true );

        CGui::DragDropSystem& clDragDropSystem = CGui::DragDropSystem::GetInstance();

        clDragDropSystem.SetDragController( pNewObject, CGui::DragControllerPtr( new CGui::FreeDragController() ) );

		pNewObject->SetDraggable( true );
		pNewObject->SetResizable( true );
	}
}
        
void MainWindow::OnClickedEditableObject( CGui::GuiObjectPtr _spObject )
{
    if ( _spObject == m_spEditorWindow )
    {
        // Select none
        SetSelected( CGui::GuiObjectPtr() );
    }
    else
    {
        // Select
        SetSelected( _spObject );
    }
}
       
void MainWindow::OnChildAdded( CGui::PanelPtr _spPanel, CGui::GuiObjectPtr _spChild )
{
    // Create tree node
    Unsigned32 u32Node = m_spTrControls->CreateNode( m_mapObjectNode[ _spPanel ] );
    CGui::TreeNodePtr spNode = m_spTrControls->GetNode( u32Node );
    if ( spNode != NULL )
    {
        m_mapNodeObject[ u32Node ] = _spChild;
        m_mapObjectNode[ _spChild ] = u32Node;

        const CFoundation::String& strName = _spChild->GetName();
        if ( strName.IsEmpty() )
        {
            spNode->SetText( "<Unnamed>" );
        }
        else
        {
            spNode->SetText( strName );
        }
    }
}
        
void MainWindow::OnChildRemoved( CGui::PanelPtr _spPanel, CGui::GuiObjectPtr _spChild )
{
    // Remove tree node
    m_spTrControls->RemoveNode( m_mapObjectNode[ _spChild ] );
}

void MainWindow::OnSelectedTreeNode( Unsigned32 _u32Node, Integer64 _i64UserData )
{
    SetSelected( m_mapNodeObject[ _u32Node ] );
}
       
void MainWindow::OnMovedTreeNode( Unsigned32 _u32Node, Integer64 _i64UserData )
{
    CGui::GuiObjectPtr spObject = m_mapNodeObject[ _u32Node ];
    if ( spObject == NULL )
    {
        return;
    }

    // Get new parent node
    Unsigned32 u32ParentNode = m_spTrControls->GetParentNode( _u32Node );

    // Set new parent
    CGui::PanelPtr spParent = dynamic_pointer_cast< CGui::Panel >( m_mapNodeObject[ u32ParentNode ] );
    if ( spParent != NULL )
    {
        spParent->AddChildLast( spObject );
    }
}
        
void MainWindow::OnClosedFileDialog( CCtrl::WindowControllerPtr _spWindow, Unsigned32 _u32ReturnValue )
{
    if ( m_spOpenFileDialog == _spWindow )
    {
        OnClosedOpenDialog( _u32ReturnValue );
    }
    else if ( m_spFileDialogSave == _spWindow )
    {
        OnClosedSaveDialog( _u32ReturnValue );
    }
}
    
void MainWindow::OnClosedOpenDialog( Unsigned32 _u32ReturnValue )
{
    if ( _u32ReturnValue != WINDOW_RETURN_VALUE_OK )
    {
        return;
    }

    // Get 
    m_spLastDirectory = m_spOpenFileDialog->GetCurrentDirectory();

    SetSelected( CGui::GuiObjectPtr() );
	bool bSuccess = m_pDoc->Open( m_spOpenFileDialog->GetSelectedFile() );
	if( bSuccess )
	{
	    //m_spEditorWindow->AddListener( GetSharedPtr() );
	}
	else
	{
	    gLogger.Log( "Opening not successful" );
	}

    // Remove delegate
    m_spOpenFileDialog->RemoveDelegate( CCtrl::EVENT_WINDOW_CLOSED, m_spDelegateClosedFileDialog );

    // Free window
    m_spOpenFileDialog.reset();

	// Make all elements draggable and resizable
	MakeEditable( m_spEditorWindow, true );
	m_spEditorWindow->SetDraggable( false );

    // Update tree view
    UpdateTreeView();
}
    
void MainWindow::OnClosedSaveDialog( Unsigned32 _u32ReturnValue )
{
    if ( _u32ReturnValue != WINDOW_RETURN_VALUE_OK )
    {
        return;
    }

    // Get directory
    m_spLastDirectory = m_spFileDialogSave->GetCurrentDirectory();

	// Make all elements not draggable and resizable
	MakeNonEditable( m_spEditorWindow, false );

    // Save
    bool bSuccess = m_pDoc->SaveAs( m_spFileDialogSave->GetSelectedFile() );
	if ( bSuccess )
	{
	    //m_spEditorWindow->AddListener( GetSharedPtr() );
	}
	else
	{
	    gLogger.Log( "Save not successful" );
	}

	// Make all elements draggable and resizable again
	MakeEditable( m_spEditorWindow, false );

    // Remove delegate
    m_spFileDialogSave->RemoveDelegate( CCtrl::EVENT_WINDOW_CLOSED, m_spDelegateClosedFileDialog );

    // Free window
    m_spFileDialogSave.reset();
}
        
void MainWindow::OnDeleteSelectedObject()
{
    if ( m_spSelObject != NULL )
    {
		gLogger.Log( "Delete element " + m_spSelObject->GetName() );
        CGui::GuiMain::GetInstance().Delete( m_spSelObject );
        SetSelected( CGui::GuiObjectPtr() );
    }
}

void MainWindow::MakeEditable( CGui::GuiObjectPtr _spObject, bool _bCompletely )
{
	if( _spObject->IsIndependent() )
	{
        if ( _bCompletely )
        {
            CGui::DragDropSystem& clDragDropSystem = CGui::DragDropSystem::GetInstance();
            clDragDropSystem.SetDragController( _spObject, CGui::DragControllerPtr( new CGui::FreeDragController() ) );

            // Add delegates
            _spObject->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedEditableObject );
        }

		_spObject->SetDraggable( true );
		_spObject->SetResizable( true );
	}

	// Make childs draggable
    CGui::PanelPtr spPanel = dynamic_pointer_cast< CGui::Panel >( _spObject );
    if ( spPanel != NULL )
    {
        if ( _bCompletely )
        {
            // Add drop controller
            CGui::DragDropSystem& clDragDropSystem = CGui::DragDropSystem::GetInstance();
            clDragDropSystem.SetDropController( spPanel, CGui::DropControllerPtr( new CGui::FreeDropController() ) );

            // Add delegates
            spPanel->RegisterDelegate( CGui::EVENT_PANEL_CHILD_ADDED, m_spDelegateChildAdded );
            spPanel->RegisterDelegate( CGui::EVENT_PANEL_CHILD_REMOVED, m_spDelegateChildRemoved );
        }

	    CGui::ChildIterator clIterator = spPanel->GetChildIterator();
	    CGui::GuiObjectPtr spObject = clIterator.GetFirst();
	    while( !clIterator.IsDone() )
	    {
		    MakeEditable( spObject, _bCompletely );

		    spObject = clIterator.GetNext();
	    }
    }
}
        
void MainWindow::MakeNonEditable( CGui::GuiObjectPtr _spObject, bool _bRemoveCompletely )
{
	if ( _spObject->IsIndependent() )
	{
        if ( _bRemoveCompletely )
        {
            CGui::DragDropSystem& clDragDropSystem = CGui::DragDropSystem::GetInstance();
            clDragDropSystem.RemoveDragController( _spObject );

            // Remove delegates
            _spObject->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateClickedEditableObject );
        }

		_spObject->SetDraggable( false );
		_spObject->SetResizable( false );
	}

	// Make childs draggable
    CGui::PanelPtr spPanel = dynamic_pointer_cast< CGui::Panel >( _spObject );
    if ( spPanel != NULL )
    {
        if ( _bRemoveCompletely )
        {
            // Add drop controller
            CGui::DragDropSystem& clDragDropSystem = CGui::DragDropSystem::GetInstance();
            clDragDropSystem.RemoveDropController( spPanel );

            // Remove delegates
            spPanel->RemoveDelegate( CGui::EVENT_PANEL_CHILD_ADDED, m_spDelegateChildAdded );
            spPanel->RemoveDelegate( CGui::EVENT_PANEL_CHILD_REMOVED, m_spDelegateChildRemoved );
        }

	    CGui::ChildIterator clIterator = spPanel->GetChildIterator();
	    CGui::GuiObjectPtr spObject = clIterator.GetFirst();
	    while( !clIterator.IsDone() )
	    {
		    MakeNonEditable( spObject, _bRemoveCompletely );

		    spObject = clIterator.GetNext();
	    }
    }
}

