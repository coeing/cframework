#include "cgui/GUI/CombinedCtrl.h"

#include <algorithm>

#include "cfoundation/Debug/Makros.h"
#include "cfoundation/Events/MessageSystem.h"

#include "cgui/Messages/FocusMsg.h"
#include "cgui/Messages/KeyboardEventMsg.h"
#include "cgui/Messages/KeyboardMsg.h"

using namespace CGui;

CombinedCtrl::CombinedCtrl( const CFoundation::String &sName )
:	Panel( sName )
{
    // Create delegates
    m_spDelegatePressedPiece = ObjectInputEventDelegate1Ptr( new ObjectInputEventDelegate1( this, &CombinedCtrl::OnPressedPiece ) );
    m_spDelegateReleasedPiece = ObjectInputEventDelegate1Ptr( new ObjectInputEventDelegate1( this, &CombinedCtrl::OnReleasedPiece ) );
    m_spDelegateClickedPiece = ObjectInputEventDelegate1Ptr( new ObjectInputEventDelegate1( this, &CombinedCtrl::OnClickedPiece ) );
    m_spDelegateDoubleClickedPiece = ObjectInputEventDelegate1Ptr( new ObjectInputEventDelegate1( this, &CombinedCtrl::OnDoubleClickedPiece ) );
    m_spDelegatePieceGainedFocus = ObjectInputEventDelegate1Ptr( new ObjectInputEventDelegate1( this, &CombinedCtrl::OnPieceGainedFocus ) );
    m_spDelegatePieceLostFocus = ObjectInputEventDelegate1Ptr( new ObjectInputEventDelegate1( this, &CombinedCtrl::OnPieceLostFocus ) );
}

CombinedCtrl::~CombinedCtrl()
{
}
		
void CombinedCtrl::Clear()
{
    // Remove pieces
    Pieces aspPieces = m_vPieces;
    for ( Pieces::iterator it = aspPieces.begin(); it != aspPieces.end(); ++it )
    {
        GuiObjectPtr spPiece = *it;
        spPiece->Clear();
        RemovePiece( spPiece );
    }

    Panel::Clear();
}

void CombinedCtrl::SetStateFlag( StateFlag _eStateFlag, bool _bSet )
{
    if ( IsStateFlagSet( _eStateFlag ) == _bSet )
    {
        return;
    }

    // Set state of pieces
    for ( Unsigned32 u32PieceIdx = 0; u32PieceIdx != GetNumPieces(); ++u32PieceIdx )
    {
        GuiObjectPtr spPiece = GetPiece( u32PieceIdx );
        spPiece->SetStateFlag( _eStateFlag, _bSet );
    }

    Panel::SetStateFlag( _eStateFlag, _bSet );
}
        
bool CombinedCtrl::ProcessInputFocused( const CSystem::InputState& _State )
{
    bool bProcessed = Panel::ProcessInputFocused( _State );
    return bProcessed;
}

bool CombinedCtrl::ProcessInputPointed( const CSystem::InputState& _State )
{
    bool bProcessed = Panel::ProcessInputPointed( _State );
    return bProcessed;
}
        
void CombinedCtrl::OnGainedFocus()
{
    if ( IsStateFlagSet( STATE_FLAG_FOCUSED ) )
    {
        return;
    }

    Panel::OnGainedFocus();

    // Gain focus for pieces
    Unsigned32 u32NumPieces = m_vPieces.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPieces; ++u32Idx )
    {
        GuiObjectPtr spPiece = m_vPieces[ u32Idx ];
        spPiece->OnGainedFocus();
    }
}

void CombinedCtrl::OnLostFocus()
{
    if ( !IsStateFlagSet( STATE_FLAG_FOCUSED ) )
    {
        return;
    }

    Panel::OnLostFocus();

    // Gain focus for pieces
    Unsigned32 u32NumPieces = m_vPieces.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumPieces; ++u32Idx )
    {
        GuiObjectPtr spPiece = m_vPieces[ u32Idx ];
        spPiece->OnLostFocus();
    }
}

void CombinedCtrl::AddPieceFirst( GuiObjectPtr _spObject )
{
	if ( !IsPiece( _spObject ) )
	{
		AddChildFirst( _spObject );

		m_vPieces.push_front( _spObject );

        // Setup the piece
        InitPiece( _spObject );
	}
}

void CombinedCtrl::AddPieceLast( GuiObjectPtr _spObject )
{
	if ( !IsPiece( _spObject ) )
	{
		AddChildLast( _spObject );

		m_vPieces.push_back( _spObject );

        // Setup the piece
        InitPiece( _spObject );
	}
}

void CombinedCtrl::RemovePiece( GuiObjectPtr _spChild )
{
    Pieces::iterator it = std::find( m_vPieces.begin(), m_vPieces.end(), _spChild );
    if ( it == m_vPieces.end() )
    {
        return;
    }

    // Deinit piece
    DeinitPiece( _spChild );

    m_vPieces.erase( it );

    RemoveChild( _spChild );
}

void CombinedCtrl::InitPiece( GuiObjectPtr _spObject )
{
	// Set dependent
	_spObject->SetIndependent( false );

	// set not draggable
	_spObject->SetDraggable( false );

	// set not resizable
	//_spObject->SetResizableAnchor( CFoundation::BORDER_NONE );

    // Set same state flags
	_spObject->SetStateFlags( GetStateFlags() );

    // Add delegates for input events
    _spObject->RegisterDelegate( INPUT_EVENT_PRESSED, m_spDelegatePressedPiece );
    _spObject->RegisterDelegate( INPUT_EVENT_RELEASED, m_spDelegateReleasedPiece );
    _spObject->RegisterDelegate( INPUT_EVENT_CLICKED, m_spDelegateClickedPiece );
    _spObject->RegisterDelegate( INPUT_EVENT_DOUBLE_CLICKED, m_spDelegateDoubleClickedPiece );
    _spObject->RegisterDelegate( INPUT_EVENT_GAINED_FOCUS, m_spDelegatePieceGainedFocus );
    _spObject->RegisterDelegate( INPUT_EVENT_LOST_FOCUS, m_spDelegatePieceLostFocus );
}

void CombinedCtrl::DeinitPiece( GuiObjectPtr _spObject )
{
    // Remove delegates for input events
    _spObject->RemoveDelegate( INPUT_EVENT_PRESSED, m_spDelegatePressedPiece );
    _spObject->RemoveDelegate( INPUT_EVENT_RELEASED, m_spDelegateReleasedPiece );
    _spObject->RemoveDelegate( INPUT_EVENT_CLICKED, m_spDelegateClickedPiece );
    _spObject->RemoveDelegate( INPUT_EVENT_DOUBLE_CLICKED, m_spDelegateDoubleClickedPiece );
    _spObject->RemoveDelegate( INPUT_EVENT_GAINED_FOCUS, m_spDelegatePieceGainedFocus );
    _spObject->RemoveDelegate( INPUT_EVENT_LOST_FOCUS, m_spDelegatePieceLostFocus );

    // Set independent
    _spObject->SetIndependent( true );
}

void CombinedCtrl::OnPressedPiece( GuiObjectPtr _spPiece )
{
    OnPressed();
}

void CombinedCtrl::OnReleasedPiece( GuiObjectPtr _spPiece )
{
    OnReleased();
}

void CombinedCtrl::OnClickedPiece( GuiObjectPtr _spPiece )
{
    OnClicked();
}

void CombinedCtrl::OnDoubleClickedPiece( GuiObjectPtr _spPiece )
{
    OnDoubleClicked();
}

void CombinedCtrl::OnPieceGainedFocus( GuiObjectPtr _spPiece )
{
    OnGainedFocus();
}

void CombinedCtrl::OnPieceLostFocus( GuiObjectPtr _spPiece )
{
    OnLostFocus();
}

GuiObjectPtr CombinedCtrl::GetPiece( const CFoundation::String &sName )
{
	for( Pieces::const_iterator it = m_vPieces.begin(); it != m_vPieces.end(); ++it )
	{
		GuiObjectPtr spObject = *it;
		if( spObject->GetName() == sName )
		{
			return spObject;
		}
	}

	return GuiObjectPtr();
}

const GuiObjectPtr CombinedCtrl::GetPiece( const CFoundation::String &sName ) const
{
	return const_cast<CombinedCtrl*>( this )->GetPiece( sName );
}

GuiObjectPtr CombinedCtrl::GetPiece( Unsigned32 u32Idx )
{
	if( u32Idx < GetNumPieces() )
	{
		return m_vPieces[ u32Idx ];
	}
	else
	{
		return GuiObjectPtr();
	}
}

const GuiObjectPtr CombinedCtrl::GetPiece( Unsigned32 u32Idx ) const
{
	return const_cast<CombinedCtrl*>( this )->GetPiece( u32Idx );
}


bool CombinedCtrl::IsPiece( GuiObjectCPtr _spObject ) const
{
    Pieces::const_iterator it = std::find( m_vPieces.begin(), m_vPieces.end(), _spObject );
    return it != m_vPieces.end();
}

Unsigned32 CombinedCtrl::GetNumPieces() const
{
	return m_vPieces.size();
}

void CombinedCtrlCreator::InitDefault( GuiObjectPtr _spObject ) const
{
	// Init control itself
	PanelCreator::InitDefault( _spObject );
}

void CombinedCtrlCreator::InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const
{
	// Init control itself
	PanelCreator::InitFromXML( _spObject, _spXMLElement );

    // Add children as pieces
	CombinedCtrlPtr spCombinedCtrl = static_pointer_cast< CombinedCtrl >( _spObject );
    Unsigned32 u32NumChildren = spCombinedCtrl->GetNumChildren();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumChildren; ++u32Idx )
    {
        GuiObjectPtr spChild = spCombinedCtrl->GetChild( u32Idx );
        spCombinedCtrl->AddPieceLast( spChild );
    }
}

bool CombinedCtrlCreator::SaveToXML( GuiObjectCPtr _spObject, CFoundation::XMLDocumentPtr _spDoc, CFoundation::XMLElementPtr _spXMLElement ) const
{
	return PanelCreator::SaveToXML( _spObject, _spDoc, _spXMLElement );
}
