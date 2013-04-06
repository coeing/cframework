#include "cgui/Gui/Tree.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/XML/XMLElement.h"

#include "cgui/DragDrop/DragDropSystem.h"
#include "cgui/DragDrop/ProxyDragController.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/SizeMode.h"
#include "cgui/Gui/TreeNode.h"

using namespace CGui;

namespace CGui
{
    ///////////////////////////////////////////////////////////////////////////////
    ///
    /// TreeNodeDragController
    ///
    ///////////////////////////////////////////////////////////////////////////////
    class TreeNodeDragData : public DragData
    {
    public:
        /// Tree
        TreePtr                 m_spTree;

        /// Node index
        Unsigned32              m_u32Node;
    };

    INCLUDE_SHARED_CLASS( TreeNodeDragData );

    ///////////////////////////////////////////////////////////////////////////////
    ///
    /// TreeNodeDragController
    ///
    ///////////////////////////////////////////////////////////////////////////////
    class TreeNodeDragController : public ProxyDragController
    {
    public:
        /// Constructor
        TreeNodeDragController( TreePtr _spTree, Unsigned32 _u32Node );

        /// Returns the drag data of the current drag operation
        virtual DragDataPtr     GetDragData() const;

    protected:
        /// Creates the proxy gui object that's shown while dragging
        virtual GuiObjectPtr    CreateProxyObject( GuiObjectPtr _spObject ) const;

    private:
        /// Tree
        TreePtr                 m_spTree;

        /// Node index
        Unsigned32              m_u32Node;
    };

    INCLUDE_SHARED_CLASS( TreeNodeDragController );

    ///////////////////////////////////////////////////////////////////////////////
    ///
    /// TreeNodeDropController
    ///
    ///////////////////////////////////////////////////////////////////////////////
    class TreeNodeDropController : public DropController
    {
    public:
        /// Constructor
        TreeNodeDropController( TreePtr _spTree, Unsigned32 _u32Node );

        //! Executes a drop action
        /*! The drag object is dropped on the drop object at the passed position.
         *  Position is relative to the drop object.
         */
        virtual bool            Execute( GuiObjectPtr _spDragObject, DragDataPtr _spDragData, const CFoundation::Vector2Di& _vDragPosition, const CFoundation::Vector2Di& _vDropPosition );

    private:
        /// Tree
        TreePtr                 m_spTree;

        /// Node index
        Unsigned32              m_u32Node;
    };

    INCLUDE_SHARED_CLASS( TreeNodeDropController );
}

///////////////////////////////////////////////////////////////////////////////
///
/// Tree
///
///////////////////////////////////////////////////////////////////////////////

Tree::Tree( const CFoundation::String& _strName )
:	CombinedCtrl( _strName )
{
    // Create delegates
    m_spDelegateNodeCollapsed = TreeNodeEventDelegate1Ptr( new TreeNodeEventDelegate1( this, &Tree::OnNodeCollapsed ) );
    m_spDelegateNodeExpanded = TreeNodeEventDelegate1Ptr( new TreeNodeEventDelegate1( this, &Tree::OnNodeExpanded ) );
    m_spDelegateNodeClicked = TreeNodeEventDelegate1Ptr( new TreeNodeEventDelegate1( this, &Tree::OnNodeClicked ) );
    m_spDelegateNodeDoubleClicked = TreeNodeEventDelegate1Ptr( new TreeNodeEventDelegate1( this, &Tree::OnNodeDoubleClicked ) );
    m_spDelegateDragStarted = DragEventDelegate3Ptr( new DragEventDelegate3( this, &Tree::OnNodeDragStarted ) );
    m_spDelegateDragEnded = DragEventDelegate3Ptr( new DragEventDelegate3( this, &Tree::OnNodeDragEnded ) );
    m_spDelegateDragUndone = DragEventDelegate3Ptr( new DragEventDelegate3( this, &Tree::OnNodeDragUndone ) );
}

Tree::~Tree()
{
}

const CFoundation::String Tree::GetTypeID()
{
	return "tree";
}

const CFoundation::String Tree::GetTypeId() const
{
	return Tree::GetTypeID();
}
		
void Tree::Clear()
{
    CombinedCtrl::Clear();
}
        
void Tree::StartInit()
{
    CombinedCtrl::StartInit();
}        

void Tree::EndInit()
{
    // Update nodes
    UpdateNodes();

    CombinedCtrl::EndInit();
}
        
void Tree::SetNodeTemplate( PanelPtr _spPnlNodeTemplate )
{
    if ( _spPnlNodeTemplate == m_spPnlNodeTemplate )
    {
        return;
    }

    m_spPnlNodeTemplate = _spPnlNodeTemplate;

    // Update node panels
    Unsigned32 u32NumNodes = m_aNodeInfos.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumNodes; ++u32Idx )
    {
        NodeInfo& nodeInfo = m_aNodeInfos[ u32Idx ];

        // Remove old panel
        RemoveChild( nodeInfo.m_spPanel );
        if ( nodeInfo.m_spPanel != NULL )
        {
            nodeInfo.m_spPanel->SetDraggable( false );
            DragDropSystem::GetInstance().RemoveDragController( nodeInfo.m_spPanel );
        }

        // Create new panel
        PanelPtr spPanel = GuiMain::GetInstance().CreateFromTemplate< Panel >( CFoundation::String::Build( "TreeNode%d", u32Idx ), m_spPnlNodeTemplate );
        AddChildLast( spPanel );

        if ( spPanel != NULL )
        {
            spPanel->SetDraggable( true );
            DragDropSystem::GetInstance().SetDragController( spPanel, nodeInfo.m_spDragController );
            DragDropSystem::GetInstance().SetDropController( spPanel, nodeInfo.m_spDropController );
        }

        nodeInfo.m_spNode->Init( spPanel );
        nodeInfo.m_spPanel = spPanel;
    }
}
        
Unsigned32 Tree::CreateNode( Unsigned32 _u32Parent, Integer64 _i64UserData )
{
    // Get parent node
    NodeInfo* pParentNode = GetNodeInfo( _u32Parent );
    if ( pParentNode == NULL )
    {
        CF_WARN( "Tree::CreateNode: Invalid parent node %d.", _u32Parent );
        return MAX_U32;
    }

    // Create new node info
    Unsigned32 u32Index = m_aNodeInfos.size();
    NodeInfo* pNewNode = CreateNodeInfo( u32Index );
    pNewNode->m_spPanel->SetVisible( true );
    pNewNode->m_i64UserData = _i64UserData;

    // Add to parent node
    AddNodeToParent( u32Index, _u32Parent );

    // Connect child and parent
    pNewNode->m_u32ParentNode = _u32Parent;

    if ( !IsInitializing() )
    {
        // Update nodes
        UpdateNodes();
    }

    // Dispatch event
    DispatchEvent( EVENT_TREE_ADDED_NODE, u32Index, _i64UserData );

    return u32Index;
}
       
void Tree::MoveNode( Unsigned32 _u32Node, Unsigned32 _u32NewParent )
{
    // Check if same node
    if ( _u32Node == _u32NewParent )
    {
        return;
    }

    // Get node info
    NodeInfo* pNodeInfo = GetNodeInfo( _u32Node );
    if ( pNodeInfo == NULL )
    {
        return;
    }

    // Check if already under right parent
    if ( pNodeInfo->m_u32ParentNode == _u32NewParent )
    {
        return;
    }

    // Remove from parent node
    RemoveNodeFromParent( _u32Node );

    // Add to parent node
    AddNodeToParent( _u32Node, _u32NewParent );

    if ( !IsInitializing() )
    {
        // Update nodes
        UpdateNodes();
    }

    // Dispatch event
    DispatchEvent( EVENT_TREE_MOVED_NODE, _u32Node, pNodeInfo->m_i64UserData );
}
        
void Tree::RemoveNode( Unsigned32 _u32Node )
{
    // Get node info
    NodeInfo* pNodeInfo = GetNodeInfo( _u32Node );
    if ( pNodeInfo == NULL )
    {
        return;
    }

    // Remove from parent node
    RemoveNodeFromParent( _u32Node );

    // Add index to free nodes
    pNodeInfo->m_spPanel->SetVisible( false );
    //pNodeInfo->Reset();
    //m_au32FreeNodes.push_back( _u32Node );

    if ( !IsInitializing() )
    {
        // Update nodes
        UpdateNodes();
    }

    // Dispatch event
    DispatchEvent( EVENT_TREE_REMOVED_NODE, _u32Node, pNodeInfo->m_i64UserData );
}
       
Unsigned32 Tree::GetParentNode( Unsigned32 _u32Node ) const
{
    // Get node info
    const NodeInfo* pNodeInfo = GetNodeInfo( _u32Node );
    if ( pNodeInfo == NULL )
    {
        return MAX_U32;
    }
    return pNodeInfo->m_u32ParentNode;
}
       
void Tree::SetUserData( Unsigned32 _u32Node, Integer64 _i64UserData )
{
    // Get node info
    NodeInfo* pNodeInfo = GetNodeInfo( _u32Node );
    if ( pNodeInfo == NULL )
    {
        return;
    }
    pNodeInfo->m_i64UserData = _i64UserData;
}
        
Integer64 Tree::GetUserData( Unsigned32 _u32Node ) const
{
    // Get node info
    const NodeInfo* pNodeInfo = GetNodeInfo( _u32Node );
    if ( pNodeInfo == NULL )
    {
        return 0;
    }
    return pNodeInfo->m_i64UserData;
}

TreeNodePtr Tree::GetRoot() const
{
    return GetNode( 0 );
}
        
TreeNodePtr Tree::GetNode( Unsigned32 _u32Node ) const
{
    // Get node info
    const NodeInfo* pNodeInfo = GetNodeInfo( _u32Node );
    if ( pNodeInfo == NULL )
    {
        return TreeNodePtr();
    }
    else
    {
        return pNodeInfo->m_spNode;
    }
}
        
void Tree::SelectNode( Unsigned32 _u32Node, bool _bSelected )
{
    // Get node info
    const NodeInfo* pNodeInfo = GetNodeInfo( _u32Node );
    if ( pNodeInfo == NULL )
    {
        return;
    }

    // Check if already selected
    if ( pNodeInfo->m_spNode->IsSelected() == _bSelected )
    {
        return;
    }

    // Unselect other nodes
    Unsigned32 u32NumNodes = m_aNodeInfos.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumNodes; ++u32Idx )
    {
        if ( u32Idx != _u32Node )
        {
            SelectNode( u32Idx, false );
        }
    }

    // Select node
    pNodeInfo->m_spNode->SetSelected( _bSelected );

    // Dispatch event
    DispatchEvent( _bSelected ? EVENT_TREE_SELECTED_NODE : EVENT_TREE_UNSELECTED_NODE, _u32Node, pNodeInfo->m_i64UserData );
}
       
Tree::NodeInfo* Tree::GetNodeInfo( Unsigned32 _u32Idx )
{
    if ( _u32Idx < m_aNodeInfos.size() )
    {
        return &m_aNodeInfos[ _u32Idx ];
    }
    else
    {
        return NULL;
    }
    /*
    NodeInfoMap::const_iterator it = m_mapNodeInfos.find( _u32Idx );
    if ( it != m_mapNodeInfos.end() )
    {
        return &it->second;
    }
    else
    {
        return NULL;
    }*/
}
        
Tree::NodeInfo* Tree::CreateNodeInfo( Unsigned32& _u32Index )
{    
    _u32Index = m_aNodeInfos.size();

    // Create new panel
    PanelPtr spPanel = GuiMain::GetInstance().CreateFromTemplate< Panel >( CFoundation::String::Build( "TreeNode%d", _u32Index ), m_spPnlNodeTemplate );
    AddChildLast( spPanel );

    // Create new tree node
    TreeNodePtr spNode( new TreeNode( _u32Index ) );
    spNode->Init( spPanel );
    spNode->RegisterDelegate( EVENT_TREE_NODE_COLLAPSED, m_spDelegateNodeCollapsed );
    spNode->RegisterDelegate( EVENT_TREE_NODE_EXPANDED, m_spDelegateNodeExpanded );
    spNode->RegisterDelegate( EVENT_TREE_NODE_CLICKED, m_spDelegateNodeClicked );
    spNode->RegisterDelegate( EVENT_TREE_NODE_DOUBLE_CLICKED, m_spDelegateNodeDoubleClicked );

    // Add drag controller

    // Create drag controller
    TreeNodeDragControllerPtr spDragController = TreeNodeDragControllerPtr( new TreeNodeDragController( GetSharedPtr< Tree >(), _u32Index ) );
    spDragController->RegisterDelegate( EVENT_DRAG_STARTED, m_spDelegateDragStarted );
    spDragController->RegisterDelegate( EVENT_DRAG_ENDED, m_spDelegateDragEnded );
    spDragController->RegisterDelegate( EVENT_DRAG_UNDONE, m_spDelegateDragUndone );
    if ( spPanel != NULL )
    {
        spPanel->SetDraggable( true );
        DragDropSystem::GetInstance().SetDragController( spPanel, spDragController );
    }

    // Create drop controller
    TreeNodeDropControllerPtr spDropController = TreeNodeDropControllerPtr( new TreeNodeDropController( GetSharedPtr< Tree >(), _u32Index ) );
    if ( spPanel != NULL )
    {
        DragDropSystem::GetInstance().SetDropController( spPanel, spDropController );
    }

    NodeInfo newNode;
    newNode.m_spNode = spNode;
    newNode.m_spPanel = spPanel;
    newNode.m_spDragController = spDragController;
    newNode.m_spDropController = spDropController;
    
    // Add node
    m_aNodeInfos.push_back( newNode );

    return &m_aNodeInfos.back();
}
       
const Tree::NodeInfo* Tree::GetNodeInfo( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < m_aNodeInfos.size() )
    {
        return &m_aNodeInfos[ _u32Idx ];
    }
    else
    {
        return NULL;
    }
    /*
    NodeInfoMap::const_iterator it = m_mapNodeInfos.find( _u32Idx );
    if ( it != m_mapNodeInfos.end() )
    {
        return &it->second;
    }
    else
    {
        return NULL;
    }*/
}
        
void Tree::AddNodeToParent( Unsigned32 _u32Node, Unsigned32 _u32Parent )
{
    // Get node info
    NodeInfo* pNodeInfo = GetNodeInfo( _u32Node );
    if ( pNodeInfo == NULL )
    {
        return;
    }

    // Check if already parent
    if ( pNodeInfo->m_u32ParentNode == _u32Parent )
    {
        return;
    }

    pNodeInfo->m_u32ParentNode = _u32Parent;

    // Get parent node
    NodeInfo* pParentNode = GetNodeInfo( _u32Parent );
    if ( pParentNode == NULL )
    {
        return;
    }

    pParentNode->m_au32ChildNodes.push_back( _u32Node );

    // Set parent expandable
    pParentNode->m_spNode->SetExpandable( true );
}
        
void Tree::RemoveNodeFromParent( Unsigned32 _u32Node )
{
    // Get node info
    NodeInfo* pNodeInfo = GetNodeInfo( _u32Node );
    if ( pNodeInfo == NULL )
    {
        return;
    }

    // Get parent node
    NodeInfo* pParentNode = GetNodeInfo( pNodeInfo->m_u32ParentNode );
    if ( pParentNode == NULL )
    {
        return;
    }

    // Remove from children
    std::vector< Unsigned32 >::iterator it = std::find( pParentNode->m_au32ChildNodes.begin(), pParentNode->m_au32ChildNodes.end(), _u32Node );
    if ( it != pParentNode->m_au32ChildNodes.end() )
    {
        pParentNode->m_au32ChildNodes.erase( it );
    }

    // Set parent non-expandable, if no more chilren
    pParentNode->m_spNode->SetExpandable( pParentNode->m_au32ChildNodes.size() > 0 );
}
        
void Tree::UpdateNodes() const
{
    // Get root node
    const NodeInfo* pRootNode = GetNodeInfo( 0 );
    Unsigned32 u32OffsetY = 0;
    UpdateNode( pRootNode, true, 0, u32OffsetY );
}
      
void Tree::UpdateNode( const NodeInfo* _pNode, bool _bVisible, Unsigned32 _u32OffsetX, Unsigned32& _u32OffsetY ) const
{
    if ( _pNode == NULL )
    {
        return;
    }

    // Update node panel
    if ( _pNode->m_spPanel != NULL )
    {
        _pNode->m_spPanel->SetVisible( _bVisible );
        _pNode->m_spPanel->SetOffset( CFoundation::Vector2Di( _u32OffsetX, _u32OffsetY ) );

        // Add y offset
        if ( _bVisible )
        {
            _u32OffsetY += _pNode->m_spPanel->GetHeight();
        }
    }

    // Check if childs are visible
    bool bVisible = _bVisible && _pNode->m_spNode->IsExpanded();

    Unsigned32 u32NumChildNodes = _pNode->m_au32ChildNodes.size();
    Unsigned32 u32OffsetX = _u32OffsetX + 20;
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumChildNodes; ++u32Idx )
    {
        // Get child node
        const NodeInfo* pChildNode = GetNodeInfo( _pNode->m_au32ChildNodes[ u32Idx ] );
        if ( pChildNode != NULL )
        {
            // Update child node
            UpdateNode( pChildNode, bVisible, u32OffsetX, _u32OffsetY );
        }
    }
}
        
void Tree::OnNodeCollapsed( TreeNodePtr _spTreeNode )
{
    UpdateNodes();
}
        
void Tree::OnNodeExpanded( TreeNodePtr _spTreeNode )
{
    UpdateNodes();
}
        
void Tree::OnNodeClicked( TreeNodePtr _spTreeNode )
{
    SelectNode( _spTreeNode->m_u32Idx, true );
}
       
void Tree::OnNodeDoubleClicked( TreeNodePtr _spTreeNode )
{
    // Expand or collapse
    _spTreeNode->SetExpanded( !_spTreeNode->IsExpanded() );
}
        
void Tree::OnNodeDragStarted( DragControllerPtr, GuiObjectPtr, DragDataPtr )
{
}
       
void Tree::OnNodeDragEnded( DragControllerPtr, GuiObjectPtr, DragDataPtr )
{
}
        
void Tree::OnNodeDragUndone( DragControllerPtr, GuiObjectPtr, DragDataPtr )
{
}

///////////////////////////////////////////////////////////////////////////////
///
/// Tree::NodeInfo
///
///////////////////////////////////////////////////////////////////////////////

Tree::NodeInfo::NodeInfo()
:   m_u32ParentNode( MAX_U32 ),
    m_i64UserData( 0 )
{
}

///////////////////////////////////////////////////////////////////////////////
///
/// TreeCreator
///
///////////////////////////////////////////////////////////////////////////////

const CFoundation::String TreeCreator::GetTypeId() const
{
	return Tree::GetTypeID();
}

GuiObjectPtr TreeCreator::CreateObject( const CFoundation::String& _strName ) const
{
	TreePtr spTree( new Tree( _strName ) );
    
    // Create root node
    Unsigned32 u32Idx = 0;
    Tree::NodeInfo* pRootNode = spTree->CreateNodeInfo( u32Idx );
    pRootNode->m_u32ParentNode = MAX_U32;

	return spTree;
}

void TreeCreator::InitDefault( GuiObjectPtr _spObject ) const
{
	CombinedCtrlCreator::InitDefault( _spObject );

	TreePtr spTree = static_pointer_cast< Tree >( _spObject );
}

void TreeCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	CombinedCtrlCreator::InitFromXML( _spObject, _spXMLElement );

	TreePtr spTree = static_pointer_cast< Tree >( _spObject );

    // Init panel template
    CFoundation::XMLNodeCPtr spChildNode = _spXMLElement->GetFirstChild();
    while ( spChildNode != NULL )
    {
        if ( spChildNode->IsElement() )
        {
            CFoundation::XMLElementCPtr spChildElement = static_pointer_cast< const CFoundation::XMLElement >( spChildNode );
            const CFoundation::String& strName = spChildElement->GetName();
            if ( strName == "treenode" )
            {
                if ( spTree->m_spPnlNodeTemplate == NULL )
                {
                    PanelPtr spPanel = GuiMain::GetInstance().Create< Panel >( "PnlNodeTemplate" );
                    GuiMain::GetInstance().InitFromXML( spPanel, spChildElement );
                    spTree->SetNodeTemplate( spPanel );
                }
                else
                {
                    GuiMain::GetInstance().InitFromXML( spTree->m_spPnlNodeTemplate, spChildElement );
                }
            }
        }

        spChildNode = spChildNode->GetNextSibling();
    }
}

bool TreeCreator::SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const
{
    GuiObjectCreator::SaveToXML( _spObject, _spDoc, _spXMLElement );

    TreeCPtr spTree = static_pointer_cast< const Tree >( _spObject );

    return true;
}

///////////////////////////////////////////////////////////////////////////////
///
/// TreeNodeDragController
///
///////////////////////////////////////////////////////////////////////////////
TreeNodeDragController::TreeNodeDragController( TreePtr _spTree, Unsigned32 _u32Node )
:   m_spTree( _spTree ),
    m_u32Node( _u32Node )
{
}
        
DragDataPtr TreeNodeDragController::GetDragData() const
{
    TreeNodeDragDataPtr spDragData( new TreeNodeDragData() );
    spDragData->m_spTree = m_spTree;
    spDragData->m_u32Node = m_u32Node;
    return spDragData;
}

GuiObjectPtr TreeNodeDragController::CreateProxyObject( GuiObjectPtr _spObject ) const
{
    GuiObjectPtr spProxyObject = GuiMain::GetInstance().CreateFromTemplate( "ProxyObject", _spObject );
    spProxyObject->SetAlpha( 0.5f );
    return spProxyObject;
}

///////////////////////////////////////////////////////////////////////////////
///
/// TreeNodeDropController
///
///////////////////////////////////////////////////////////////////////////////  
TreeNodeDropController::TreeNodeDropController( TreePtr _spTree, Unsigned32 _u32Node )
:   m_spTree( _spTree ),
    m_u32Node( _u32Node )
{
}

bool TreeNodeDropController::Execute( GuiObjectPtr _spDragObject, DragDataPtr _spDragData, const CFoundation::Vector2Di& _vDragPosition, const CFoundation::Vector2Di& _vDropPosition )
{
    // Expect tree node drag data
    TreeNodeDragDataPtr spDragData = dynamic_pointer_cast< TreeNodeDragData >( _spDragData );
    if ( spDragData == NULL )
    {
        return false;
    }

    m_spTree->MoveNode( spDragData->m_u32Node, m_u32Node );

    return true;
}
