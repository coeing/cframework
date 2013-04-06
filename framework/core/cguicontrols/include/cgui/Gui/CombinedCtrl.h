#ifndef CGUI_COMBINEDCTRL_H
#define CGUI_COMBINEDCTRL_H

#include <vector>

#include "cfoundation/Utils/SharedPtr.h"

#include "cgui/GUI/Panel.h"

namespace CGui
{
    class CombinedCtrl : public Panel
	{
	public:
        /// Clears the object
		virtual void                        Clear();

        /// Sets the passed state flag
        virtual void                        SetStateFlag( StateFlag _eStateFlag, bool _bSet );

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

        /// Called when the object gained the focus
        virtual void                        OnGainedFocus();

        /// Called when the object lost the focus
        virtual void                        OnLostFocus();

	protected:
		CombinedCtrl( const CFoundation::String &sName );
		~CombinedCtrl();

		void                                AddPieceFirst( GuiObjectPtr _spObject );
		void                                AddPieceLast( GuiObjectPtr _spObject ); 

        /// Removes a piece
		void                                RemovePiece( GuiObjectPtr _spObject );

		GuiObjectPtr                        GetPiece( const CFoundation::String &sName );
		const GuiObjectPtr                  GetPiece( const CFoundation::String &sName ) const;

		GuiObjectPtr                        GetPiece( Unsigned32 u32Idx );
		const GuiObjectPtr                  GetPiece( Unsigned32 u32Idx ) const;

		bool                                IsPiece( GuiObjectCPtr _spObject ) const;

		Unsigned32                          GetNumPieces() const;

        /// Called when a piece was pressed
        virtual void                        OnPressedPiece( GuiObjectPtr _spPiece );

        /// Called when a piece was released
        virtual void                        OnReleasedPiece( GuiObjectPtr _spPiece );

        /// Called when a piece was clicked
        virtual void                        OnClickedPiece( GuiObjectPtr _spPiece );

        /// Called when a piece was double clicked
        virtual void                        OnDoubleClickedPiece( GuiObjectPtr _spPiece );

        /// Called when a piece gained focus
        virtual void                        OnPieceGainedFocus( GuiObjectPtr _spPiece );

        /// Called when a piece lost focus
        virtual void                        OnPieceLostFocus( GuiObjectPtr _spPiece );

	private:
		friend class CombinedCtrlCreator;

        typedef std::deque< GuiObjectPtr > Pieces;

        /// Inits a new piece (e.g. set it depenedent,...)
		void                                InitPiece( GuiObjectPtr _spObject );

        /// Deinits a removed piece
		void                                DeinitPiece( GuiObjectPtr _spObject );

        /// Delegate when piece was pressed
        ObjectInputEventDelegate1Ptr        m_spDelegatePressedPiece;

        /// Delegate when piece was released
        ObjectInputEventDelegate1Ptr        m_spDelegateReleasedPiece;

        /// Delegate when piece was clicked
        ObjectInputEventDelegate1Ptr        m_spDelegateClickedPiece;

        /// Delegate when piece was double clicked
        ObjectInputEventDelegate1Ptr        m_spDelegateDoubleClickedPiece;

        /// Delegate when piece gained focus
        ObjectInputEventDelegate1Ptr        m_spDelegatePieceGainedFocus;

        /// Delegate when piece lost focus
        ObjectInputEventDelegate1Ptr        m_spDelegatePieceLostFocus;

		//! A piece is a child which is dependent of its parent
		Pieces				                m_vPieces;
	};

    INCLUDE_SHARED_CLASS( CombinedCtrl );

	class CombinedCtrlCreator : public PanelCreator
	{
	public:
		virtual void    InitDefault( GuiObjectPtr _spObject ) const;

        /// Initializes the passed object from a xml element
		virtual void    InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

        /// Saves the passed object to a xml element
		virtual bool    SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const;
	};
}

#endif
