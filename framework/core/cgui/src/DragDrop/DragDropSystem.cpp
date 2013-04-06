#include "cgui/DragDrop/DragDropSystem.h"

#include "cgui/DragDrop/FreeDragController.h"

#include "cgui/GUI/GuiMain.h"
#include "cgui/GUI/GuiObject.h"
#include "cgui/GUI/Layer.h"
#include "cgui/GUI/Panel.h"

#include "cgui/Input/Cursor.h"

using namespace CGui;

DragDropSystem::DragDropSystem()
:   m_eResizeAnchor( CFoundation::BORDER_NONE )
{
}

DragDropSystem::~DragDropSystem()
{
}
        
bool DragDropSystem::ProcessInput( const CSystem::InputState& _state, GuiObjectPtr _spPointedObject, const CFoundation::Vector2Di& _vRelativePoint )
{
	m_clMousePosition = _state.m_vMousePosition;
    if ( m_eResizeAnchor != CFoundation::BORDER_NONE )
	{
        if ( _state.IsDown( CSystem::MOUSE_LEFT ) )
		{
			// continue resizing
			ContinueResizing( _state.GetMouseMovement() );
		}
		else
		{
			// End resizing
			EndResizing( false );
		}

        // Check if abort resizing
        if ( _state.WasPressed( CSystem::MOUSE_RIGHT ) ||
             _state.WasPressed( CSystem::KEY_ESC ) )
        {
			// End resizing
			EndResizing( true );
        }

		return true;
	}
	else if ( m_spDragController != NULL )
	{
		if ( _state.IsDown( CSystem::MOUSE_LEFT ) )
	    {            
            // Let drag controller continue dragging
            m_spDragController->Continue( _state.GetMouseMovement() );

            // Find new drop controller          
            UpdateDropController( m_clMousePosition, _spPointedObject );
		}
		else
		{
            EndDragging( false );
		}

        // Check if abort dragging
        if ( _state.WasPressed( CSystem::MOUSE_RIGHT ) ||
             _state.WasPressed( CSystem::KEY_ESC ) )
        {
			// End dragging
			EndDragging( true );
        }

        return true;
	}
	else
	{
        // Search for resize object
		GuiObjectPtr pResizeReceiver = _spPointedObject;
        CFoundation::Vector2Di vRelativePointResizeReceiver = _vRelativePoint;
		CFoundation::BorderType eBorder = CFoundation::BORDER_NONE;
		while( pResizeReceiver != NULL )
		{
			vRelativePointResizeReceiver += pResizeReceiver->GetRelativePosition();

            // Compute border size
			const CFoundation::RectI32& rectObject = pResizeReceiver->GetRelativeRect();
            static const Integer32 MAX_BORDER_SIZE = 10;
            Unsigned32 u32BorderSize = std::min( rectObject.GetHeight() / 3, std::min( rectObject.GetWidth() / 3, MAX_BORDER_SIZE ) );

            // Get border
			eBorder = rectObject.GetBordersPointIsOn( vRelativePointResizeReceiver, u32BorderSize );

			if( eBorder != CFoundation::BORDER_NONE &&
				pResizeReceiver->IsResizable( eBorder ) )
			{
				break;
			}

			pResizeReceiver = pResizeReceiver->GetParent();
		}

        // Check if resize prepared
	    if ( _state.WasPressed( CSystem::MOUSE_LEFT ) )
        {
            m_spResizeObject = pResizeReceiver;
        }
        else if ( !_state.IsDown( CSystem::MOUSE_LEFT ) )
        {
            m_spResizeObject.reset();
        }

		if ( pResizeReceiver != NULL )
		{
            // Setup cursor
			UpdateResizeCursor( eBorder );

            if ( m_spResizeObject == pResizeReceiver &&
                !_state.GetMouseMovement().IsZero() )
            {
				// Start resizing
				StartResizing( m_spResizeObject, eBorder );
				return true;
			}
		}
		else
		{
            // Reset resize cursor
		    UpdateResizeCursor( CFoundation::BORDER_NONE );

            // Search for drag object
            CGui::GuiObjectPtr pDragReceiver = _spPointedObject;
            CFoundation::Vector2Di vRelativePointDragReceiver = _vRelativePoint;
            while( pDragReceiver != NULL )
            {
                if( pDragReceiver->IsDraggable() )
                {
                    break;
                }

                vRelativePointDragReceiver += pDragReceiver->GetRelativePosition();
                pDragReceiver = pDragReceiver->GetParent();
            }

            // Check if drag prepared
		    if ( _state.WasPressed( CSystem::MOUSE_LEFT ) )
            {
                m_spDragObject = pDragReceiver;
            }
            else if ( !_state.IsDown( CSystem::MOUSE_LEFT ) )
            {
                m_spDragObject.reset();
            }
            
            if ( m_spDragObject != NULL &&
                 m_spDragObject == pDragReceiver &&
                !_state.GetMouseMovement().IsZero() )
			{                
                // Get drag controller for current drag object
                m_spDragController = GetDragController( m_spDragObject );
                
                if ( m_spDragController != NULL )
                {
                    // Let drag controller start dragging
                    m_spDragController->Start( m_spDragObject, vRelativePointDragReceiver );
                }

                // Find new drop controller          
                UpdateDropController( m_clMousePosition, _spPointedObject );

				return true;
			}
		}
	}

	return false;
}
		
void DragDropSystem::EndDragging( bool _bUndo )
{
    if ( m_spDragController == NULL )
    {
        return;
    }

    // Check where the drag object dropped in the active window
    bool bDropSuccessful = false;
    if ( m_spDropController != NULL &&
         m_spDragObject != NULL &&
         !_bUndo )
    {
        // Check if valid
        if ( m_spDragController->IsDropValid( m_spDropController->GetObject() ) )
        {
            // Compute drag position
            CFoundation::Vector2Di vDragPosition = m_clMousePosition;
            vDragPosition -= m_spDragObject->GetAbsolutePosition();

            // Compute drop point
            CFoundation::Vector2Di vDropPoint = m_clMousePosition;
            GuiObjectPtr pDropObject = m_spDropController->GetObject();
            if ( pDropObject != NULL )
            {
                vDropPoint -= pDropObject->GetAbsolutePosition();
            }

            // Execute drop action
            bDropSuccessful = m_spDropController->Execute( m_spDragObject, m_spDragController->GetDragData(), vDragPosition, vDropPoint );
        }

        // Remove drop object and controller
        m_spDropController->RemoveObject();
        m_spDropController.reset();
    }

    // We have to check again, because the dragging could be ended by the dropping
    if ( m_spDragController != NULL )
    {
        // Revert drag if drop was not successful
        if ( !bDropSuccessful )
        {
            m_spDragController->Undo();
        }

        // Let drag controller end dragging
        m_spDragController->End();

        // Reset drag controller
        m_spDragController.reset();
    }

    // Reset drag object
    m_spDragObject.reset();
}

const GuiObjectPtr DragDropSystem::GetCurrentDragObject() const
{
    return m_spDragController != NULL ? m_spDragController->GetObject() : GuiObjectPtr();
}

void DragDropSystem::UpdateResizeCursor( CFoundation::BorderType _eBorder ) const
{
	CGui::Cursor::CursorType eCursorType = CGui::Cursor::TYPE_ARROW;
    switch ( _eBorder )
    {
    case CFoundation::EDGE_TOP_LEFT:
    case CFoundation::EDGE_BOTTOM_RIGHT:
        {
		    eCursorType = CGui::Cursor::TYPE_SIZE_DIAGONAL_LR;
        }
        break;
    case CFoundation::EDGE_TOP_RIGHT:
    case CFoundation::EDGE_BOTTOM_LEFT:
        {
		    eCursorType = CGui::Cursor::TYPE_SIZE_DIAGONAL_RL;
        }
        break;
    case CFoundation::BORDER_TOP:
    case CFoundation::BORDER_BOTTOM:
        {
		    eCursorType = CGui::Cursor::TYPE_SIZE_VERTICAL;
        }
        break;
    case CFoundation::BORDER_LEFT:
    case CFoundation::BORDER_RIGHT:
        {
		    eCursorType = CGui::Cursor::TYPE_SIZE_HORIZONTAL;
        }
        break;
    }

	GuiMain::GetInstance().GetCursor()->SetType( eCursorType );
}

void DragDropSystem::StartResizing( CGui::GuiObjectPtr _spResizeObject, CFoundation::BorderType _eBorder )
{
	m_spResizeObject = _spResizeObject;
    m_vResizeInitialOffset = _spResizeObject->GetOffset();
    m_vResizeInitialSize = _spResizeObject->GetCustomSize();
	m_eResizeAnchor = _eBorder;

	UpdateResizeCursor( _eBorder );
}

void DragDropSystem::ContinueResizing( const CFoundation::Vector2Di& _vMovement )
{
	if( m_spResizeObject == NULL ||
	    !m_spResizeObject->IsResizable( m_eResizeAnchor ) )
    {
        return;
    }

    // Resize object
	Integer32 i32NewWidth = m_spResizeObject->GetWidth();
	Integer32 i32NewHeight = m_spResizeObject->GetHeight();
	CFoundation::Vector2Di clNewOffset = m_spResizeObject->GetOffset();
	bool bWidthChanged = false;
	bool bHeightChanged = false;
	bool bOffsetChanged = false;

	if ( m_eResizeAnchor & CFoundation::BORDER_LEFT )
	{
        clNewOffset.AddX( std::min( i32NewWidth - 1, _vMovement.GetX() ) );
		bOffsetChanged = true;
		i32NewWidth -= _vMovement.GetX();
		bWidthChanged = true;
	}
	else if ( m_eResizeAnchor & CFoundation::BORDER_RIGHT )
	{
		i32NewWidth += _vMovement.GetX();
		bWidthChanged = true;
	}

	if ( m_eResizeAnchor & CFoundation::BORDER_TOP )
	{
		clNewOffset.AddY( std::min( i32NewHeight - 1, _vMovement.GetY() ) );
		bOffsetChanged = true;
		i32NewHeight -= _vMovement.GetY();
		bHeightChanged = true;
	}
	else if ( m_eResizeAnchor & CFoundation::BORDER_BOTTOM )
	{
		i32NewHeight += _vMovement.GetY();
		bHeightChanged = true;
	}

	i32NewWidth = std::max( i32NewWidth, 1 );
	i32NewHeight = std::max( i32NewHeight, 1 );

	if ( bWidthChanged &&
		 bHeightChanged )
	{
        m_spResizeObject->SetCustomSize( CFoundation::Vector2Di( i32NewWidth, i32NewHeight ) );
	}
	else if ( bWidthChanged )
	{
		m_spResizeObject->SetCustomWidth( i32NewWidth );
	}
	else if ( bHeightChanged )
	{
		m_spResizeObject->SetCustomHeight( i32NewHeight );
	}

	if( bOffsetChanged )
	{
		m_spResizeObject->SetOffset( clNewOffset );
	}
}

void DragDropSystem::EndResizing( bool _bUndo )
{
    if ( m_spResizeObject == NULL )
    {
        return;
    }

    // Undo changes if wanted
    if ( _bUndo )
    {
        m_spResizeObject->SetOffset( m_vResizeInitialOffset );
        m_spResizeObject->SetCustomSize( m_vResizeInitialSize );
    }

	m_spResizeObject.reset();
    m_eResizeAnchor = CFoundation::BORDER_NONE;
}

CGui::GuiObjectPtr DragDropSystem::GetCurrentResizeObject() const
{
	return m_spResizeObject;
}

void DragDropSystem::SetDragController( CGui::GuiObjectCPtr _spObject, DragControllerPtr _spDragController )
{
    m_mapDragControllers[ _spObject ] = _spDragController;
}

void DragDropSystem::RemoveDragController( CGui::GuiObjectCPtr _spObject )
{
    ObjectDragControllerMap::iterator it = m_mapDragControllers.find( _spObject );
    if ( it != m_mapDragControllers.end() )
    {
        m_mapDragControllers.erase( it );
    }
}

DragControllerPtr DragDropSystem::GetDragController( CGui::GuiObjectCPtr _spObject ) const
{
    ObjectDragControllerMap::const_iterator it = m_mapDragControllers.find( _spObject );
    if ( it != m_mapDragControllers.end() )
    {
        return it->second;
    }
    else
    {
        // Use default drag controller if none is assigned
        return FreeDragControllerPtr( new FreeDragController );
    }
}

void DragDropSystem::SetDropController( CGui::GuiObjectCPtr _spObject, DropControllerPtr _spDropController )
{
    m_mapDropControllers[ _spObject ] = _spDropController;
}

void DragDropSystem::RemoveDropController( CGui::GuiObjectCPtr _spObject )
{
    ObjectDropControllerMap::iterator it = m_mapDropControllers.find( _spObject );
    if ( it != m_mapDropControllers.end() )
    {
        m_mapDropControllers.erase( it );
    }
}

DropControllerPtr DragDropSystem::GetDropController( CGui::GuiObjectCPtr _spObject ) const
{
    ObjectDropControllerMap::const_iterator it = m_mapDropControllers.find( _spObject );
    if ( it != m_mapDropControllers.end() )
    {
        return it->second;
    }
    else
    {
        return DropControllerPtr();
    }
}

void DragDropSystem::UpdateDropController( const CFoundation::Vector2Di& /*_vMousePosition*/, GuiObjectPtr _spPointedObject )
{
    // Find new drop controller  
    GuiObjectPtr pNewDropObject = _spPointedObject;

    DropControllerPtr pNewDropController;
    while ( pNewDropObject != NULL )
    {
        // Get drop controller
        pNewDropController = GetDropController( pNewDropObject );
        if ( pNewDropController != NULL &&
             pNewDropController->IsEnabled() &&
             ( m_spDragController == NULL ||
               m_spDragController->IsDropValid( pNewDropObject ) ) )
        {
            break;
        }

        pNewDropObject = pNewDropObject->GetParent();
    }

    // Update drop controller  
    if ( m_spDropController != pNewDropController )
    {
        if ( m_spDropController != NULL )
        {
            m_spDropController->RemoveObject();
        }

        m_spDropController = pNewDropController;
    }

    if ( m_spDropController != NULL )
    {
        if ( pNewDropObject != NULL )
        {
            m_spDropController->SetObject( pNewDropObject );
        }
        else
        {
            m_spDropController->RemoveObject();
        }
    }
}
