#include "cgui/GUI/Panel.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Enum/EnumConverter.h"
#include "cfoundation/XML/XMLDocument.h"
#include "cfoundation/XML/XMLElement.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/GuiObjectView.h"
#include "cgui/Gui/SizeMode.h"

using namespace CGui;

Panel::Panel( const CFoundation::String &sName )
:   GuiObject( sName )
{	
    m_pView = new GuiObjectView();

    // Create delegates
    m_spDelegateChildPositionChanged = PositionEventDelegate0Ptr( new PositionEventDelegate0( this, &Panel::OnChildPositionChanged ) );
    m_spDelegateChildSizeChanged = SizeEventDelegate0Ptr( new SizeEventDelegate0( this, &Panel::OnChildSizeChanged ) );
}

Panel::~Panel()
{
    SAFE_DELETE( m_pView );
}

void Panel::Clear()
{
    // Clear children
    Children aspChildren = m_aspChildren;
    for ( Children::reverse_iterator it = aspChildren.rbegin(); it != aspChildren.rend(); ++it )
    {
        GuiObjectPtr spChild = *it;
        spChild->Clear();
        RemoveChild( spChild );
    }

    GuiObject::Clear();
}

const CFoundation::String Panel::GetTypeID()
{
	return "panel";
}

const CFoundation::String Panel::GetTypeId() const
{
	return Panel::GetTypeID();
}

void Panel::StartInit()
{
	// Start init object
    GuiObject::StartInit();

    // Start init children
    for ( Children::iterator it = m_aspChildren.begin(); it != m_aspChildren.end(); ++it )
    {
        GuiObjectPtr pChild = *it;
        pChild->StartInit();
    }
}

void Panel::EndInit()
{
	// End init object
    GuiObject::EndInit();

    // End init children
    for ( Children::iterator it = m_aspChildren.begin(); it != m_aspChildren.end(); ++it )
    {
        GuiObjectPtr pChild = *it;
        pChild->EndInit();
    }
}

void Panel::Update( Float32 _f32TimeDifference )
{
	// Update Object
    GuiObject::Update( _f32TimeDifference );

    // Update children
    for ( Children::iterator it = m_aspChildren.begin(); it != m_aspChildren.end(); ++it )
    {
        GuiObjectPtr pChild = *it;
        pChild->Update( _f32TimeDifference );
    }
}

#ifdef MULTIPLE_RENDER_ELEMENTS
void Panel::Render( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements )
#else
void Panel::Render( const RenderParams& _Params, std::vector< RenderElement >& _aspRenderElements )
#endif
{
    // Render object
    GuiObject::Render( _Params, _aspRenderElements );

    if ( m_aspChildren.size() == 0 )
    {
        return;
    }

    // Calculate render rectangle for childs
    CFoundation::RectI32 clRenderRect = _Params.GetRenderRect();
    clRenderRect.SetPosition( _Params.GetPosition() );
    clRenderRect.SetSize( GetSize() );
    clRenderRect = clRenderRect.Intersect( _Params.GetRenderRect() );
    if ( clRenderRect.GetWidth() <= 0 ||
         clRenderRect.GetHeight() <= 0 )
    {
        return;
    }

    RenderParams paramsBase = _Params;
    paramsBase.AddAngle( GetAngle() );
    paramsBase.MultiplyAlpha( GetAlpha() );
    paramsBase.SetRenderRect( clRenderRect );

    // Render children
    for ( Children::iterator it = m_aspChildren.begin(); it != m_aspChildren.end(); ++it )
    {
        GuiObjectPtr pChild = *it;

        // Render child only if visible
	    if ( !pChild->IsVisible() )
	    {
            continue;
        }

        // Set render params
        RenderParams clNewParams = paramsBase;
        clNewParams.AddToPosition( pChild->GetRelativePosition() );

        pChild->Render( clNewParams, _aspRenderElements );

        // Dispatch post render event
        // TODO CO pChild->DispatchEvent( RENDER_EVENT_POST_RENDER, pChild, clNewParams, _aspRenderElements );
    }
}

		
GuiObjectPtr Panel::FindAt( const CFoundation::Vector2Di& _clPoint, CFoundation::Vector2Di& _clRelativePoint )
{
    if ( !IsVisible() )
    {
        return GuiObjectPtr();
    }
   
    // Get object rect
    const CFoundation::RectI32& clObjectRect = GetRelativeRect();

	// Test if inside
    if ( clObjectRect.IsPointIn( _clPoint ) )
	{
		// Test if child is pointed
		GuiObjectPtr pChild = PointToChild( _clPoint, _clRelativePoint );

		// No child at this point => This object is pointed
		if ( pChild == NULL )
		{
			_clRelativePoint -= GetRelativePosition();
			return GetSharedPtr< GuiObject >();
		}
		else
		{
			return pChild;
		}
	}
	// Test if on border
	else if ( clObjectRect.GetBordersPointIsOn( _clPoint, 2 ) != CFoundation::BORDER_NONE )
	{
		_clRelativePoint -= GetRelativePosition();
		return GetSharedPtr< GuiObject >();
	}
	// Test if inside
	else
	{
        return GuiObjectPtr();
	}
}

void Panel::AddChildFirst( GuiObjectPtr _spNewChild )
{
	if ( _spNewChild == NULL ||
         _spNewChild == GetSharedPtr() ||
          IsChild( _spNewChild ) )
	{
        return;
    }

    // Remove from old parent
    _spNewChild->RemoveFromParent();

    m_aspChildren.push_front( _spNewChild );
    InitChild( _spNewChild );

    // Dispatch event
    DispatchEvent( EVENT_PANEL_CHILD_ADDED, GetSharedPtr< Panel >(), _spNewChild );
    DispatchEvent( EVENT_PANEL_CHILDREN_CHANGED, GetSharedPtr< Panel >(), _spNewChild );
}

void Panel::AddChildLast( GuiObjectPtr _spNewChild )
{
	if ( _spNewChild == NULL ||
         _spNewChild == GetSharedPtr() ||
          IsChild( _spNewChild ) )
	{
        return;
    }

    // Remove from old parent
    _spNewChild->RemoveFromParent();

    m_aspChildren.push_back( _spNewChild );
    InitChild( _spNewChild );

    // Dispatch event
    DispatchEvent( EVENT_PANEL_CHILD_ADDED, GetSharedPtr< Panel >(), _spNewChild );
    DispatchEvent( EVENT_PANEL_CHILDREN_CHANGED, GetSharedPtr< Panel >(), _spNewChild );
}

void Panel::AddChildBefore( GuiObjectPtr _spNewChild, GuiObjectCPtr _spChildAfter )
{
	if ( _spNewChild == NULL ||
         _spNewChild == GetSharedPtr() ||
          IsChild( _spNewChild ) )
	{
        return;
    }

    // Remove from old parent
    _spNewChild->RemoveFromParent();

    for ( Children::iterator it = m_aspChildren.begin(); it != m_aspChildren.end(); ++it )
    {
        GuiObjectPtr pChild = *it;
        if ( pChild == _spChildAfter )
        {
            m_aspChildren.insert( it, _spNewChild );
            break;
        }
    }
    InitChild( _spNewChild );

    // Dispatch event
    DispatchEvent( EVENT_PANEL_CHILD_ADDED, GetSharedPtr< Panel >(), _spNewChild );
    DispatchEvent( EVENT_PANEL_CHILDREN_CHANGED, GetSharedPtr< Panel >(), _spNewChild );
}

void Panel::AddChildAfter( GuiObjectPtr _spNewChild, GuiObjectCPtr _spChildBefore )
{
	if ( _spNewChild == NULL ||
         _spNewChild == GetSharedPtr() ||
          IsChild( _spNewChild ) )
	{
        return;
    }

    // Search prev sibling
    Children::iterator it;
    for ( it = m_aspChildren.begin(); it != m_aspChildren.end(); ++it )
    {
        GuiObjectPtr pChild = *it;
        if ( pChild == _spChildBefore )
        {
            break;
        }
    }

    if ( it == m_aspChildren.end() )
    {
        CF_WARN( "Panel::AddChildAfter: Didn't find prev sibling." );
        return;
    }

    // Remove from old parent
    _spNewChild->RemoveFromParent();
    
    // Insert after found child
    m_aspChildren.insert( ++it, _spNewChild );

    InitChild( _spNewChild );

    // Dispatch event
    DispatchEvent( EVENT_PANEL_CHILD_ADDED, GetSharedPtr< Panel >(), _spNewChild );
    DispatchEvent( EVENT_PANEL_CHILDREN_CHANGED, GetSharedPtr< Panel >(), _spNewChild );
}

void Panel::RemoveChild( GuiObjectPtr _spChild )
{
    if ( _spChild == NULL )
    {
        return;
    }

    Children::iterator it = std::find( m_aspChildren.begin(), m_aspChildren.end(), _spChild );
    if ( it == m_aspChildren.end() )
    {
        //CF_WARN( L"Panel::RemoveChild: Removed child which was not in child vector." );
        return;
    }
        
    m_aspChildren.erase( it );

    // Deinitialize child
    DeinitChild( _spChild );

    // Recompute size if it depends on the children
    if ( !IsLayoutSuspended() &&
         ( GetSizeModeX() == SIZE_MODE_CONTENT ||
           GetSizeModeY() == SIZE_MODE_CONTENT ) )
    {
        UpdateSize();
    }

    // Dispatch events
    DispatchEvent( EVENT_PANEL_CHILD_REMOVED, GetSharedPtr< Panel >(), _spChild );
    DispatchEvent( EVENT_PANEL_CHILDREN_CHANGED, GetSharedPtr< Panel >(), _spChild );
}
        
void Panel::RemoveAllChildren()
{
    Children aspChildren = m_aspChildren;
    for ( Children::iterator it = aspChildren.begin(); it != aspChildren.end(); ++it )
    {
        GuiObjectPtr spChild = *it;
        DeinitChild( spChild );
    }
    m_aspChildren.clear();
    
    if ( GetSizeModeX() == SIZE_MODE_CONTENT ||
         GetSizeModeY() == SIZE_MODE_CONTENT )
    {
        UpdateSize();
    }
     
    // Dispatch events   
    for ( Children::iterator it = aspChildren.begin(); it != aspChildren.end(); ++it )
    {
        GuiObjectPtr spChild = *it;
        DispatchEvent( EVENT_PANEL_CHILD_REMOVED, GetSharedPtr< Panel >(), spChild );
        DispatchEvent( EVENT_PANEL_CHILDREN_CHANGED, GetSharedPtr< Panel >(), spChild );
    }
}

Unsigned32 Panel::GetNumChildren() const
{
    return static_cast< Unsigned32 >( m_aspChildren.size() );
}

GuiObjectPtr Panel::GetChild( Unsigned32 u32Idx )
{
    Children::iterator it = m_aspChildren.begin();
    while ( u32Idx > 0 &&
            it != m_aspChildren.end() )
    {
        --u32Idx;
        ++it;
    }

    if ( it != m_aspChildren.end() )
    {
        return *it;
    }
    else
    {
        return GuiObjectPtr();
    }
    /*if ( u32Idx < GetNumChildren() )
    {
        return m_aspChildren[ u32Idx ];
    }
    else
    {
        return NULL;
    }*/
}

const GuiObjectPtr Panel::GetChild( Unsigned32 u32Idx ) const
{
	Panel* pThis = const_cast< Panel* >( this );
	return pThis->GetChild( u32Idx );
}

GuiObjectPtr Panel::GetChildBefore( GuiObjectCPtr _spChild )
{
    for ( Children::reverse_iterator it = m_aspChildren.rbegin(); it != m_aspChildren.rend(); ++it )
    {
        if ( *it == _spChild )
        {
            // Goto child before this one
            ++it;

            // Check if valid
            if ( it != m_aspChildren.rend() )
            {
                return *it;
            }
            else
            {
                return GuiObjectPtr();
            }
        }
    }

    // No such child found
    return GuiObjectPtr();
}

const GuiObjectPtr Panel::GetChildBefore( GuiObjectCPtr _spChild ) const
{
	Panel *pThis = const_cast< Panel* >( this );
	return pThis->GetChildBefore( _spChild );
}

GuiObjectPtr Panel::GetChildAfter( GuiObjectCPtr _spChild )
{
    for ( Children::iterator it = m_aspChildren.begin(); it != m_aspChildren.end(); ++it )
    {
        if ( *it == _spChild )
        {
            // Goto child after this one
            ++it;

            // Check if valid
            if ( it != m_aspChildren.end() )
            {
                return *it;
            }
            else
            {
                return GuiObjectPtr();
            }
        }
    }

    // No such child found
    return GuiObjectPtr();
}

const GuiObjectPtr Panel::GetChildAfter( GuiObjectCPtr _spChild ) const
{
	Panel *pThis = const_cast< Panel* >( this );
	return pThis->GetChildAfter( _spChild );
}

ChildIterator Panel::GetChildIterator()
{
	return ChildIterator( GetSharedPtr< Panel >() );
}

const ChildIterator Panel::GetChildIterator() const
{
	Panel *pThis = const_cast< Panel* >( this );
	return pThis->GetChildIterator();
}

GuiObjectPtr Panel::FindChild( const CFoundation::String& _strName ) const
{
    for ( Children::const_iterator it = m_aspChildren.begin(); it != m_aspChildren.end(); ++it )
    {
        if ( (*it)->GetName() == _strName )
        {
            return *it;
        }
    }

    // No such child found
    return GuiObjectPtr();
}
		
GuiObjectPtr Panel::FindDescendant( const CFoundation::String& _strName ) const
{
    // Check childs
    GuiObjectPtr spObject = FindChild( _strName );
    if ( spObject != NULL )
    {
        return spObject;
    }

    // Check descendants of childs
    for ( Children::const_iterator it = m_aspChildren.begin(); it != m_aspChildren.end(); ++it )
    {
        // Check if panel
        PanelPtr spPanel = dynamic_pointer_cast< Panel >( *it );
        if ( spPanel != NULL )
        {
            spObject = spPanel->FindDescendant( _strName );
            if ( spObject != NULL )
            {
                break;
            }
        }
	}

    return spObject;
}

bool Panel::IsChild( GuiObjectCPtr _spChild ) const
{
    Children::const_iterator it = std::find( m_aspChildren.begin(), m_aspChildren.end(), _spChild );
    return it != m_aspChildren.end();
}

bool Panel::IsDescendant( GuiObjectCPtr _spObject ) const
{
    if ( _spObject == NULL )
    {
        return false;
    }

    for ( Children::const_iterator it = m_aspChildren.begin(); it != m_aspChildren.end(); ++it )
    {
		const GuiObjectPtr pChild = *it;
		if( pChild == _spObject )
		{
			return true;
		}

        // Check if panel
        PanelPtr spPanel = dynamic_pointer_cast< Panel >( pChild );
        if ( spPanel != NULL )
        {
            if ( spPanel->IsDescendant( _spObject ) )
            {
                return true;
            }
        }
	}
	return false;
}
        
void Panel::InitChild( GuiObjectPtr _spNewChild )
{
	_spNewChild->SetParent( GetSharedPtr< Panel >() );

    if ( IsInitializing() )
    {
        _spNewChild->StartInit();
    }
    else
    {
        if ( GetSizeModeX() == SIZE_MODE_CONTENT ||
             GetSizeModeY() == SIZE_MODE_CONTENT )
        {
            UpdateSize();
        }
    }

    // Register delegate for position and size changes
    _spNewChild->RegisterDelegate( POSITION_EVENT_POSITION_CHANGED, m_spDelegateChildPositionChanged );
    _spNewChild->RegisterDelegate( SIZE_EVENT_SIZE_CHANGED, m_spDelegateChildSizeChanged );
}
        
void Panel::DeinitChild( GuiObjectPtr _spChild )
{
    _spChild->RemoveParent();

    // Remove delegate for position and size changes
    _spChild->RemoveDelegate( POSITION_EVENT_POSITION_CHANGED, m_spDelegateChildPositionChanged );
    _spChild->RemoveDelegate( SIZE_EVENT_SIZE_CHANGED, m_spDelegateChildSizeChanged );
}

GuiObjectPtr Panel::PointToChild( const CFoundation::Vector2Di& _clPoint, CFoundation::Vector2Di& _clRelativePoint )
{
	GuiObjectPtr pFoundChild;
	CFoundation::Vector2Di clFoundRelativePoint = _clPoint;

	// Test childs
    for ( Children::reverse_iterator it = m_aspChildren.rbegin(); it != m_aspChildren.rend(); ++it )
    {
		GuiObjectPtr pChild = *it;

		CFoundation::Vector2Di clNewFoundRelativePoint = clFoundRelativePoint;
		clNewFoundRelativePoint -= GetRelativePosition();
		GuiObjectPtr pNewFoundChild = pChild->FindAt( _clPoint - GetRelativePosition(), clNewFoundRelativePoint );

		if ( pNewFoundChild != NULL )
		{
			pFoundChild = pNewFoundChild;
			clFoundRelativePoint = clNewFoundRelativePoint;
            break;
		}
	}

	_clRelativePoint = clFoundRelativePoint;

	return pFoundChild;
}

const GuiObjectPtr Panel::PointToChild( const CFoundation::Vector2Di& clPoint, CFoundation::Vector2Di &clRelativePoint ) const
{
    return const_cast< Panel* >( this )->PointToChild( clPoint, clRelativePoint );
}
        
CFoundation::Vector2Du Panel::ComputeSize()
{
    CFoundation::Vector2Du32 vNewSize = GuiObject::ComputeSize();

    // Compute children rect if necessary
    if ( GetSizeModeX() == SIZE_MODE_CONTENT ||
         GetSizeModeY() == SIZE_MODE_CONTENT )
    {
        CFoundation::RectI32 clRect;
        for ( Children::const_iterator it = m_aspChildren.begin(); it != m_aspChildren.end(); ++it )
        {
            const GuiObjectPtr spChild = *it;
            if ( spChild == NULL )
            {
                continue;
            }

            const CFoundation::RectI32& clChildRect = spChild->GetRelativeRect();
            clRect.Expand( clChildRect );
        }

        // The rect must always start at position (0,0)
        // because it should not be moved
        if ( GetSizeModeX() == SIZE_MODE_CONTENT )
        {
            Integer32 i32Width = clRect.GetWidth() + clRect.GetX();
            if ( i32Width > 0 )
            {
                vNewSize.SetX( static_cast< Unsigned32 >( i32Width ) );
            }
        }

        if ( GetSizeModeY() == SIZE_MODE_CONTENT )
        {
            Integer32 i32Height = clRect.GetHeight() + clRect.GetY();
            if ( i32Height > 0 )
            {
                vNewSize.SetY( static_cast< Unsigned32 >( i32Height ) );
            }
        }
    }

    return vNewSize;
}
        
void Panel::OnChildPositionChanged()
{
    if ( GetSizeModeX() == SIZE_MODE_CONTENT ||
         GetSizeModeY() == SIZE_MODE_CONTENT )
    {
        UpdateSize();
    }
}

void Panel::OnChildSizeChanged()
{
    if ( GetSizeModeX() == SIZE_MODE_CONTENT ||
         GetSizeModeY() == SIZE_MODE_CONTENT )
    {
        UpdateSize();
    }
}

const CFoundation::String PanelCreator::GetTypeId() const
{
	return Panel::GetTypeID();
}

GuiObjectPtr PanelCreator::CreateObject( const CFoundation::String &sName ) const
{
	PanelPtr spObject( new Panel( sName ) );
	return spObject;
}

void PanelCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	GuiObjectCreator::InitFromXML( _spObject, _spXMLElement );

	PanelPtr spPanel = static_pointer_cast< Panel >( _spObject );

	// create children
    const CFoundation::EnumConverter< State >& clStateConverter = CFoundation::EnumConverter< State >::GetInstance();
    CFoundation::XMLNodePtr pChildNode = _spXMLElement->GetFirstChild();
    while ( pChildNode != NULL )
    {
        if ( pChildNode->IsElement() )
        {
            CFoundation::XMLElementPtr pElement = static_pointer_cast< CFoundation::XMLElement >( pChildNode );

            // Check if state
            State eState = clStateConverter.Convert( pElement->GetName() );

            if ( eState == STATE_NONE )
            {
                // Get name
                const CFoundation::String& strName = pElement->GetAttribute( "name" );

                // Check if to override existent child
                GuiObjectPtr spObject = spPanel->FindChild( strName );
                if ( !strName.IsEmpty() &&
                      spObject != NULL )
                {
                    // Initialize from element
                    GuiMain::GetInstance().InitFromXML( spObject, pElement );
                }
                else
                {
                    // Create new object
                    spObject = GuiMain::GetInstance().CreateFromXML( pElement );
		            spPanel->AddChildLast( spObject );
                }
            }
        }
			
        pChildNode = pChildNode->GetNextSibling();
	}
}
		
void PanelCreator::InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const
{
	GuiObjectCreator::InitFromTemplate( _spObject, _spTemplate );

	PanelPtr spPanel = static_pointer_cast< Panel >( _spObject );
	PanelCPtr spPanelTemplate = static_pointer_cast< const Panel >( _spTemplate );

	// Create children
    for ( Panel::Children::const_iterator it = spPanelTemplate->m_aspChildren.begin(); it != spPanelTemplate->m_aspChildren.end(); ++it )
    {
        GuiObjectCPtr pChild = *it;

        GuiObjectPtr spObject = GuiMain::GetInstance().CreateFromTemplate( pChild->GetName(), pChild );
        spPanel->AddChildLast( spObject );
    }
}

bool PanelCreator::SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const
{
	GuiObjectCreator::SaveToXML( _spObject, _spDoc, _spXMLElement );

	PanelCPtr spPanel = static_pointer_cast< const Panel >( _spObject );

	// save children
	ChildIterator clIterator = spPanel->GetChildIterator();
	GuiObjectPtr pChild = clIterator.GetFirst();
	while( !clIterator.IsDone() )
	{
		// Create new XML node
		CFoundation::XMLElementPtr spChildXMLElement = _spDoc->CreateElement( pChild->GetTypeId() );

		// Add as child
		_spXMLElement->InsertChildLast( spChildXMLElement );

        // Save child
		GuiMain::GetInstance().SaveToXML( pChild, _spDoc, spChildXMLElement );

		pChild = clIterator.GetNext();
	}

    return true;
}
