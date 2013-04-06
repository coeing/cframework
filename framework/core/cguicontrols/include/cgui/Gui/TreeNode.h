#ifndef CGUI_TREE_NODE_H
#define CGUI_TREE_NODE_H

#include "cgui/Gui/CombinedCtrl.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( Button );
    INCLUDE_SHARED_CLASS( Image );
    INCLUDE_SHARED_CLASS( TextBox );
    INCLUDE_SHARED_CLASS( TreeNode );

    enum TreeNodeEvent
    {
        EVENT_TREE_NODE_COLLAPSED,
        EVENT_TREE_NODE_EXPANDED,
        EVENT_TREE_NODE_CLICKED,
        EVENT_TREE_NODE_DOUBLE_CLICKED,
    };
    TYPEDEF_DELEGATE1( TreeNodeEvent, TreeNodePtr );

    class TreeNode : public CFoundation::SharedPtrBase< TreeNode >, public CFoundation::Delegate1Invoker< TreeNodeEvent, TreeNodePtr >
	{
	public:
        DELEGATE1_INVOKER( TreeNodeEvent, TreeNodePtr );

        /// Destructor
		virtual ~TreeNode();

        /// Initialization
        void                                Init( PanelPtr _spPanel );

        /// Sets the text
        void                                SetText( const CFoundation::String& _strText );

        /// Returns the text
        const CFoundation::String&          GetText() const;

        /// Sets the node expandable
        void                                SetExpandable( bool _bExpandable );

        /// Indicates if the node is expandable
        bool                                IsExpandable() const;

        /// Sets the node expanded
        void                                SetExpanded( bool _bExpanded );

        /// Indicates if the node is expanded
        bool                                IsExpanded() const;

        /// Sets the node selected
        void                                SetSelected( bool _bSelected );

        /// Indicates if the node is selected
        bool                                IsSelected() const;

	private:
        friend class Tree;

        /// Constructor
		TreeNode( Unsigned32 _u32Idx );

        /// Callbacks
        void                                OnActivatedButton();
        void                                OnDeactivatedButton();
        void                                OnClickedPanel();
        void                                OnDoubleClickedPanel();
        void                                OnDragStarted();
        void                                OnDragEnded();
        void                                OnDragUndone();

        /// Delegates
        StateEventDelegate0Ptr              m_spDelegateActivatedButton;
        StateEventDelegate0Ptr              m_spDelegateDeactivatedButton;
        ObjectInputEventDelegate0Ptr        m_spDelegateClickedPanel;
        ObjectInputEventDelegate0Ptr        m_spDelegateDoubleClickedPanel;

        /// Index in tree
        Unsigned32                          m_u32Idx;

        /// Panel
        PanelPtr                            m_spPanel;

        /// Button
        ButtonPtr                           m_spButton;

        /// Icon
        ImagePtr                            m_spIcon;

        /// Text box
        TextBoxPtr                          m_spTextBox;

        /// Indicates if the node is expandable
        bool                                m_bExpandable;

        /// Indicates if the node is expanded
        bool                                m_bExpanded;

        /// Indicates if the node is selected
        bool                                m_bSelected;
	};
}

#endif
