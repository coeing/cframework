#ifndef CGUI_PANEL_H
#define CGUI_PANEL_H

#include <list>

#include "cfoundation/Utils/Singleton.h"

#include "cgui/GUI/ChildIterator.h"
#include "cgui/GUI/GuiObject.h"
#include "cgui/GUI/GuiObjectCreator.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( Panel );

    enum PanelEvent
    {
        EVENT_PANEL_CHILD_ADDED,
        EVENT_PANEL_CHILD_REMOVED,
        EVENT_PANEL_CHILDREN_CHANGED,
    };
    TYPEDEF_DELEGATE2( PanelEvent, PanelPtr, GuiObjectPtr );

    class Panel : public GuiObject, public CFoundation::Delegate2Invoker< PanelEvent, PanelPtr, GuiObjectPtr >
	{
	public:
        DELEGATE3_INVOKER( RenderEvent, GuiObjectPtr, const RenderParams&, std::vector< RenderElementPtr >& );
        DELEGATE1_INVOKER( StateEvent, GuiObjectPtr );
        DELEGATE1_INVOKER( ObjectInputEvent, GuiObjectPtr );
        DELEGATE1_INVOKER( SizeEvent, GuiObjectPtr );
        DELEGATE3_INVOKER( PositionEvent, GuiObjectPtr, const CFoundation::Vector2Di&, const CFoundation::Vector2Di& );

        DELEGATE2_INVOKER( PanelEvent, PanelPtr, GuiObjectPtr );

        /// Destructor
		virtual ~Panel();

        /// Clears the object
		virtual void                    Clear();

		static const CFoundation::String    GetTypeID();
		virtual const CFoundation::String   GetTypeId() const;

        /// Starts initializing the object
        /** During initialization no messages are send and
         *  size, position, etc. are not updated
         */
        virtual void                    StartInit();

        /// Finishes initializing the object
        virtual void                    EndInit();

        /// Per frame update
		virtual void                    Update( Float32 _f32TimeDifference );

		/// Renders this object
#ifdef MULTIPLE_RENDER_ELEMENTS
		virtual void                        Render( const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements );
#else
		virtual void                        Render( const RenderParams& _Params, std::vector< RenderElement >& _aRenderElements );
#endif

        //! Returns the gui object at the passed point on this panel
		virtual GuiObjectPtr            FindAt( const CFoundation::Vector2Di& _clPoint, CFoundation::Vector2Di& _clRelativePoint );

		virtual void                    AddChildFirst( GuiObjectPtr _spNewChild );
		virtual void                    AddChildLast( GuiObjectPtr _spNewChild );
		virtual void                    AddChildBefore( GuiObjectPtr _spNewChild, GuiObjectCPtr _spChildAfter );
		virtual void                    AddChildAfter( GuiObjectPtr _spNewChild, GuiObjectCPtr _spChildBefore );

        /// Removes the passed child
		virtual void                    RemoveChild( GuiObjectPtr _spChild );

        /// Removes all children
        virtual void                    RemoveAllChildren();

		virtual Unsigned32              GetNumChildren() const;

		virtual GuiObjectPtr            GetChild( Unsigned32 _u32Idx );
		virtual const GuiObjectPtr      GetChild( Unsigned32 _u32Idx ) const;

        //! Returns the child before the passed child, NULL if the passed
        //! object is no child or the first child
        virtual GuiObjectPtr            GetChildBefore( GuiObjectCPtr _spChild );

        //! Returns the child before the passed child, NULL if the passed
        //! object is no child or the first child
        virtual const GuiObjectPtr      GetChildBefore( GuiObjectCPtr _spChild ) const;

        //! Returns the child after the passed child, NULL if the passed
        //! object is no child or the last child
        virtual GuiObjectPtr            GetChildAfter( GuiObjectCPtr _spChild );

        //! Returns the child after the passed child, NULL if the passed
        //! object is no child or the last child
        virtual const GuiObjectPtr      GetChildAfter( GuiObjectCPtr _spChild ) const;

		virtual ChildIterator           GetChildIterator();
		virtual const ChildIterator     GetChildIterator() const;

        /// Searches for a child with the passed name
		virtual GuiObjectPtr            FindChild( const CFoundation::String& _strName ) const;

        /// Searches for an ancestor with the passed name
		virtual GuiObjectPtr            FindDescendant( const CFoundation::String& _strName ) const;

        //! Checks if the passed object is a child of this panel
		virtual bool                    IsChild( GuiObjectCPtr _spObject ) const;

        //! Checks if the passed object is a descendant of this panel
		virtual bool                    IsDescendant( GuiObjectCPtr _spObject ) const;

    protected:
        /// Constructor
		Panel( const CFoundation::String& _strName );

        //! Computes the size of the object
        /*! Depending on the size mode the real size
         *  of the object is computed.
         */
        virtual CFoundation::Vector2Du  ComputeSize();

        /// Called when a childs position changed
        virtual void                    OnChildPositionChanged();

        /// Called when a childs size changed
        virtual void                    OnChildSizeChanged();

        /*
        /// Called when a child was pressed
        virtual void                    OnPressedChild( GuiObjectPtr _spChild );

        /// Called when a child was released
        virtual void                    OnReleasedChild( GuiObjectPtr _spChild );

        /// Called when a child was clicked
        virtual void                    OnClickedChild( GuiObjectPtr _spChild );

        /// Called when a child was double clicked
        virtual void                    OnDoubleClickedChild( GuiObjectPtr _spChild );*/

    private:
        friend class PanelCreator;

        typedef std::list< GuiObjectPtr > Children;

        /// Initializes the passed object as a child of this panel
        void                            InitChild( GuiObjectPtr _spChild );

        /// Deinitializes the passed object as a child of this panel
        void                            DeinitChild( GuiObjectPtr _spChild );

        //! Returns the child at the passed point on the panel
		GuiObjectPtr                    PointToChild( const CFoundation::Vector2Di& _clPoint, CFoundation::Vector2Di& _clRelativePoint );
        
        //! Returns the child at the passed point on the panel
		const GuiObjectPtr              PointToChild( const CFoundation::Vector2Di& _clPoint, CFoundation::Vector2Di& _clRelativePoint ) const;

        /// Delegates
        PositionEventDelegate0Ptr       m_spDelegateChildPositionChanged;
        SizeEventDelegate0Ptr           m_spDelegateChildSizeChanged;

        /// Child objects
        Children                        m_aspChildren;
	};

	class PanelCreator : public GuiObjectCreator
	{
	public:
		virtual const CFoundation::String GetTypeId() const;

		virtual GuiObjectPtr    CreateObject( const CFoundation::String& _strName ) const;

        /// Initializes the passed object from a xml element
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

        /// Initializes the passed object from another
		virtual void            InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const;

        /// Saves the passed object to a xml element
		virtual bool            SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const;
	};
};

#endif
