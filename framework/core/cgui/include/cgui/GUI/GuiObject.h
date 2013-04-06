#ifndef CGUI_GUIOBJECT_H
#define CGUI_GUIOBJECT_H

#include <deque>

#include "cfoundation/Math/BorderType.h"
#include "cfoundation/Utils/Color.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Math/Rect.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/Math/Vector.h"

#include "cgui/GUI/GuiObjectEvents.h"
#include "cgui/GUI/Outline.h"
#include "cgui/GUI/RenderParams.h"
#include "cgui/GUI/State.h"

namespace CSystem
{
    class InputState;
}

namespace CGui
{
    enum SizeMode;

    INCLUDE_SHARED_CLASS( CtrlStyle );
    INCLUDE_SHARED_CLASS( GuiObject );
	class GuiObjectView;
    INCLUDE_SHARED_CLASS( Panel );
    INCLUDE_SHARED_CLASS( RenderElement );

    TYPEDEF_DELEGATE2( PanelEvent, PanelPtr, GuiObjectPtr );

    enum AttachPoint
    {
        ATTACH_POINT_INVALID,

        ATTACH_POINT_TOP_LEFT,
        ATTACH_POINT_TOP,
        ATTACH_POINT_TOP_RIGHT,

        ATTACH_POINT_LEFT,
        ATTACH_POINT_CENTER,
        ATTACH_POINT_RIGHT,   

        ATTACH_POINT_BOTTOM_LEFT,
        ATTACH_POINT_BOTTOM,
        ATTACH_POINT_BOTTOM_RIGHT,

        NUM_ATTACH_POINTS
    };

    enum AttachObject
    {
        ATTACH_OBJECT_INVALID,

        ATTACH_OBJECT_PARENT,
        ATTACH_OBJECT_PREV_SIBLING,

        ATTACH_OBJECT_END
    };
    extern CFoundation::EnumConverter< AttachObject >& g_ConverterAttachObject;

    class GuiObject : public CFoundation::SharedPtrBase< GuiObject >, 
                      public CFoundation::Delegate3Invoker< RenderEvent, GuiObjectPtr, const RenderParams&, std::vector< RenderElementPtr >& >, 
                      public CFoundation::Delegate1Invoker< StateEvent, GuiObjectPtr >, 
                      public CFoundation::Delegate3Invoker< StateEventStateState, GuiObjectPtr, State, State >,
                      public CFoundation::Delegate1Invoker< ObjectInputEvent, GuiObjectPtr >,
                      public CFoundation::Delegate1Invoker< SizeEvent, GuiObjectPtr >,
                      public CFoundation::Delegate3Invoker< PositionEvent, GuiObjectPtr, const CFoundation::Vector2Di&, const CFoundation::Vector2Di& >
	{
	public:
        DELEGATE3_INVOKER( RenderEvent, GuiObjectPtr, const RenderParams&, std::vector< RenderElementPtr >& );
        DELEGATE1_INVOKER( StateEvent, GuiObjectPtr );
        DELEGATE3_INVOKER( StateEventStateState, GuiObjectPtr, State, State );
        DELEGATE1_INVOKER( ObjectInputEvent, GuiObjectPtr );
        DELEGATE1_INVOKER( SizeEvent, GuiObjectPtr );
        DELEGATE3_INVOKER( PositionEvent, GuiObjectPtr, const CFoundation::Vector2Di&, const CFoundation::Vector2Di& );

        /// Destructor
		virtual ~GuiObject();

        /// Returns the type of this class
        static const CFoundation::String    GetTypeID();

        /// Returns the type of this object
		virtual const CFoundation::String   GetTypeId() const;

        /// Clears the object
		virtual void                        Clear();

        /// Set name
		virtual void                        SetName( const CFoundation::String &sName );

        /// Returns name
		virtual const CFoundation::String&      GetName() const;

        /// Suspends the update of the layout till ResumeLayout is called
        virtual void                        SuspendLayout();

        /// Indicates if the layout is suspended
        virtual bool                        IsLayoutSuspended() const;

        /// Resumes the update of the layout
        /// _bPerformLayout indicates if the layout should be updated immediately
        virtual void                        ResumeLayout( bool _bPerformLayout = true );

        /// DEPRECATED: Use SuspendLayout instead
        /// Starts initializing the object
        /** During initialization no messages are send and
         *  size, position, etc. are not updated
         */
        virtual void                        StartInit();

        /// DEPRECATED: Use IsLayoutSuspended instead
        /// Indicates if the object is currently initializing
        virtual bool                        IsInitializing() const;

        /// DEPRECATED: Use ResumeLayout instead
        /// Finishes initializing the object
        virtual void                        EndInit();

        /// Returns parent panel
		virtual PanelPtr                    GetParent();

        /// Returns parent panel
		virtual const PanelPtr              GetParent() const;

        /// Removes the object from its parent
        virtual void                        RemoveFromParent();

        /// Adds the passed object after this object
        virtual void                        AddAfter( GuiObjectPtr _spObject );

        /// Adds the passed object before this object
        virtual void                        AddBefore( GuiObjectPtr _spObject );

        /// Returns the previous sibling of this object, NULL if it has
        /// no parent or if it's the first child of its parent
        virtual GuiObjectPtr                GetPrevSibling();

        /// Returns the previous sibling of this object, NULL if it has
        /// no parent or if it's the first child of its parent
        virtual const GuiObjectPtr          GetPrevSibling() const;

        /// Returns the next sibling of this object, NULL if it has
        /// no parent or if it's the last child of its parent
        virtual GuiObjectPtr                GetNextSibling();

        /// Returns the next sibling of this object, NULL if it has
        /// no parent or if it's the last child of its parent
        virtual const GuiObjectPtr          GetNextSibling() const;

        /// Checks if the passed panel is an ancestor of this object
		virtual bool                        IsAncestor( GuiObjectCPtr _spObject ) const;

        /// Checks if the passed object is a descendant of this object
		virtual bool                        IsDescendant( GuiObjectCPtr _spObject ) const;

		virtual GuiObjectPtr                FindAt( const CFoundation::Vector2Di& clPoint, CFoundation::Vector2Di &clRelativePoint );
		virtual const GuiObjectPtr          FindAt( const CFoundation::Vector2Di& clPoint, CFoundation::Vector2Di &clRelativePoint ) const;

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Position
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// Sets the local attach point
        virtual void                        SetLocalAttachPoint( AttachPoint _eAttachPoint );

        /// Returns the local attach point
        virtual AttachPoint                 GetLocalAttachPoint() const;

        /// Sets the reference attach point
        virtual void                        SetReferenceAttachPoint( AttachPoint _eAttachPoint );

        /// Returns the reference attach point
        virtual AttachPoint                 GetReferenceAttachPoint() const;

        /// Sets the attach object with which the position
        /// of this object is computed
        virtual void                        SetAttachObject( AttachObject _eAttachObject );

        /// Returns the attach object
        virtual AttachObject                GetAttachObject() const;

        /// Sets the offset to the reference object
		virtual void                        SetOffset( const CFoundation::Vector2Di& _vOffset );

        /// Sets the x offset to the reference object
		virtual void                        SetOffsetX( Integer32 _i32OffsetX );

        /// Sets the y offset to the reference object
		virtual void                        SetOffsetY( Integer32 _i32OffsetY );

        /// Returns the offset to the reference object
		virtual const CFoundation::Vector2Di&   GetOffset() const;

        /// Returns the relative position to the parent object
		virtual const CFoundation::Vector2Di&   GetRelativePosition() const;

        /// Returns the absolute position on the screen
		virtual const CFoundation::Vector2Di    GetAbsolutePosition() const;

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Size
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// Sets the size mode for both dimensions
        virtual void                        SetSizeMode( SizeMode _eSizeMode );

        /// Sets the size mode
        virtual void                        SetSizeModeX( SizeMode _eSizeMode );

        /// Returns the size mode
        virtual SizeMode                    GetSizeModeX() const;

        /// Sets the size mode
        virtual void                        SetSizeModeY( SizeMode _eSizeMode );

        /// Returns the size mode
        virtual SizeMode                    GetSizeModeY() const;

        /// Sets the custom size
		virtual void                        SetCustomSize( const CFoundation::Vector2Du& _clCustomSize );

        /// Sets the custom size
		virtual void                        SetCustomSize( Unsigned32 _u32Width, Unsigned32 _u32Height );

        /// Returns the custom size
		virtual const CFoundation::Vector2Du&   GetCustomSize() const;

		virtual void                        SetCustomWidth( Unsigned32 u32Width );

		virtual Unsigned32                  GetCustomWidth() const;

		virtual void                        SetCustomHeight( Unsigned32 u32Height );

		virtual Unsigned32                  GetCustomHeight() const;

		virtual const CFoundation::Vector2Du&   GetSize() const;

		virtual Unsigned32                  GetWidth() const;

		virtual Unsigned32                  GetHeight() const;

        /// Returns the relative rect
		virtual const CFoundation::RectI32      GetRelativeRect() const;

        /// Returns the absolute screen rect
        virtual const CFoundation::RectI32      GetAbsoluteRect() const;

        /// Sets the scaling
		virtual void                        SetScale( const CFoundation::Vector3Df &rScale );

        /// Sets the angle
        virtual void                        SetAngle( Float16 _f16Angle );

        /// Returns the angle
        virtual Float16                     GetAngle() const;

		virtual void                        SetRotation( const CFoundation::Vector3Df& clRotation );

		virtual void                        SetSizeRatio( Float32 f32Ratio );
		virtual bool                        HasSizeRatio() const;
		virtual Float32                     GetSizeRatio() const;

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// State
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// Returns the current active state
        virtual State                       GetState() const;

        /// Sets the state flags
        virtual void                        SetStateFlags( Unsigned32 _u32StateFlags );

        /// Returns the state flags
        virtual Unsigned32                  GetStateFlags() const;

        /// Sets the passed state flag
        virtual void                        SetStateFlag( StateFlag _eStateFlag, bool _bSet );

        /// Indicates if the passed state flag is set or not
        virtual bool                        IsStateFlagSet( StateFlag _eStateFlag ) const;

        /// Activates/Deactivates the object
        /*! NOTE: Just a shortcut for SetStateFlag( STATE_FLAG_ACTIVE, _bActive )
         */
        virtual void                        SetActive( bool _bActive );

        /// Indicates if the object is active
        virtual bool                        IsActive() const;

        /// Highlightes/Dishighlightes the object
        virtual void                        SetHighlighted( bool _bHighlighted );

        /// Indicates if the object is highlighted
        virtual bool                        IsHighlighted() const;

        /// Enables/Disables the object
        virtual void                        SetDisabled( bool _bDisabled );

        /// Indicates if the object is disabled
        virtual bool                        IsDisabled() const;

		virtual void                        SetVisible( bool bVisible = true );
		virtual bool                        IsVisible() const;

		virtual bool                        HasFocus() const;

		virtual void                        SetIndependent( bool bIndependent );
		virtual bool                        IsIndependent() const;

		virtual void                        SetResizableAnchor( const CFoundation::BorderType& eAnchor );
		virtual const CFoundation::BorderType&  GetResizableAnchor() const;

		virtual void                        SetResizable( bool bResizable, const CFoundation::BorderType& eAnchor = CFoundation::BORDER_ALL );
		virtual bool                        IsResizable( const CFoundation::BorderType& eAnchor ) const;

        /// Sets the draggability
		virtual void                        SetDraggable( bool bDraggable );

        /// Indicates if the object is draggable
		virtual bool                        IsDraggable() const;

		/// Sets the alpha value of this object
		virtual void                        SetAlpha( Float16 _f16Alpha );

		/// Returns the alpha value of this object
		virtual Float16                     GetAlpha() const;

        /// Makes the background visible/invisible for all states
		virtual void                        SetBgVisible( bool _bBgVisible );

        /// Makes the background visible/invisible for the passed state
		virtual void                        SetBgVisible( State _eState, bool _bBgVisible );

        /// Returns if the background is visible
        virtual bool                        IsBgVisible( State _eState, bool _bUseFallback = true ) const;

        /// Sets the background color for all states
		virtual void                        SetBgColor( const CFoundation::Color& _clColor );

        /// Sets the background color of the passed state
		virtual void                        SetBgColor( State _eState, const CFoundation::Color& _clColor );

        /// Returns the background color of the passed state
		virtual const CFoundation::Color&   GetBgColor( State _eState, bool _bUseFallback = true ) const;

        /* DEPRECATED
		virtual void                        SetFgColor( const CFoundation::Color& clColor );
		virtual const CFoundation::Color&       GetFgColor() const;
        */

        /// Per frame update
		virtual void                        Update( Float32 _f32TimeDifference );

		/// Renders this object
#ifdef MULTIPLE_RENDER_ELEMENTS
		virtual void                        Render( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements );
#else
		virtual void                        Render( const RenderParams& _Params, std::vector< RenderElement >& _aRenderElements );
#endif

		virtual void                        SetOutline( Outline eOutline );
		virtual Outline                     GetOutline() const;

		virtual void                        SetToolTip( const CFoundation::String &sToolTip );
		virtual const CFoundation::String&      GetToolTip() const;

		virtual void                        SetTabIndex( Unsigned16 u16TabIndex );
		virtual Unsigned16                  GetTabIndex() const;

        /// Assigns the passed style
        virtual void                        SetStyle( CtrlStyleCPtr _spStyle );

        ////////////////////////////////////////////////////////////////////////////////////////////
        /// Input
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// Processes input as focused object
        /// Returns if input was processed. If not the input is send to the parent, 
        ///                                 if object isn't independent
        virtual bool                        ProcessInputFocused( const CSystem::InputState& _State );

        /// Processes input as pointed object
        /// Returns if input was processed. If not the input is send to the parent, 
        ///                                 if object isn't independent
        virtual bool                        ProcessInputPointed( const CSystem::InputState& _State );

        /// Called when the pointer entered the object
        virtual void                        OnPointerEntered();

        /// Called when the pointer left the object
        virtual void                        OnPointerLeft();

        /// Called when the object gained the focus
        virtual void                        OnGainedFocus();

        /// Called when the object lost the focus
        virtual void                        OnLostFocus();

        /// Called when the object was pressed
        virtual void                        OnPressed();

        /// Called when the object was released
        virtual void                        OnReleased();

        /// Called when the object was clicked
        virtual void                        OnClicked();

        /// Called when the object was double clicked
        virtual void                        OnDoubleClicked();

	protected:
        friend class Panel;

        /// Constructor
		GuiObject( const CFoundation::String& _strName );

        /// Sets the parent of this object
		virtual void                        SetParent( PanelPtr _spParent );

        /// Removes the parent of this object
		virtual void                        RemoveParent();

        /// Changes the current state
        virtual void                        SetState( State _eState );

        /// Changes the position of the object
        virtual bool                        SetPosition( const CFoundation::Vector2Di& _vPosition );

        /// Changes the size of the object
        virtual void                        SetSize( const CFoundation::Vector2Du& _vSize );

        /// Callbacks
        virtual void                        OnParentSizeChanged();
        virtual void                        OnParentChildrenChanged();
        virtual void                        OnAttachObjectSizeChanged();
        virtual void                        OnAttachObjectPositionChanged();

        /// Computes the size of the object
        /*! Depending on the size mode the real size
         *  of the object is computed.
         */
        virtual CFoundation::Vector2Du      ComputeSize();

        /// Updates the size of the object
        virtual void                        UpdateSize();

        /// View
        GuiObjectView*                      m_pView;

	private:
        friend class GuiObjectCreator;

        typedef std::map< State, CFoundation::Color > StateColorMap;
        typedef std::map< State, bool > StateBoolMap;

        const CFoundation::Vector2Di            GetAttachPointPosition( AttachPoint _eAttachPoint ) const;

        /// Checks for a new attach object and computes the 
        /// position new if neccessary
        void                                CheckAttachObject();

        /// Computes the current state
        /*! Depending on the active state flags the
         *  current state of the object is computed.
         */
        void                                ComputeState();

        /// Computes the relative position of the object
        /*! The offset of the object and its attach points
         *  are taken into account.
         */
        void                                ComputePosition();

        /// Delegates
        SizeEventDelegate0Ptr               m_spDelegateParentSizeChanged;
        PanelEventDelegate0Ptr              m_spDelegateParentChildrenChanged;
        SizeEventDelegate0Ptr               m_spDelegateAttachObjectSizeChanged;
        PositionEventDelegate0Ptr           m_spDelegateAttachObjectPositionChanged;

        /// Indicates if the layout is suspended
        bool                                m_bLayoutSuspended;

        /// Parent object
		PanelPtr                            m_spParent;

        /// Name
		CFoundation::String		                m_sName;

        /// Indicates if the object is independent
		bool				                m_bIndependent;

        /// Indicates if the object is visible
		bool				                m_bVisible;

        //////////////////////////////////////////////////////
        // Position
        //////////////////////////////////////////////////////

        AttachPoint                 m_eLocalAttachPoint;
        AttachPoint                 m_eReferenceAttachPoint;
        AttachObject                m_eAttachObject;

        /// Cached pointer to our attach object
        SharedPtr< GuiObject >      m_spAttachObject;

        CFoundation::Vector2Di          m_vOffset;
		CFoundation::Vector3Df	        m_clScale;

        /// The position is calculated by the offset
        /// and its attach points
		CFoundation::Vector2Di	                m_clPosition;

        //////////////////////////////////////////////////////
        // Rotation
        //////////////////////////////////////////////////////

        /// Angle
        Float16                         m_f16Angle;

		CFoundation::Vector3Df	        m_clRotation;

        //////////////////////////////////////////////////////
        // Size
        //////////////////////////////////////////////////////
        
        /// Size modes
        SizeMode                            m_eSizeModeX;
        SizeMode                            m_eSizeModeY;

        /// Custom size (width and height)
		CFoundation::Vector2Du	            m_vCustomSize;

        /// Ratio between width and height (width/height)
		Float32				                m_f32Ratio;

        //////////////////////////////////////////////////////
        // State
        //////////////////////////////////////////////////////

        /// State flags (which states are active)
        Unsigned32                          m_u32StateFlags;

        /// Indicates in which state the background is visible
        StateBoolMap                        m_bBgVisible;

        /// Background colors of the different states
        StateColorMap                       m_clBgColors;

        /// The currently shown state
        State                               m_eState;

		CFoundation::BorderType	                m_eResizableAnchor;
		bool				                m_bDraggable;

        /// Alpha value
        Float16                             m_f16Alpha;

		CFoundation::Color		                m_clFgColor;

		CFoundation::String		                m_sToolTip;

		Unsigned16			                m_u16TabIndex;
	};
}

#endif
