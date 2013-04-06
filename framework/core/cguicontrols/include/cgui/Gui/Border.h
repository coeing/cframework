#ifndef CGUI_BORDER_H
#define CGUI_BORDER_H

#include "cgui/GUI/Decorator.h"

namespace CGui
{
	class Border : public Decorator
	{
	public:
		Border( const CFoundation::String &sName );
		void operator=( const Border &rhs );

		static const CFoundation::String GetTypeID();
		virtual const CFoundation::String GetTypeId() const;

        /*
		virtual void        SetCustomSize( Unsigned32 u32Width, Unsigned32 u32Height );
		virtual void        SetCustomSize( const CFoundation::Vector2Du& clSize );
		virtual void        SetWidth( Unsigned32 u32Width );
		virtual void        SetHeight( Unsigned32 u32Height );
        */

		virtual void        SetThickness( Unsigned8 u8Thickness );
		virtual Unsigned8   GetThickness() const;

		virtual void        SetReference( GuiObjectPtr _spReference );

	protected:
		virtual void        CreatePieces();
		virtual void        InitPieces();
		virtual void        UpdatePieces();

		//virtual void NewReferenceSize( Unsigned32 u32Width, Unsigned32 u32Height );

		virtual Unsigned32  GetBorderWidth() const;
		virtual Unsigned32  GetBorderHeight() const;

		virtual Unsigned32  GetLeftMargin() const;
		virtual Unsigned32  GetRightMargin() const;
		virtual Unsigned32  GetTopMargin() const;
		virtual Unsigned32  GetBottomMargin() const;

        /*
		virtual void        FitReferenceSize();
		virtual void        FitReferenceWidth();
		virtual void        FitReferenceHeight();
*/
	private:
		Unsigned8		    m_u8Thickness;
	};

	class BorderCreator : public CombinedCtrlCreator, public CFoundation::Singleton<BorderCreator>
	{
	public:
		virtual const CFoundation::String GetTypeId() const;

		virtual GuiObjectPtr    CreateObject( const CFoundation::String &sName ) const;
		
        /*
		virtual void            InitDefault( GuiObjectPtr _spObject ) const;
		virtual void            InitFromXML( GuiObjectPtr _spObject, const CFoundation::XMLElement &clXMLElement ) const;

		virtual CFoundation::XMLElement& SaveToXML( CFoundation::XMLElement &clXMLElement, const GuiNode &clNode ) const;*/

	};
}

#endif
