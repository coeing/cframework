#include "cctrl/Controls/ButtonGroup.h"

#include "cfoundation/Debug/Assert.h"

#include "cgui/Gui/Button.h"

using namespace CCtrl;

ButtonGroup::ButtonGroup()
:   m_i32ActiveButton( -1 )
{
    // Create delegates
    m_spDelegateButtonActivated = CGui::StateEventDelegate1Ptr( new CGui::StateEventDelegate1( this, &ButtonGroup::OnButtonActivated ) );
    m_spDelegateButtonDeactivated = CGui::StateEventDelegate1Ptr( new CGui::StateEventDelegate1( this, &ButtonGroup::OnButtonDeactivated ) );
}

void ButtonGroup::ProcessMessage( const CFoundation::Message& _clMessage )
{
}

bool ButtonGroup::AddButton( CGui::ButtonPtr _spButton )
{
    if ( _spButton == NULL )
    {
        return false;
    }

    if ( std::find( m_aspButtons.begin(), m_aspButtons.end(), _spButton ) != m_aspButtons.end() )
    {
        CF_WARN( "ButtonGroup::AddButton: Tried to add already added button." );
        return false;
    }

    m_aspButtons.push_back( _spButton );

    // Register delegates
    _spButton->RegisterDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateButtonActivated );
    _spButton->RegisterDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateButtonDeactivated );

    // Set as active button if none at the moment
    if ( GetActiveButton() == NULL )
    {
        SetActiveButton( _spButton );
    }
    else
    {
        _spButton->SetActive( false );
    }
       
    return true;
}

void ButtonGroup::RemoveButton( CGui::ButtonPtr _spButton )
{
    ButtonVec::iterator it = std::find( m_aspButtons.begin(), m_aspButtons.end(), _spButton );
    if ( it == m_aspButtons.end() )
    {
        CF_WARN( "ButtonGroup::AddButton: Tried to remove not added button." );
        return;
    }

    it = m_aspButtons.erase( it );
    
    // Check if active button
    if ( GetActiveButton() == _spButton )
    {
        if ( m_aspButtons.size() > 0 ) 
        {
            SetActiveButton( *m_aspButtons.begin() );
        }
        else
        {
            SetActiveButton( -1 );
        }
    }

    // Stop listening to state changes
    _spButton->RemoveDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateButtonActivated );
    _spButton->RemoveDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateButtonDeactivated );
}
        
void ButtonGroup::RemoveAllButtons()
{
    for ( ButtonVec::iterator it = m_aspButtons.begin(); it != m_aspButtons.end(); ++it )
    {
        CGui::ButtonPtr spButton = *it;

        // Stop listening to state changes
        spButton->RemoveDelegate( CGui::STATE_EVENT_ACTIVATED, m_spDelegateButtonActivated );
        spButton->RemoveDelegate( CGui::STATE_EVENT_DEACTIVATED, m_spDelegateButtonDeactivated );
    }

    // Clear buttons
    m_aspButtons.clear();
    SetActiveButton( -1 );
}

Unsigned32 ButtonGroup::GetNumButtons() const
{
    return m_aspButtons.size();
}
        
CGui::ButtonPtr ButtonGroup::GetButton( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < m_aspButtons.size() )
    {
        return m_aspButtons[ _u32Idx ];
    }
    else
    {
        return CGui::ButtonPtr();
    }
}

void ButtonGroup::SetActiveButton( CGui::ButtonPtr _spButton )
{
    // Check if in this button group
    ButtonVec::const_iterator it = std::find( m_aspButtons.begin(), m_aspButtons.end(), _spButton );
    if ( it == m_aspButtons.end() )
    {
        return;
    }

    // Get index
    Unsigned32 u32Idx = it - m_aspButtons.begin();

    // Set active index
    SetActiveButton( u32Idx );
}
        
void ButtonGroup::SetActiveButton( Integer32 _i32Idx )
{
    if ( _i32Idx == m_i32ActiveButton )
    {
        return;
    }

    CGui::ButtonPtr spOldActiveButton = GetActiveButton();

    // Store new active button
    m_i32ActiveButton = _i32Idx;

    // Deactivate all buttons
    Unsigned32 u32NumButtons = m_aspButtons.size();
    for ( Unsigned32 u32Idx = 0; u32Idx < u32NumButtons; ++u32Idx )
    {
        CGui::ButtonPtr spButton = m_aspButtons[ u32Idx ];
        if ( m_i32ActiveButton >= 0 &&
             u32Idx == static_cast< Unsigned32 >( m_i32ActiveButton ) )
        {
            spButton->SetActive( true );
        }
        else
        {
            spButton->SetActive( false );
        }
    }

    // Dispatch event
    DispatchEvent( BUTTON_GROUP_EVENT_SELECTION_CHANGED, GetSharedPtr< ButtonGroup >(), GetActiveButton(), spOldActiveButton );
}
       
CGui::ButtonPtr ButtonGroup::GetActiveButton() const
{
    if ( m_i32ActiveButton >= 0 &&
         static_cast< Unsigned32 >( m_i32ActiveButton ) < m_aspButtons.size() )
    {
        return m_aspButtons[ m_i32ActiveButton ];
    }
    else
    {
        return CGui::ButtonPtr();
    }
}

Integer32 ButtonGroup::GetActiveButtonIdx() const
{
    return m_i32ActiveButton;
}
        
void ButtonGroup::OnButtonActivated( CGui::GuiObjectPtr _spObject )
{
    SetActiveButton( static_pointer_cast< CGui::Button >( _spObject ) );
}
        
void ButtonGroup::OnButtonDeactivated( CGui::GuiObjectPtr _spObject )
{
    if ( _spObject == GetActiveButton() )
    {
        // Activated button may not be deactivated
        _spObject->SetActive( true );
    }
}
