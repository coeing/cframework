#ifndef CGUI_DECORATOR_H
#define CGUI_DECORATOR_H

#include "cgui/GUI/CombinedCtrl.h"

namespace CGui
{
	class Decorator : public CombinedCtrl
	{
	public:
		Decorator( const CFoundation::String &sName, GuiObject *pReference = NULL );

        /// Clears the object
		virtual void                        Clear();

		//virtual void SetSize( Unsigned32 u32Width, Unsigned32 u32Height );

		virtual void SetResizable( bool bResizable );
		virtual void SetDraggable( bool bDraggable );


        /// Sets the reference object
		virtual void                        SetReference( GuiObjectPtr _spReference );

        /// Returns the reference object
		virtual GuiObjectPtr                GetReference() const;

        /// Indicates if the decorator has a reference
		virtual bool                        HasReference() const;

	protected:
		//virtual void NewReferenceParent( GuiObject *pParent );

		virtual void            NewReferencePosition( const CFoundation::Vector2Di &clPosition );
		virtual void            SetReferencePosition( const CFoundation::Vector2Di &clPosition );

		virtual void            NewReferenceSize( Unsigned32 u32Width, Unsigned32 u32Height );
		virtual void            SetReferenceSize( Unsigned32 u32Width, Unsigned32 u32Height );

		virtual void            FitReference();
		virtual void            ReleaseReference();

		virtual Unsigned32      GetLeftMargin() const;
		virtual Unsigned32      GetRightMargin() const;
		virtual Unsigned32      GetTopMargin() const;
		virtual Unsigned32      GetBottomMargin() const;

        /// Callback after reference object was rendered
        virtual void            OnPostRenderReference( GuiObjectPtr _spObject, const RenderParams& _Params, std::vector< RenderElementPtr >& _aspRenderElements );

    private:
        /// Delegates
        RenderEventDelegate3Ptr m_spDelegatePostRenderReference;

        /// Reference object
		GuiObjectPtr    m_spReference;

		Unsigned32		m_u32ParentChange;

		Unsigned32		m_u32PositionChange;
		Unsigned32		m_u32SizeChange;

		Unsigned32		m_u32ResizabilityChange;
		Unsigned32		m_u32DraggabilityChange;
	};

	class DecoratorCreator : public CombinedCtrlCreator
	{
	public:
		//virtual void    InitDefault( GuiObjectPtr _spObject ) const;
		//virtual void    InitFromXML( GuiObjectPtr _spObject, const CFoundation::XMLElement &clXMLElement ) const;

		//virtual CFoundation::XMLElement& SaveToXML( const GuiObject &clObject, CFoundation::XMLDocument &clDoc, CFoundation::XMLElement &clXMLElement ) const;
	};
}

#endif
