#include "cgui/GUI/GuiObject.h"

#include "cfoundation/Debug/Assert.h"
#include "cfoundation/Makros/BitMakros.h"

#include "csystem/Input/InputState.h"

#include "cgui/Gui/GuiMain.h"
#include "cgui/Gui/GuiObjectCreator.h"
#include "cgui/Gui/GuiObjectView.h"
#include "cgui/Gui/Panel.h"
#include "cgui/Gui/SizeMode.h"

#include "cgui/Messages/DraggabilityChangedMsg.h"
#include "cgui/Messages/ParentChangedMsg.h"
#include "cgui/Messages/ResizabilityChangedMsg.h"

using namespace CGui;

///////////////////////////////////////////////////////////////////////////////
///
/// GuiObject
///
///////////////////////////////////////////////////////////////////////////////

GuiObject::GuiObject( const CFoundation::String &sName )
:	m_pView( NULL ),
    m_bLayoutSuspended( false ),
    m_sName( sName ),
	m_bIndependent( true ),
	m_bVisible( true ),
    m_eLocalAttachPoint( ATTACH_POINT_TOP_LEFT ),
    m_eReferenceAttachPoint( ATTACH_POINT_TOP_LEFT ),
    m_eAttachObject( ATTACH_OBJECT_PARENT ),
    m_eSizeModeX( SIZE_MODE_CUSTOM ),
    m_eSizeModeY( SIZE_MODE_CUSTOM ),
	m_vCustomSize( 0, 0 ),
	m_f32Ratio( -1.0 ),
    m_eState( STATE_NORMAL ),
    m_u32StateFlags( 0 ),
	m_eResizableAnchor( CFoundation::BORDER_NONE ),
	m_bDraggable( false ),
    m_f16Alpha( 1.0f ),
    m_u16TabIndex( 0 )
{
	Clear();

    // Create delegates
    m_spDelegateParentSizeChanged = SizeEventDelegate0Ptr( new SizeEventDelegate0( this, &GuiObject::OnParentSizeChanged ) );
    m_spDelegateParentChildrenChanged = PanelEventDelegate0Ptr( new PanelEventDelegate0( this, &GuiObject::OnParentChildrenChanged ) );
    m_spDelegateAttachObjectSizeChanged = SizeEventDelegate0Ptr( new SizeEventDelegate0( this, &GuiObject::OnAttachObjectSizeChanged ) );
    m_spDelegateAttachObjectPositionChanged = PositionEventDelegate0Ptr( new PositionEventDelegate0( this, &GuiObject::OnAttachObjectPositionChanged ) );
}

GuiObject::~GuiObject()
{
    m_bBgVisible.clear();
}

const CFoundation::String GuiObject::GetTypeID()
{
    return "object";
}
		
const CFoundation::String GuiObject::GetTypeId() const
{
    return GetTypeID();
}

void GuiObject::Clear()
{
    // Remove listeners from attach object
    if ( m_spAttachObject != NULL )
    {
        m_spAttachObject->RemoveDelegate( SIZE_EVENT_SIZE_CHANGED, m_spDelegateAttachObjectSizeChanged );
        m_spAttachObject->RemoveDelegate( POSITION_EVENT_POSITION_CHANGED, m_spDelegateAttachObjectSizeChanged );

        // Reset attach object
        m_spAttachObject.reset();
    }
}

void GuiObject::Update( Float32 /*_f32TimeDifference*/ )
{
}

#ifdef MULTIPLE_RENDER_ELEMENTS
void GuiObject::Render( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements )
#else
void GuiObject::Render( const RenderParams& _Params, std::vector< RenderElement >& _aspRenderElements )
#endif
{
	if ( m_pView != NULL )
	{
		m_pView->Render( _Params, _aspRenderElements );
	}
}

void GuiObject::SetName( const CFoundation::String &sName )
{
	m_sName = sName;
}

const CFoundation::String& GuiObject::GetName() const
{
	return m_sName;
}
        
void GuiObject::SuspendLayout()
{
    m_bLayoutSuspended = true;
}
        
bool GuiObject::IsLayoutSuspended() const
{
    return m_bLayoutSuspended;
}

void GuiObject::ResumeLayout( bool _bPerformLayout )
{
    m_bLayoutSuspended = false;

    if ( _bPerformLayout )
    {
        // Compute state
        m_eState = STATE_NONE;
        ComputeState();

        // Compute attach object
        CheckAttachObject();

        // Update size
        UpdateSize();
    }
}

void GuiObject::StartInit()
{
    SuspendLayout();
}
        
bool GuiObject::IsInitializing() const
{
    return IsLayoutSuspended();
}

void GuiObject::EndInit()
{
    ResumeLayout();
}

PanelPtr GuiObject::GetParent()
{
	return m_spParent;
}

const PanelPtr GuiObject::GetParent() const
{
	return m_spParent;
}
        
void GuiObject::RemoveFromParent()
{
    if ( m_spParent == NULL )
    {
        return;
    }

    m_spParent->RemoveChild( GetSharedPtr< GuiObject >() );

    CheckAttachObject();
}
        
void GuiObject::AddAfter( GuiObjectPtr _spObject )
{
    if ( m_spParent == NULL )
    {
        return;
    }

    m_spParent->AddChildAfter( _spObject, GetSharedPtr< GuiObject >() );
}

void GuiObject::AddBefore( GuiObjectPtr _spObject )
{
    if ( m_spParent == NULL )
    {
        return;
    }

    m_spParent->AddChildBefore( _spObject, GetSharedPtr< GuiObject >() );
}

GuiObjectPtr GuiObject::GetPrevSibling()
{
    if ( m_spParent != NULL )
    {
        return m_spParent->GetChildBefore( GetSharedPtr< GuiObject >() );
    }
    else
    {
        return GuiObjectPtr();
    }
}

const GuiObjectPtr GuiObject::GetPrevSibling() const
{
	GuiObject* pThis = const_cast<GuiObject*>( this );
	return pThis->GetPrevSibling();
}

GuiObjectPtr GuiObject::GetNextSibling()
{
    if ( m_spParent != NULL )
    {
        return m_spParent->GetChildAfter( GetSharedPtr< GuiObject >() );
    }
    else
    {
        return GuiObjectPtr();
    }
}

const GuiObjectPtr GuiObject::GetNextSibling() const
{
	GuiObject* pThis = const_cast<GuiObject*>( this );
	return pThis->GetNextSibling();
}

bool GuiObject::IsAncestor( GuiObjectCPtr _spObject ) const
{
	return _spObject->IsDescendant( GetSharedPtr< GuiObject >() );
}
		
bool GuiObject::IsDescendant( GuiObjectCPtr /*_spObject*/ ) const
{
    return false;
}

GuiObjectPtr GuiObject::FindAt( const CFoundation::Vector2Di& _clPoint, CFoundation::Vector2Di& _clRelativePoint )
{
    if ( !IsVisible() )
    {
        return GuiObjectPtr();
    }
   
    // Get object rect
    const CFoundation::RectI32& clObjectRect = GetRelativeRect();

	// Check if on inside or on border
    if ( clObjectRect.IsPointIn( _clPoint ) ||
         clObjectRect.GetBordersPointIsOn( _clPoint, 2 ) != CFoundation::BORDER_NONE )
	{
		_clRelativePoint -= GetRelativePosition();
		return GetSharedPtr< GuiObject >();
	}
    else
    {
        return GuiObjectPtr();
    }
}

const GuiObjectPtr GuiObject::FindAt( const CFoundation::Vector2Di& _clPoint, CFoundation::Vector2Di& _clRelativePoint ) const
{
    return const_cast< GuiObject* >( this )->FindAt( _clPoint, _clRelativePoint );
}

void GuiObject::SetLocalAttachPoint( AttachPoint _eAttachPoint )
{
    if ( m_eLocalAttachPoint != _eAttachPoint )
    {
        m_eLocalAttachPoint = _eAttachPoint;

        ComputePosition();
    }
}

AttachPoint GuiObject::GetLocalAttachPoint() const
{
    return m_eLocalAttachPoint;
}

void GuiObject::SetReferenceAttachPoint( AttachPoint _eAttachPoint )
{
    if ( m_eReferenceAttachPoint != _eAttachPoint )
    {
        m_eReferenceAttachPoint = _eAttachPoint;

        ComputePosition();
    }
}
        
AttachPoint GuiObject::GetReferenceAttachPoint() const
{
    return m_eReferenceAttachPoint;
}

void GuiObject::SetAttachObject( AttachObject _eAttachObject )
{
    if ( m_eAttachObject != _eAttachObject )
    {
        m_eAttachObject = _eAttachObject;

        CheckAttachObject();
    }
}
        
AttachObject GuiObject::GetAttachObject() const
{
    return m_eAttachObject;
}

void GuiObject::SetOffset( const CFoundation::Vector2Di& _vOffset )
{
	if ( m_vOffset != _vOffset )
	{
		m_vOffset = _vOffset;

        ComputePosition();

        if ( m_eSizeModeX == SIZE_MODE_STRETCH ||
             m_eSizeModeY == SIZE_MODE_STRETCH )
        {
            UpdateSize();
        }
	}
}

void GuiObject::SetOffsetX( Integer32 _i32OffsetX )
{
	if ( m_vOffset.GetX() != _i32OffsetX )
	{
		m_vOffset.SetX( _i32OffsetX );

        ComputePosition();

        if ( m_eSizeModeX == SIZE_MODE_STRETCH )
        {
            UpdateSize();
        }
	}
}

void GuiObject::SetOffsetY( Integer32 _i32OffsetY )
{
	if ( m_vOffset.GetY() != _i32OffsetY )
	{
		m_vOffset.SetY( _i32OffsetY );

        ComputePosition();

        if ( m_eSizeModeY == SIZE_MODE_STRETCH )
        {
            UpdateSize();
        }
	}
}

const CFoundation::Vector2Di& GuiObject::GetOffset() const
{
    return m_vOffset;
}

const CFoundation::Vector2Di& GuiObject::GetRelativePosition() const
{
	return m_clPosition;
}
		
const CFoundation::Vector2Di GuiObject::GetAbsolutePosition() const
{
    CFoundation::Vector2Di vAbsolutePosition = m_clPosition;
    if ( m_spParent != NULL )
    {
        vAbsolutePosition += m_spParent->GetAbsolutePosition();
    }
    return vAbsolutePosition;
}

void GuiObject::SetScale( const CFoundation::Vector3Df &rScale )
{
	m_clScale = rScale;
}
        
void GuiObject::SetAngle( Float16 _f16Angle )
{
    SAFE_CALL( m_pView )->SetRotation( _f16Angle );
}

Float16 GuiObject::GetAngle() const
{
	if ( m_pView != NULL )
	{
        return m_pView->GetRotation();
    }
    else
    {
        return 0.0f;
    }
}

void GuiObject::SetRotation( const CFoundation::Vector3Df &rRotation )
{
	m_clRotation = rRotation;
}
        
void GuiObject::SetSizeMode( SizeMode _eSizeMode )
{
    SetSizeModeX( _eSizeMode );
    SetSizeModeY( _eSizeMode );
}

void GuiObject::SetSizeModeX( SizeMode _eSizeMode )
{
    if ( m_eSizeModeX == _eSizeMode )
    {
        return;
    }

    m_eSizeModeX = _eSizeMode;

    UpdateSize();        

    if ( m_eSizeModeX == SIZE_MODE_STRETCH &&
         m_spParent != NULL )
    {
        // Listen to parent size changes
        m_spParent->RegisterDelegate( SIZE_EVENT_SIZE_CHANGED, m_spDelegateParentSizeChanged );
    }
}

SizeMode GuiObject::GetSizeModeX() const
{
    return m_eSizeModeX;
}

void GuiObject::SetSizeModeY( SizeMode _eSizeMode )
{
    if ( m_eSizeModeY == _eSizeMode )
    {
        return;
    }

    m_eSizeModeY = _eSizeMode;

    UpdateSize();              

    if ( m_eSizeModeY == SIZE_MODE_STRETCH &&
         m_spParent != NULL )
    {
        // Listen to parent size changes
        m_spParent->RegisterDelegate( SIZE_EVENT_SIZE_CHANGED, m_spDelegateParentSizeChanged );
    }
}

SizeMode GuiObject::GetSizeModeY() const
{
    return m_eSizeModeY;
}

void GuiObject::SetCustomSize( const CFoundation::Vector2Du& _vCustomSize )
{
    // Only possible in custom size mode
    // and for stretching only one dimension
    if ( m_eSizeModeX != SIZE_MODE_CUSTOM &&
         m_eSizeModeY != SIZE_MODE_CUSTOM )
    {
        CF_WARN( L"GuiObject::SetSize: Called for object %s with no custom or stretch size mode!", GetName().wc_str() );
        return;
    }

    // Check ratio
    CFoundation::Vector2Du32 clCustomSizeTmp = _vCustomSize;
	if ( HasSizeRatio() )
	{
		clCustomSizeTmp.SetY( static_cast<Unsigned32>( clCustomSizeTmp.GetX() * GetSizeRatio() ) );
	}

	if ( m_vCustomSize != _vCustomSize )
	{
		m_vCustomSize = _vCustomSize;

        UpdateSize();
	}
}
		
void GuiObject::SetCustomSize( Unsigned32 _u32Width, Unsigned32 _u32Height )
{
    SetCustomSize( CFoundation::Vector2Du( _u32Width, _u32Height ) );
}

const CFoundation::Vector2Du& GuiObject::GetCustomSize() const
{
	return m_vCustomSize;
}

void GuiObject::SetCustomWidth( Unsigned32 u32Width )
{
    if( m_vCustomSize.GetX() != u32Width )
	{
		m_vCustomSize.SetX( u32Width );

		if( HasSizeRatio() )
		{
			Unsigned32 u32Height = static_cast<Unsigned32>( u32Width * GetSizeRatio() );
			if( u32Height != GetHeight() )
			{
				m_vCustomSize.SetY( u32Height );
			}
		}

        if ( !IsInitializing() )
        {
            if ( m_eSizeModeX == SIZE_MODE_CUSTOM ||
                 m_eSizeModeY == SIZE_MODE_CUSTOM )
            {
                UpdateSize();
            }
        }
	}
}

Unsigned32 GuiObject::GetCustomWidth() const
{
	return m_vCustomSize.GetX();
}

void GuiObject::SetCustomHeight( Unsigned32 u32Height )
{
	if( m_vCustomSize.GetY() != u32Height )
	{
		m_vCustomSize.SetY( u32Height );

		if( HasSizeRatio() )
		{
			Unsigned32 u32Width = static_cast<Unsigned32>( u32Height / GetSizeRatio() );
			if( u32Width != GetWidth() )
			{
				m_vCustomSize.SetX( u32Width );
			}
		}

        if ( !IsInitializing() )
        {
            if ( m_eSizeModeX == SIZE_MODE_CUSTOM ||
                 m_eSizeModeY == SIZE_MODE_CUSTOM )
            {
                UpdateSize();
            }
        }
	}
}

Unsigned32 GuiObject::GetCustomHeight() const
{
	return m_vCustomSize.GetY();
}

const CFoundation::Vector2Du& GuiObject::GetSize() const
{
	if ( m_pView != NULL )
	{
	    return m_pView->GetSize();
    }
    else
    {
        static CFoundation::Vector2Du vZero( 0, 0 );
        return vZero;
    }
}

Unsigned32 GuiObject::GetWidth() const
{
	if ( m_pView != NULL )
	{
        return m_pView->GetWidth();
    }
    else
    {
        return 0;
    }
}

Unsigned32 GuiObject::GetHeight() const
{
	if ( m_pView != NULL )
	{
        return m_pView->GetHeight();
    }
    else
    {
        return 0;
    }
}

void GuiObject::SetSizeRatio( Float32 f32Ratio )
{
	if ( f32Ratio != GetSizeRatio() )
	{
		m_f32Ratio = f32Ratio;
	}
}

bool GuiObject::HasSizeRatio() const
{
	return m_f32Ratio > 0.0;
}

Float32 GuiObject::GetSizeRatio() const
{
	return m_f32Ratio;
}
        
State GuiObject::GetState() const
{
    return m_eState;
}

void GuiObject::SetStateFlags( Unsigned32 _u32StateFlags )
{
    m_u32StateFlags = _u32StateFlags;
}

Unsigned32 GuiObject::GetStateFlags() const
{
    return m_u32StateFlags;
}

void GuiObject::SetStateFlag( StateFlag _eStateFlag, bool _bSet )
{
    bool bWasSet = IsStateFlagSet( _eStateFlag );
    if ( bWasSet == _bSet )
    {
        return;
    }

    if ( _bSet )
    {
        BITS_ON( m_u32StateFlags, _eStateFlag );
    }
    else
    {
        BITS_OFF( m_u32StateFlags, _eStateFlag );
    }

    if ( !IsInitializing() )
    {
        // Compute new state
        ComputeState();
    }

    // Invoke delegates
    switch ( _eStateFlag )
    {
    case STATE_FLAG_ACTIVE:
        {
            DispatchEvent( _bSet ? STATE_EVENT_ACTIVATED : STATE_EVENT_DEACTIVATED, GetSharedPtr< GuiObject >() );
        }
        break;
    case STATE_FLAG_FOCUSED:
        {
            DispatchEvent( _bSet ? STATE_EVENT_FOCUSED : STATE_EVENT_UNFOCUSED, GetSharedPtr< GuiObject >() );
        }
        break;
    };
}
  
bool GuiObject::IsStateFlagSet( StateFlag _eStateFlag ) const
{
    return TEST_BITS_ON( m_u32StateFlags, static_cast< Unsigned32 >( _eStateFlag ) );
}

void GuiObject::SetActive( bool _bActive )
{
    SetStateFlag( STATE_FLAG_ACTIVE, _bActive );
}

bool GuiObject::IsActive() const
{
    return IsStateFlagSet( STATE_FLAG_ACTIVE );
}

void GuiObject::SetHighlighted( bool _bHighlighted )
{
    SetStateFlag( STATE_FLAG_HIGHLIGHT, _bHighlighted );
}

bool GuiObject::IsHighlighted() const
{
    return IsStateFlagSet( STATE_FLAG_HIGHLIGHT );
}
        
void GuiObject::SetDisabled( bool _bDisabled )
{
    SetStateFlag( STATE_FLAG_DISABLED, _bDisabled );
}

bool GuiObject::IsDisabled() const
{
    return IsStateFlagSet( STATE_FLAG_DISABLED );
}

void GuiObject::SetVisible( bool bVisible )
{
	m_bVisible = bVisible;
}

bool GuiObject::IsVisible() const
{
    return m_bVisible;
}

bool GuiObject::HasFocus() const
{
    return IsStateFlagSet( STATE_FLAG_FOCUSED );
}

void GuiObject::SetIndependent( bool bIndependent )
{
	m_bIndependent = bIndependent;
}

bool GuiObject::IsIndependent() const
{
    return m_bIndependent;
}

void GuiObject::SetResizableAnchor( const CFoundation::BorderType& eAnchor )
{
	m_eResizableAnchor = eAnchor;
}

const CFoundation::BorderType& GuiObject::GetResizableAnchor() const
{
	return m_eResizableAnchor;
}

void GuiObject::SetResizable( bool bResizable, const CFoundation::BorderType& eAnchor )
{
	if( bResizable )
	{
		m_eResizableAnchor = CFoundation::BorderType( m_eResizableAnchor | eAnchor );
	}
	else
	{
		m_eResizableAnchor = CFoundation::BorderType( m_eResizableAnchor & ~eAnchor );
	}
/*
	ResizabilityChangedMsg clMsg( bResizable, bOldValue );
	clMsg.SetSender( this );
	clMsg.SetReceiver( this );
	clMsg.SendToReceiver();*/
}

bool GuiObject::IsResizable( const CFoundation::BorderType& eAnchor ) const
{
	return ( m_eResizableAnchor == eAnchor ) || ( m_eResizableAnchor & eAnchor );
}

void GuiObject::SetDraggable( bool bDraggable )
{
	bool bOldValue = m_bDraggable;
	if ( bOldValue != bDraggable )
	{
        m_bDraggable = bDraggable;
	}
}

bool GuiObject::IsDraggable() const
{
	return m_bDraggable;
}

const CFoundation::RectI32 GuiObject::GetRelativeRect() const
{
	return CFoundation::RectI32( GetRelativePosition(), GetSize() );
}
        
const CFoundation::RectI32 GuiObject::GetAbsoluteRect() const
{
	return CFoundation::RectI32( GetAbsolutePosition(), GetSize() );
}
		
void GuiObject::SetAlpha( Float16 _f16Alpha )
{
    if ( _f16Alpha == m_f16Alpha )
    {
        return;
    }

    m_f16Alpha = _f16Alpha;

	if ( m_pView != NULL )
	{
        m_pView->SetAlpha( m_f16Alpha );
    }
}

Float16 GuiObject::GetAlpha() const
{
    return m_f16Alpha;
}
		
void GuiObject::SetBgVisible( bool _bBgVisible )
{
    SetBgVisible( STATE_NORMAL, _bBgVisible );
}

void GuiObject::SetBgVisible( State _eState, bool _bBgVisible )
{
    m_bBgVisible[ _eState ] = _bBgVisible;

    if ( GetState() == _eState &&
         m_pView != NULL )
    {
        // Set the background visible
        m_pView->SetBgVisible( _bBgVisible );
    }
}

bool GuiObject::IsBgVisible( State _eState, bool _bUseFallback ) const
{
    do
    {
        // Try to find visibility
        StateBoolMap::const_iterator it = m_bBgVisible.find( _eState );
        if ( it != m_bBgVisible.end() )
        {
            return it->second;
        }
        else if ( !_bUseFallback )
        {
            break;
        }

        // Get fallback state
        _eState = GetFallbackState( _eState );
    } while ( _eState != STATE_NONE );

    return true;
}
		
void GuiObject::SetBgColor( const CFoundation::Color& _clColor )
{
    SetBgColor( STATE_NORMAL, _clColor );
}

void GuiObject::SetBgColor( State _eState, const CFoundation::Color& _clColor )
{
    m_clBgColors[ _eState ] = _clColor;

    if ( GetState() == _eState &&
         m_pView != NULL )
    {
        // Set the background color
        m_pView->SetBgColor( _clColor );
    }
}

const CFoundation::Color& GuiObject::GetBgColor( State _eState, bool _bUseFallback ) const
{
    StateColorMap::const_iterator it = m_clBgColors.find( _eState );
    while ( it == m_clBgColors.end() &&
            _eState != STATE_NONE &&
            _bUseFallback )
    {
        _eState = GetFallbackState( _eState );

        // Try to return color of fallback state
        it = m_clBgColors.find( _eState );
    }

    if ( it != m_clBgColors.end() )
    {
        return it->second;
    }
    else
    {
        static CFoundation::Color clDefaultBgColor( 1.0f, 1.0f, 1.0f, 1.0f );
        return clDefaultBgColor;
    }
}

/* DEPRECATED
void GuiObject::SetFgColor( const CFoundation::Color &clColor )
{
	m_clFgColor = clColor;
}

const CFoundation::Color& GuiObject::GetFgColor() const
{
	return m_clFgColor;
}*/

void GuiObject::SetParent( PanelPtr pParent )
{
	if( m_spParent == pParent )
	{
        return;
    }

    PanelPtr spOldParent = m_spParent;

    // Remove listeners from old parent
    if ( m_spParent != NULL )
    {
        m_spParent->RemoveDelegate( SIZE_EVENT_SIZE_CHANGED, m_spDelegateParentSizeChanged );
        m_spParent->RemoveDelegate( EVENT_PANEL_CHILDREN_CHANGED, m_spDelegateParentChildrenChanged );
    }

	m_spParent = pParent;

    if ( m_spParent != NULL )
    {
        m_spParent->RegisterDelegate( EVENT_PANEL_CHILDREN_CHANGED, m_spDelegateParentChildrenChanged );
    }

    if ( !IsInitializing() )
    {
        CheckAttachObject();
    }

    if ( m_eSizeModeX == SIZE_MODE_STRETCH ||
         m_eSizeModeY == SIZE_MODE_STRETCH )
    {
        if ( !IsInitializing() )
        {
            UpdateSize();
        }

        if ( m_spParent != NULL )
        {
            m_spParent->RegisterDelegate( SIZE_EVENT_SIZE_CHANGED, m_spDelegateParentSizeChanged );
        }
    }
}

void GuiObject::RemoveParent()
{
    SetParent( PanelPtr() );
}

void GuiObject::SetOutline( Outline eOutline )
{
	if ( m_pView != NULL )
	{
        m_pView->SetOutline( eOutline );
    }
}

Outline GuiObject::GetOutline() const
{
	if( m_pView != NULL )
	{
		return m_pView->GetOutline();
	}
	else
	{
		return OUTLINE_NONE;
	}
}

void GuiObject::SetToolTip( const CFoundation::String &sToolTip )
{
	m_sToolTip = sToolTip;
}

const CFoundation::String& GuiObject::GetToolTip() const
{
	return m_sToolTip;
}

void GuiObject::SetTabIndex( Unsigned16 u16TabIndex )
{
	m_u16TabIndex = u16TabIndex;
}

Unsigned16 GuiObject::GetTabIndex() const
{
	return m_u16TabIndex;
}

void GuiObject::SetStyle( CtrlStyleCPtr _spStyle )
{
	GuiObjectCreatorPtr spCreator = GuiMain::GetInstance().GetObjectCreator( GetTypeId() );
	if ( spCreator != NULL )
	{
		spCreator->AssignStyle( GetSharedPtr< GuiObject >(), *_spStyle );
	}
}

bool GuiObject::ProcessInputFocused( const CSystem::InputState& /*_state*/ )
{
    return false;
}

bool GuiObject::ProcessInputPointed( const CSystem::InputState& _state )
{
    CSystem::MouseButtonState eLMBState = _state.m_eMouseButtonStates[ CSystem::MOUSE_LEFT ];

    // Check if pressed
    if ( eLMBState == CSystem::MBS_FIRST_DOWN )
    {
        OnPressed();
    }
    // Check if released
    else if ( eLMBState == CSystem::MBS_FIRST_UP )
    {   
        OnReleased();
    }

    return false;
}

void GuiObject::OnPointerEntered()
{
    SetHighlighted( true );

    DispatchEvent( INPUT_EVENT_POINTER_ENTERED, GetSharedPtr< GuiObject >() );
}

void GuiObject::OnPointerLeft()
{
    SetHighlighted( false );

    DispatchEvent( INPUT_EVENT_POINTER_LEFT, GetSharedPtr< GuiObject >() );
}

void GuiObject::OnGainedFocus()
{
    if ( IsStateFlagSet( STATE_FLAG_FOCUSED ) )
    {
        return;
    }

    SetStateFlag( STATE_FLAG_FOCUSED, true );
    DispatchEvent( INPUT_EVENT_GAINED_FOCUS, GetSharedPtr< GuiObject >() );
}

void GuiObject::OnLostFocus()
{
    if ( !IsStateFlagSet( STATE_FLAG_FOCUSED ) )
    {
        return;
    }

    SetStateFlag( STATE_FLAG_FOCUSED, false );
    DispatchEvent( INPUT_EVENT_LOST_FOCUS, GetSharedPtr< GuiObject >() );
}

void GuiObject::OnPressed()
{
    DispatchEvent( INPUT_EVENT_PRESSED, GetSharedPtr< GuiObject >() );
}

void GuiObject::OnReleased()
{
    DispatchEvent( INPUT_EVENT_RELEASED, GetSharedPtr< GuiObject >() );
}

void GuiObject::OnClicked()
{
    DispatchEvent( INPUT_EVENT_CLICKED, GetSharedPtr< GuiObject >() );
}

void GuiObject::OnDoubleClicked()
{
    DispatchEvent( INPUT_EVENT_DOUBLE_CLICKED, GetSharedPtr< GuiObject >() );
}

bool GuiObject::SetPosition( const CFoundation::Vector2Di& _vPosition )
{
    if ( _vPosition == m_clPosition )
    {
        return false;
    }

    const CFoundation::Vector2Di& vOldPosition = m_clPosition;

    m_clPosition = _vPosition;

    // Dispatch event
    DispatchEvent( POSITION_EVENT_POSITION_CHANGED, GetSharedPtr< GuiObject >(), vOldPosition, m_clPosition );

    return true;
}
        
void GuiObject::SetSize( const CFoundation::Vector2Du& _vSize )
{
    if ( GetSize() == _vSize )
    {
        return;
    }
    if ( m_pView != NULL )
    {
        // Set new size
	    m_pView->SetSize( _vSize );
    }

    // Update position if dependent on size
    if ( m_eLocalAttachPoint != ATTACH_POINT_TOP_LEFT )
    {
        ComputePosition();
    }

    // Dispatch event
    DispatchEvent( SIZE_EVENT_SIZE_CHANGED, GetSharedPtr< GuiObject >() );
}
        
const CFoundation::Vector2Di GuiObject::GetAttachPointPosition( AttachPoint _eAttachPoint ) const
{
    Integer32 i32X = 0;
    switch ( _eAttachPoint )
    {
    case ATTACH_POINT_TOP:
    case ATTACH_POINT_CENTER:
    case ATTACH_POINT_BOTTOM:
        {
            i32X = GetWidth() / 2;
        }
        break;
    case ATTACH_POINT_TOP_RIGHT:
    case ATTACH_POINT_RIGHT:
    case ATTACH_POINT_BOTTOM_RIGHT:
        {
            i32X = GetWidth();
        }
        break;
    }
    Integer32 i32Y = 0;
    switch ( _eAttachPoint )
    {
    case ATTACH_POINT_LEFT:
    case ATTACH_POINT_CENTER:
    case ATTACH_POINT_RIGHT:
        {
            i32Y = GetHeight() / 2;
        }
        break;
    case ATTACH_POINT_BOTTOM_LEFT:
    case ATTACH_POINT_BOTTOM:
    case ATTACH_POINT_BOTTOM_RIGHT:
        {
            i32Y = GetHeight();
        }
        break;
    }
    return CFoundation::Vector2Di32( i32X, i32Y );
}
       
void GuiObject::CheckAttachObject()
{
    GuiObjectPtr pNewAttachObject;
    switch ( m_eAttachObject )
    {
    case ATTACH_OBJECT_PARENT:
        {
            pNewAttachObject = m_spParent;
        }
        break;
    case ATTACH_OBJECT_PREV_SIBLING:
        {
            pNewAttachObject = GetPrevSibling();
            if ( pNewAttachObject == NULL )
            {
                pNewAttachObject = m_spParent;
            }
        }
        break;
    }

    // Set new attach object
    if ( m_spAttachObject == pNewAttachObject )
    {
        return;
    }

    // Remove delegates
    if ( m_spAttachObject != NULL )
    {
        m_spAttachObject->RemoveDelegate( SIZE_EVENT_SIZE_CHANGED, m_spDelegateAttachObjectSizeChanged );
        m_spAttachObject->RemoveDelegate( POSITION_EVENT_POSITION_CHANGED, m_spDelegateAttachObjectSizeChanged );
    }

    m_spAttachObject = pNewAttachObject;

    if ( m_spAttachObject != NULL )
    {
        // Listen to size and position changes 
        // if object is attached to previous sibling
        // and remove from old one
        if ( m_eAttachObject == ATTACH_OBJECT_PREV_SIBLING )
        {
            m_spAttachObject->RegisterDelegate( POSITION_EVENT_POSITION_CHANGED, m_spDelegateAttachObjectSizeChanged );
        }

        // Listen to size changes in general
        m_spAttachObject->RegisterDelegate( SIZE_EVENT_SIZE_CHANGED, m_spDelegateAttachObjectSizeChanged );
    }
    
    // Recompute position
    ComputePosition();
}
        
void GuiObject::ComputeState()
{
    State eNewState = STATE_NORMAL;

    if ( IsStateFlagSet( STATE_FLAG_DISABLED ) )
    {
        eNewState = STATE_DISABLED;
    }
    else if ( IsStateFlagSet( STATE_FLAG_HIGHLIGHT ) )
    {
        if ( IsStateFlagSet( STATE_FLAG_ACTIVE ) )
        {
            eNewState = STATE_ACTIVE_HIGHLIGHT;
        }
        else
        {
            eNewState = STATE_HIGHLIGHT;
        }
    }
    else if ( IsStateFlagSet( STATE_FLAG_INFORMATION ) )
    {
        eNewState = STATE_INFORMATION;
    }
    else if ( IsStateFlagSet( STATE_FLAG_MODIFIED ) )
    {
        eNewState = STATE_MODIFIED;
    }
    else if ( IsStateFlagSet( STATE_FLAG_ACTIVE ) )
    {
        eNewState = STATE_ACTIVE;
    }

    // Set new state 
    SetState( eNewState );
}
        
void GuiObject::SetState( State _eState )
{
    if ( m_eState == _eState )
    {
        return;
    }

    State eOldState = m_eState;

    m_eState = _eState;

    if ( m_pView != NULL )
    {
        // Set the background color
        m_pView->SetBgColor( GetBgColor( m_eState ) );

        // Set background visible
        m_pView->SetBgVisible( IsBgVisible( m_eState ) );
    }
    
    // Dispatch event
    DispatchEvent( STATE_EVENT_CHANGED, GetSharedPtr< GuiObject >(), m_eState, eOldState );
}
        
void GuiObject::ComputePosition()
{
    CFoundation::Vector2Di clReferenceAttachPointPosition;
    if ( m_spAttachObject != NULL )
    {
        // For attaching on the sibling the relative position 
        // of the sibling has to be added
        if ( m_eAttachObject == ATTACH_OBJECT_PREV_SIBLING )
        {
            clReferenceAttachPointPosition = m_spAttachObject->GetRelativePosition();
        }

        // Add reference attach point position
        clReferenceAttachPointPosition += m_spAttachObject->GetAttachPointPosition( m_eReferenceAttachPoint );
    }

    // Get local attach point position
    const CFoundation::Vector2Di& clLocalAttachPointPosition = GetAttachPointPosition( m_eLocalAttachPoint );

    // Set position
    CFoundation::Vector2Di vNewPosition = m_vOffset + clReferenceAttachPointPosition - clLocalAttachPointPosition;
    SetPosition( vNewPosition );
}

void GuiObject::UpdateSize()
{
    CFoundation::Vector2Du32 vNewSize = ComputeSize();
    SetSize( vNewSize );
}
        
CFoundation::Vector2Du GuiObject::ComputeSize()
{
    CFoundation::Vector2Du32 vNewSize = m_vCustomSize;

    // Compute width
    switch ( m_eSizeModeX )
    {
    case SIZE_MODE_STRETCH:
        {
            if ( m_spParent != NULL )
            {
                const CFoundation::Vector2Du32& clParentSize = m_spParent->GetSize();
                vNewSize.SetX( clParentSize.GetX() - std::max( m_vOffset.GetX(), 0 ) );
            }
        }
        break;
    }

    // Compute height
    switch ( m_eSizeModeY )
    {
    case SIZE_MODE_STRETCH:
        {
            if ( m_spParent != NULL )
            {
                const CFoundation::Vector2Du32& clParentSize = m_spParent->GetSize();
                vNewSize.SetY( clParentSize.GetY() - std::max( m_vOffset.GetY(), 0 ) );
            }
        }
        break;
    }                

    // Check ratio
    if ( HasSizeRatio() )
    {
        //clNewSize.SetX( static_cast<Unsigned32>( clNewSize.GetY() / GetSizeRatio() ) );
    }

    return vNewSize;
}

void GuiObject::OnParentSizeChanged()
{
    // Recompute size if size of the parent changed and the
    // size mode is "fill parent"
    if ( m_eSizeModeX == SIZE_MODE_STRETCH ||
         m_eSizeModeY == SIZE_MODE_STRETCH )
    {
        UpdateSize();
    }      
}
        
void GuiObject::OnParentChildrenChanged()
{
    if ( m_eAttachObject == ATTACH_OBJECT_PREV_SIBLING )
    {
        CheckAttachObject();    
    }
}

void GuiObject::OnAttachObjectSizeChanged()
{
    // Recompute position if the size of our attach
    // object changed
    ComputePosition();
}

void GuiObject::OnAttachObjectPositionChanged()
{
    // Recompute position if the position of our attach
    // object changed
    ComputePosition();
}
