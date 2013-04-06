#include "cgui/Gui/TreeNode.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/Image.h"
#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/SizeMode.h"
#include "cgui/Gui/TextBox.h"

using namespace CGui;

///////////////////////////////////////////////////////////////////////////////
///
/// TreeNode
///
///////////////////////////////////////////////////////////////////////////////

TreeNode::TreeNode( Unsigned32 _u32Idx )
:   m_u32Idx( _u32Idx ),
    m_bExpandable( false ),
    m_bExpanded( false ),
    m_bSelected( false )
{
    // Create delegates
    m_spDelegateActivatedButton = StateEventDelegate0Ptr( new StateEventDelegate0( this, &TreeNode::OnActivatedButton ) );
    m_spDelegateDeactivatedButton = StateEventDelegate0Ptr( new StateEventDelegate0( this, &TreeNode::OnDeactivatedButton ) );
    m_spDelegateClickedPanel = ObjectInputEventDelegate0Ptr( new ObjectInputEventDelegate0( this, &TreeNode::OnClickedPanel ) );
    m_spDelegateDoubleClickedPanel = ObjectInputEventDelegate0Ptr( new ObjectInputEventDelegate0( this, &TreeNode::OnDoubleClickedPanel ) );
}

TreeNode::~TreeNode()
{
}
        
void TreeNode::Init( PanelPtr _spPanel )
{
    m_spPanel = _spPanel;

    if ( m_spPanel != NULL )
    {
        m_spButton = dynamic_pointer_cast< Button >( m_spPanel->FindChild( "theButton" ) );
        if ( m_spButton != NULL )
        {
            m_spButton->SetVisible( m_bExpandable );

            m_spButton->RegisterDelegate( STATE_EVENT_ACTIVATED, m_spDelegateActivatedButton );
            m_spButton->RegisterDelegate( STATE_EVENT_DEACTIVATED, m_spDelegateDeactivatedButton );
        }
        m_spTextBox = dynamic_pointer_cast< TextBox >( m_spPanel->FindChild( "theTextBox" ) );
        m_spIcon = dynamic_pointer_cast< Image >( m_spPanel->FindChild( "theIcon" ) );
        

        // Register delegates
        m_spPanel->RegisterDelegate( INPUT_EVENT_PRESSED, m_spDelegateClickedPanel );
        m_spPanel->RegisterDelegate( INPUT_EVENT_DOUBLE_CLICKED, m_spDelegateDoubleClickedPanel );
        if ( m_spTextBox != NULL )
        {
            m_spTextBox->RegisterDelegate( INPUT_EVENT_PRESSED, m_spDelegateClickedPanel );
            m_spTextBox->RegisterDelegate( INPUT_EVENT_DOUBLE_CLICKED, m_spDelegateDoubleClickedPanel );
        }
        if ( m_spIcon != NULL )
        {
            m_spIcon->RegisterDelegate( INPUT_EVENT_PRESSED, m_spDelegateClickedPanel );
            m_spIcon->RegisterDelegate( INPUT_EVENT_DOUBLE_CLICKED, m_spDelegateDoubleClickedPanel );
        }
    }
}
        
void TreeNode::SetText( const CFoundation::String& _strText )
{
    SAFE_CALL( m_spTextBox )->SetText( _strText );
}

const CFoundation::String& TreeNode::GetText() const
{
    if ( m_spTextBox != NULL )
    {
        return m_spTextBox->GetText();
    }
    else
    {
        static CFoundation::String strTmp;
        return strTmp;
    }
}
        
void TreeNode::SetExpandable( bool _bExpandable )
{
    if ( _bExpandable == m_bExpandable )
    {
        return;
    }

    m_bExpandable = _bExpandable;

    // Hide/Show button
    if ( m_spButton != NULL )
    {
        m_spButton->SetVisible( m_bExpandable );
    }
}

bool TreeNode::IsExpandable() const
{
    return m_bExpandable;
}
        
void TreeNode::SetExpanded( bool _bExpanded )
{
    if ( m_spButton != NULL )
    {
        m_spButton->SetActive( _bExpanded );
    }

    // Dispatch event
    DispatchEvent( _bExpanded ? EVENT_TREE_NODE_EXPANDED : EVENT_TREE_NODE_COLLAPSED, GetSharedPtr< TreeNode >() );
}
       
bool TreeNode::IsExpanded() const
{
    return m_spButton != NULL && m_spButton->IsActive();
}
       
void TreeNode::SetSelected( bool _bSelected )
{
    if ( _bSelected == m_bSelected )
    {
        return;
    }

    m_bSelected = _bSelected;

    // Set panel active
    if ( m_spPanel != NULL )
    {
        m_spPanel->SetActive( _bSelected );

        // Set children active
        Unsigned32 u32NumChildren = m_spPanel->GetNumChildren();
        for ( Unsigned32 u32Idx = 0; u32Idx < u32NumChildren; ++u32Idx )
        {
            GuiObjectPtr spChild = m_spPanel->GetChild( u32Idx );
            if ( spChild != m_spButton )
            {
                spChild->SetActive( _bSelected );
            }
        }
    }

    // Dispatch event
}
        
bool TreeNode::IsSelected() const
{
    return m_bSelected;
}
       
void TreeNode::OnActivatedButton()
{
    SetExpanded( true );
}
        
void TreeNode::OnDeactivatedButton()
{
    SetExpanded( false );
}
        
void TreeNode::OnClickedPanel()
{
    // Dispatch event
    DispatchEvent( EVENT_TREE_NODE_CLICKED, GetSharedPtr< TreeNode >() );
}
        
void TreeNode::OnDoubleClickedPanel()
{
    // Dispatch event
    DispatchEvent( EVENT_TREE_NODE_DOUBLE_CLICKED, GetSharedPtr< TreeNode >() );
}

/*
///////////////////////////////////////////////////////////////////////////////
///
/// TreeNodeCreator
///
///////////////////////////////////////////////////////////////////////////////

const CFoundation::String TreeNodeCreator::GetTypeId() const
{
	return TreeNode::GetTypeID();
}

GuiObjectPtr TreeNodeCreator::CreateObject( const CFoundation::String& _strName ) const
{
	TreeNodePtr spTreeNode( new TreeNode( _strName ) );

	return spTreeNode;
}

void TreeNodeCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	CombinedCtrlCreator::InitFromXML( _spObject, _spXMLElement );

	TreeNodePtr spTreeNode = static_pointer_cast< TreeNode >( _spObject );

    // Initialize sub controls
    CFoundation::XMLNodePtr spChildNode = _spXMLElement->GetFirstChild();
    while ( spChildNode != NULL )
    {
        if ( spChildNode->IsElement() )
        {
            CFoundation::XMLElementPtr spElement = static_pointer_cast< CFoundation::XMLElement >( spChildNode );
            const CFoundation::String& strName = spElement->GetName();
            if ( strName == "button" )
            {
                GuiMain::GetInstance().InitFromXML( spTreeNode->m_spButton, spElement );
            }
            else if ( strName == "icon" )
            {
                GuiMain::GetInstance().InitFromXML( spTreeNode->m_spIcon, spElement );
            }
            else if ( strName == "textbox" )
            {
                GuiMain::GetInstance().InitFromXML( spTreeNode->m_spTextBox, spElement );
            }
        }

        spChildNode = spChildNode->GetNextSibling();
    }
}

bool TreeNodeCreator::SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const
{
    CombinedCtrlCreator::SaveToXML( _spObject, _spDoc, _spXMLElement );

    TreeNodeCPtr spTreeNode = static_pointer_cast< const TreeNode >( _spObject );

    return true;
}
*/