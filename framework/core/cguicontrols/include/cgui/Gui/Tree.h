#ifndef CGUI_TREE_H
#define CGUI_TREE_H

#include "cgui/Gui/CombinedCtrl.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( DragController );
    INCLUDE_SHARED_CLASS( DragData );
    INCLUDE_SHARED_CLASS( TreeNode );
    INCLUDE_SHARED_CLASS( TreeNodeDragController );
    INCLUDE_SHARED_CLASS( TreeNodeDropController );

    TYPEDEF_DELEGATE3( DragEvent, DragControllerPtr, GuiObjectPtr, DragDataPtr );
    TYPEDEF_DELEGATE1( TreeNodeEvent, TreeNodePtr );

    enum TreeEvent
    {
        EVENT_TREE_SELECTED_NODE,
        EVENT_TREE_UNSELECTED_NODE,
        EVENT_TREE_DOUBLE_CLICKED_NODE,
        EVENT_TREE_ADDED_NODE,
        EVENT_TREE_MOVED_NODE,
        EVENT_TREE_REMOVED_NODE,
    };
    TYPEDEF_DELEGATE2( TreeEvent, Unsigned32, Integer64 );

    class Tree : public CombinedCtrl, public CFoundation::Delegate2Invoker< TreeEvent, Unsigned32, Integer64 >
	{
	public:
        DELEGATE2_INVOKER( TreeEvent, Unsigned32, Integer64 );

        /// Destructor
		virtual ~Tree();

        /// Returns the type of this class
        static const CFoundation::String    GetTypeID();

        /// Returns the type of this object
		virtual const CFoundation::String   GetTypeId() const;

        /// Clears the object
		virtual void                        Clear();

        /// Starts initializing the object
        /** During initialization no messages are send and
         *  size, position, etc. are not updated
         */
        virtual void                        StartInit();

        /// Finishes initializing the object
        virtual void                        EndInit();

        /// Sets the node template
        void                                SetNodeTemplate( PanelPtr _spPnlNodeTemplate );

        /// Creates a new node
        /// Returns the index of the new node
        /// Returns MAX_U32 if parent index not found
        Unsigned32                          CreateNode( Unsigned32 _u32Parent, Integer64 _i64UserData = 0 );

        /// Moves the node to the passed parent
        void                                MoveNode( Unsigned32 _u32Node, Unsigned32 _u32NewParent );

        /// Removes the passed node
        void                                RemoveNode( Unsigned32 _u32Node );

        /// Returns the parent node of the passed node
        Unsigned32                          GetParentNode( Unsigned32 _u32Node ) const;

        /// Sets the user data for the passed node
        void                                SetUserData( Unsigned32 _u32Node, Integer64 _i64UserData );

        /// Returns the user data for the passed node
        Integer64                           GetUserData( Unsigned32 _u32Node ) const;

        /// Returns the root tree node
        TreeNodePtr                         GetRoot() const;

        /// Returns the node with the passed index
        TreeNodePtr                         GetNode( Unsigned32 _u32Node ) const;

        /// Selects/Unselects the passed node
        void                                SelectNode( Unsigned32 _u32Node, bool _bSelected );

	private:
		friend class TreeCreator;

        class NodeInfo
        {
        public:
            /// Constructor
            NodeInfo();

            /// Node
            TreeNodePtr                 m_spNode;

            /// Panel
            PanelPtr                    m_spPanel;

            /// Parent node index
            Unsigned32                  m_u32ParentNode;

            /// Child nodes
            std::vector< Unsigned32 >   m_au32ChildNodes;

            /// User data
            Integer64                   m_i64UserData;

            /// Drag controller
            TreeNodeDragControllerPtr   m_spDragController;

            /// Drop controlller
            TreeNodeDropControllerPtr   m_spDropController;
        };

        typedef std::map< Unsigned32, NodeInfo > NodeInfoMap;
        typedef std::vector< NodeInfo > NodeInfoVec;

        /// Constructor
		Tree( const CFoundation::String& _strName );

        /// Creates a new node info
        NodeInfo*                           CreateNodeInfo( Unsigned32& _u32Idx );

        /// Returns the node info with the passed index
        /// Returns NULL if invalid index
        NodeInfo*                           GetNodeInfo( Unsigned32 _u32Idx );
        const NodeInfo*                     GetNodeInfo( Unsigned32 _u32Idx ) const;

        /// Adds the passed node to a parent node
        void                                AddNodeToParent( Unsigned32 _u32Node, Unsigned32 _u32Parent );

        /// Removes the passed node from its parent
        void                                RemoveNodeFromParent( Unsigned32 _u32Node );

        /// Updates the nodes
        void                                UpdateNodes() const;

        /// Updates the node and all of its child nodes
        void                                UpdateNode( const NodeInfo* _pNode, bool _bVisible, Unsigned32 _u32OffsetX, Unsigned32& _u32OffsetY ) const;

        /// Callbacks
        void                                OnNodeCollapsed( TreeNodePtr _spTreeNode );
        void                                OnNodeExpanded( TreeNodePtr _spTreeNode );
        void                                OnNodeClicked( TreeNodePtr _spTreeNode );
        void                                OnNodeDoubleClicked( TreeNodePtr _spTreeNode );

        void                                OnNodeDragStarted( DragControllerPtr, GuiObjectPtr, DragDataPtr );
        void                                OnNodeDragEnded( DragControllerPtr, GuiObjectPtr, DragDataPtr );
        void                                OnNodeDragUndone( DragControllerPtr, GuiObjectPtr, DragDataPtr );
        
        /// Delegates
        TreeNodeEventDelegate1Ptr           m_spDelegateNodeCollapsed;
        TreeNodeEventDelegate1Ptr           m_spDelegateNodeExpanded;
        TreeNodeEventDelegate1Ptr           m_spDelegateNodeClicked;
        TreeNodeEventDelegate1Ptr           m_spDelegateNodeDoubleClicked;
        DragEventDelegate3Ptr               m_spDelegateDragStarted;
        DragEventDelegate3Ptr               m_spDelegateDragEnded;
        DragEventDelegate3Ptr               m_spDelegateDragUndone;

        /// Nodes
        //NodeInfoMap                         m_mapNodeInfos;
        NodeInfoVec                         m_aNodeInfos;

        /// Node template
        PanelPtr                            m_spPnlNodeTemplate;
	};

    /// Tree pointer
    INCLUDE_SHARED_CLASS( Tree );

    class TreeCreator : public CombinedCtrlCreator, public CFoundation::Singleton< TreeCreator >
	{
	public:
        /// Returns the type id of the creator and object
		virtual const CFoundation::String GetTypeId() const;

        /// Creates a new object with the passed name
		virtual GuiObjectPtr    CreateObject( const CFoundation::String& _strName ) const;

        /// Does the default initialization
		virtual void            InitDefault( GuiObjectPtr _spObject ) const;

        /// Initializes the passed object from a xml element
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

        /// Saves the passed object to a xml element
		virtual bool            SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const;
	};
}

#endif
