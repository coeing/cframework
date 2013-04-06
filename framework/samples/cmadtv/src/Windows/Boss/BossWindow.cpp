#include "cmadtv/Windows/Boss/BossWindow.h"

#include "cgui/Gui/Button.h"
#include "cgui/Gui/TextBox.h"

#include "cmadtv/Data/Game/Game.h"
#include "cmadtv/Data/Player/Player.h"

using namespace CMadTV;

namespace CMadTV
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BossWindowRequest
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BossWindowRequest
    {
    public:
        class Choice;

        /// Constructor
        BossWindowRequest();

        /// Sets the player
        void                SetPlayer( PlayerPtr _spPlayer );

        /// Sets the boss question
        void                SetQuestion( const CFoundation::String& _strQuestion );

        /// Returns the boss question
        const CFoundation::String&  GetQuestion() const;

        /// Returns the number of choices
        Unsigned32          GetNumChoices() const;

        /// Returns the choice with the passed index
        CFoundation::String GetChoice( Unsigned32 _u32Idx ) const;

        /// Selects the choice with the passed index
        /// Returns the following request
        virtual BossWindowRequestPtr    SelectChoice( Unsigned32 _u32Idx ) const;

    protected:
        /// Collects the choices for this request
        virtual void                    CollectChoices();

        /// Adds a choice text
        void                AddChoice( const CFoundation::String& _strChoice );

        /// Player
        PlayerPtr           m_spPlayer;

        /// Question
        CFoundation::String m_strQuestion;

        /// Choices
        std::vector< CFoundation::String >  m_astrChoices;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BossWindowMainRequest
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BossWindowMainRequest : public BossWindowRequest
    {
    public:
        /// Selects the choice with the passed index
        /// Returns the following request
        virtual BossWindowRequestPtr    SelectChoice( Unsigned32 _u32Idx ) const;

    protected:
        /// Collects the choices for this request
        virtual void                    CollectChoices();
    };
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BossWindow
///
////////////////////////////////////////////////////////////////////////////////////////////
        
CFoundation::String BossWindow::GetXMLFileName()
{
    return "data/windows/BossWindow.xml";
}

BossWindow::BossWindow()
:   m_u32Choice( MAX_U32 )
{
    // Create delegates
    m_spDelegatePrevChoicePushed = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &BossWindow::OnPrevChoicePushed ) );
    m_spDelegateNextChoicePushed = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &BossWindow::OnNextChoicePushed ) );
    m_spDelegateChoiceClicked = CGui::ObjectInputEventDelegate0Ptr( new CGui::ObjectInputEventDelegate0( this, &BossWindow::OnChoiceClicked ) );
}

BossWindow::~BossWindow()
{
}

void BossWindow::InitWindow()
{
    m_spTbBoss = GetTextBox( "TbBoss" );

    // Get scroll buttons
    m_spBtPrevChoice = GetButton( "BtPrevChoice" );
    //m_spBtPrevChoice->SetVisible( false );
    m_spBtPrevChoice->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegatePrevChoicePushed );
    m_spBtNextChoice = GetButton( "BtNextChoice" );
    //m_spBtNextChoice->SetVisible( false );
    m_spBtNextChoice->RegisterDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateNextChoicePushed );

    m_spImgPlayerShadow = GetGuiObject( "ImgPlayerShadow" );
    m_spPnlPlayer = GetPanel( "PnlPlayer" );
    m_spTbPlayer = GetTextBox( "TbPlayer" );
    m_spTbPlayer->RegisterDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateChoiceClicked );

    WindowController::InitWindow();

    // Create main request
    SetRequest( BossWindowRequestPtr( new BossWindowMainRequest() ) );
}

void BossWindow::DeinitWindow()
{
    // Remove delegates
    m_spBtPrevChoice->RemoveDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegatePrevChoicePushed );
    m_spBtNextChoice->RemoveDelegate( CGui::INPUT_EVENT_PUSHED, m_spDelegateNextChoicePushed );
    m_spTbPlayer->RemoveDelegate( CGui::INPUT_EVENT_CLICKED, m_spDelegateChoiceClicked );

    WindowController::DeinitWindow();
}
       
void BossWindow::SetPlayer( PlayerPtr _spPlayer )
{
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    m_spPlayer = _spPlayer;

    // TODO Check boss welcome text depending on player's image
    if ( m_spPlayer != NULL )
    {
    }

    // Update request
    if ( m_spRequest != NULL )
    {
        m_spRequest->SetPlayer( m_spPlayer );

        // Update controls
        m_spTbBoss->SetText( m_spRequest->GetQuestion() );
        ShowPlayerPanel( m_spRequest->GetNumChoices() > 0 );
        m_spTbPlayer->SetText( m_spRequest->GetChoice( m_u32Choice ) );
    }
}
     
void BossWindow::SetRequest( BossWindowRequestPtr _spRequest )
{
    if ( _spRequest == m_spRequest )
    {
        return;
    }

    m_spRequest = _spRequest;
    
    if ( m_spRequest != NULL )
    {
        // Initialize request
        m_spRequest->SetPlayer( m_spPlayer );

        m_u32Choice = MAX_U32;

        // Set boss question
        m_spTbBoss->SetText( m_spRequest->GetQuestion() );

        // Set first choice
        SetChoice( 0 );

        // Hide player panel if no choices
        ShowPlayerPanel( m_spRequest->GetNumChoices() > 0 );
    }
    else
    {
        ShowPlayerPanel( false );
    }
}
       
void BossWindow::SetChoice( Unsigned32 _u32Choice )
{
    if ( _u32Choice == m_u32Choice )
    {
        return;
    }

    m_u32Choice = _u32Choice;

    if ( m_spRequest != NULL )
    {
        m_spTbPlayer->SetText( m_spRequest->GetChoice( m_u32Choice ) );
    }
}
        
void BossWindow::ShowPlayerPanel( bool _bShow )
{
    m_spImgPlayerShadow->SetVisible( _bShow );
    m_spPnlPlayer->SetVisible( _bShow );
}

void BossWindow::OnPrevChoicePushed()
{
    if ( m_spRequest == NULL )
    {
        return;
    }

    if ( m_u32Choice > 0 )
    {
        SetChoice( m_u32Choice - 1 );
    }
    else if ( m_spRequest->GetNumChoices() > 0 )
    {
        SetChoice( m_spRequest->GetNumChoices() - 1 );
    }
}
       
void BossWindow::OnNextChoicePushed()
{
    if ( m_spRequest == NULL )
    {
        return;
    }

    if ( m_spRequest->GetNumChoices() > 0 &&
         m_u32Choice < m_spRequest->GetNumChoices() - 1 )
    {
        SetChoice( m_u32Choice + 1 );
    }
    else
    {
        SetChoice( 0 );
    }
}

void BossWindow::OnChoiceClicked()
{
    if ( m_spRequest != NULL )
    {
        SetRequest( m_spRequest->SelectChoice( m_u32Choice ) );
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BossWindowRequest
///
////////////////////////////////////////////////////////////////////////////////////////////
        
BossWindowRequest::BossWindowRequest()
{
}
        
void BossWindowRequest::SetPlayer( PlayerPtr _spPlayer )
{ 
    if ( _spPlayer == m_spPlayer )
    {
        return;
    }

    m_spPlayer = _spPlayer;

    // Collect choices
    m_astrChoices.clear();
    CollectChoices();
}
       
void BossWindowRequest::SetQuestion( const CFoundation::String& _strQuestion )
{
    m_strQuestion = _strQuestion;
}

const CFoundation::String& BossWindowRequest::GetQuestion() const
{
    return m_strQuestion;
}

Unsigned32 BossWindowRequest::GetNumChoices() const
{
    return m_astrChoices.size();
}

CFoundation::String BossWindowRequest::GetChoice( Unsigned32 _u32Idx ) const
{
    if ( _u32Idx < m_astrChoices.size() )
    {
        return m_astrChoices[ _u32Idx ];
    }
    else
    {
        return "$missing$";
    }
}
        
BossWindowRequestPtr BossWindowRequest::SelectChoice( Unsigned32 _u32Idx ) const
{
    return BossWindowRequestPtr();
}
        
void BossWindowRequest::CollectChoices()
{
}

void BossWindowRequest::AddChoice( const CFoundation::String& _strChoice )
{
    m_astrChoices.push_back( _strChoice );
}

namespace CMadTV
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BossWindowResignationRequest
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BossWindowAvoidResignationRequest : public BossWindowRequest
    {
    public:
        /// Selects the choice with the passed index
        /// Returns the following request
        virtual BossWindowRequestPtr    SelectChoice( Unsigned32 _u32Idx ) const;

    protected:
        /// Collects the choices for this request
        virtual void                    CollectChoices();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BossWindowResignateRequest
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BossWindowResignateRequest : public BossWindowRequest
    {
    public:
        /// Selects the choice with the passed index
        /// Returns the following request
        virtual BossWindowRequestPtr    SelectChoice( Unsigned32 _u32Idx ) const;

    protected:
        /// Collects the choices for this request
        virtual void                    CollectChoices();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BossWindowNeedCreditRequest
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BossWindowNeedCreditRequest : public BossWindowRequest
    {
    public:
        /// Selects the choice with the passed index
        /// Returns the following request
        virtual BossWindowRequestPtr    SelectChoice( Unsigned32 _u32Idx ) const;

    protected:
        /// Collects the choices for this request
        virtual void                    CollectChoices();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// BossWindowRepayCreditRequest
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    class BossWindowRepayCreditRequest : public BossWindowRequest
    {
    public:
        /// Selects the choice with the passed index
        /// Returns the following request
        virtual BossWindowRequestPtr    SelectChoice( Unsigned32 _u32Idx ) const;

    protected:
        /// Collects the choices for this request
        virtual void                    CollectChoices();

    private:
        /// Returns the repay amount
        CFoundation::Money              GetCreditRepay( Float32 _f32Percent ) const;
    };
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BossWindowMainRequest
///
////////////////////////////////////////////////////////////////////////////////////////////
     
void BossWindowMainRequest::CollectChoices()
{
    // Get player image
    Unsigned8 u8Image = m_spPlayer->GetImage();

    // Set question
    CFoundation::String strQuestion;
    if ( u8Image < 20 )
    {
        strQuestion = "TODO Image <= 20";
    }
    else if ( u8Image < 75 )
    {
        strQuestion = CFoundation::String::Build( "Kommen Sie zur Sache, %s! Zeit ist Geld!", m_spPlayer->GetName().c_str() );
    }
    else if ( u8Image < 90 )
    {
        strQuestion = CFoundation::String::Build( "%s, wie geht es Ihnen, kann ich Ihnen helfen?", m_spPlayer->GetName().c_str() );
    }
    else
    {
        strQuestion = "TODO Image > 70";
    }
    SetQuestion( strQuestion );

    // Add choices
    AddChoice( "Mir reichts. Ich kündige." );
    AddChoice( "Ich... ich brauche mehr Kredit." );
    AddChoice( "Ich will meinen Kredit zurückzahlen!" );
}

BossWindowRequestPtr BossWindowMainRequest::SelectChoice( Unsigned32 _u32Idx ) const
{
    // Get player image
    Unsigned8 u8Image = m_spPlayer->GetImage();

    switch ( _u32Idx )
    {
    case 0: // Resignation
        {
            // Check image if boss tries to avoid resignation
            if ( u8Image <= 75 )
            {
                return BossWindowRequestPtr( new BossWindowResignateRequest() );
            }
            else
            {
                return BossWindowRequestPtr( new BossWindowAvoidResignationRequest() );
            }
        }
        break;
    case 1: // Need more credit
        {
            // Check boss answer
            const CFoundation::Money& curCredit = m_spPlayer->GetBudget().GetCredit();
            Unsigned8 u8Image = m_spPlayer->GetImage();

            // Compute credit limit and raise
            CFoundation::Money creditRaise;
            CFoundation::Money creditLimit;
            if ( u8Image <= 60 )
            {
                creditRaise = CFoundation::Money( 200000, CFoundation::CURRENCY_DOLLAR );
                creditLimit = CFoundation::Money( 300000, CFoundation::CURRENCY_DOLLAR );
            }
            else
            {
                creditRaise = CFoundation::Money( 400000, CFoundation::CURRENCY_DOLLAR );
                creditLimit = CFoundation::Money( 600000, CFoundation::CURRENCY_DOLLAR );
            }

            // Check if to allow immediately
            if ( curCredit + creditRaise < creditLimit )
            {
                // Add credit
                m_spPlayer->GetBudget().Book( ACCOUNT_SUNDRY_INCOME, ACCOUNT_CASH, creditRaise, m_spPlayer->GetGame()->GetDateTime() );
                m_spPlayer->GetBudget().AddCredit( creditRaise );

                BossWindowRequestPtr spRequest( new BossWindowRequest() );
                spRequest->SetQuestion( CFoundation::String::Build( "Na gut, Sie bekommen %s Kredit. Aber die Zinsen sind saftig!", creditRaise.ToString( CFoundation::CURRENCY_DOLLAR ).c_str() ) );
                return spRequest;
            }

            // Check if never allow
            if ( curCredit >= creditLimit )
            {
                BossWindowRequestPtr spRequest( new BossWindowRequest() );
                spRequest->SetQuestion( "Jetzt ist das Maß voll! Raus!" );
                return spRequest;
            }

            // NeedCreditRequest
            return BossWindowRequestPtr( new BossWindowNeedCreditRequest() );
        }
        break;
    case 2: // Repay credit
        {            
            // RepayCreditRequest
            return BossWindowRequestPtr( new BossWindowRepayCreditRequest() );
        }
        break;
    }

    return BossWindowRequestPtr();
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BossWindowResignationRequest
///
////////////////////////////////////////////////////////////////////////////////////////////
        
void BossWindowAvoidResignationRequest::CollectChoices()
{
    // Set question
    SetQuestion( CFoundation::String::Build( "Aber %s, wir wollen doch nichts überstürzen...", m_spPlayer->GetName().c_str() ) );

    // Add choices
    AddChoice( "Doch!" );
    AddChoice( "Na gut..." );
}

BossWindowRequestPtr BossWindowAvoidResignationRequest::SelectChoice( Unsigned32 _u32Idx ) const
{
    // Get player image
    Unsigned8 u8Image = m_spPlayer->GetImage();

    switch ( _u32Idx )
    {
    case 0: // Resignation
        {
            return BossWindowRequestPtr( new BossWindowResignateRequest() );
        }
        break;
    case 1: // Continue game
        {
            BossWindowRequestPtr spRequest( new BossWindowRequest() );
            spRequest->SetQuestion( "Na sehen Sie, ich wusste doch, dass Sie vernünftig sind." );
            return spRequest;
        }
        break;
    }
    
    return BossWindowRequestPtr();
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BossWindowResignateRequest
///
////////////////////////////////////////////////////////////////////////////////////////////
        
void BossWindowResignateRequest::CollectChoices()
{
    // Get player image
    Unsigned8 u8Image = m_spPlayer->GetImage();

    // Set question
    if ( u8Image <= 75 )
    {
        SetQuestion( "Sie wagen es, Sie Trottel? Sie brauchen nicht zu kündigen, das erledige ich schon!" );
    }
    else
    {
        SetQuestion( "Nach allem, was ich für Sie getan habe, kündigen Sie? Rauuuus!" );
    }

    // No choices
}

BossWindowRequestPtr BossWindowResignateRequest::SelectChoice( Unsigned32 _u32Idx ) const
{
    // TODO Just end game
    return BossWindowRequestPtr();
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BossWindowNeedCreditRequest
///
////////////////////////////////////////////////////////////////////////////////////////////
        
void BossWindowNeedCreditRequest::CollectChoices()
{
    // Get player image
    Unsigned8 u8Image = m_spPlayer->GetImage();

    // Set question and choices
    CFoundation::String strQuestion;
    if ( u8Image <= 60 )
    {
        strQuestion = "Sie widerlicher Verschwender! Nochmals $ 200.000 Kredit? Niemals!";
        AddChoice( "Ich brauche das Geld aber!" );
        AddChoice( "Wenigstens $ 100.000?" );
        AddChoice( "Blödmann." );
    }
    else
    {
        strQuestion = "Sie sind mir einer! Sie wollen nochmal $ 400.000 Kredit?";
        AddChoice( "Ich brauche das Geld!" );
        AddChoice( "Sagen wir $ 200.000." );
        AddChoice( "Entweder $400.000 oder ich kündige!" );
    }
    SetQuestion( strQuestion );
}

BossWindowRequestPtr BossWindowNeedCreditRequest::SelectChoice( Unsigned32 _u32Idx ) const
{
    // Get player image
    Unsigned8 u8Image = m_spPlayer->GetImage();

    switch ( _u32Idx )
    {
    case 0: // Need the money
        {
            // TODO Possible if sammy was won

            // Get refuse question
            CFoundation::String strQuestion;
            if ( u8Image <= 60 )
            {
                strQuestion = "Raus!";
            }
            else
            {
                strQuestion = "Aus meinen Augen!";
            }

            BossWindowRequestPtr spRequest( new BossWindowRequest() );
            spRequest->SetQuestion( strQuestion );
            return spRequest;
        }
        break;
    case 1: // Try to get half
        {
            // Set question
            CFoundation::String strQuestion;
            if ( u8Image <= 60 )
            {
                strQuestion = "Hm... aber kommen Sie mir nicht noch einmal!";
            }
            else
            {
                strQuestion = CFoundation::String::Build( "Ausnahmsweise... weil Sie es sind, %s.", m_spPlayer->GetName().c_str() );
            }

            // Add half credit
            m_spPlayer->GetBudget().Book( ACCOUNT_SUNDRY_INCOME, ACCOUNT_CASH, CFoundation::Money( 100000, CFoundation::CURRENCY_DOLLAR ), m_spPlayer->GetGame()->GetDateTime() );
            m_spPlayer->GetBudget().AddCredit( CFoundation::Money( 100000, CFoundation::CURRENCY_DOLLAR ) );

            BossWindowRequestPtr spRequest( new BossWindowRequest() );
            spRequest->SetQuestion( strQuestion );
            return spRequest;
        }
        break;
    case 2: // Dumbass
        {
            // Set question
            CFoundation::String strQuestion;
            if ( u8Image <= 60 )
            {
                strQuestion = "Raus!";
            }
            else
            {
                strQuestion = "Zeigen Sie erstmal...";
            }

            BossWindowRequestPtr spRequest( new BossWindowRequest() );
            spRequest->SetQuestion( strQuestion );
            return spRequest;
        }
        break;
    }

    return BossWindowRequestPtr();
}

////////////////////////////////////////////////////////////////////////////////////////////
///
/// BossWindowRepayCreditRequest
///
////////////////////////////////////////////////////////////////////////////////////////////
        
void BossWindowRepayCreditRequest::CollectChoices()
{
    // Get player image
    Unsigned8 u8Image = m_spPlayer->GetImage();
    const CFoundation::Money& curCredit = m_spPlayer->GetBudget().GetCredit();

    // Check boss answer
    CFoundation::String strQuestion;
    if ( curCredit.GetAmount() > 0 )
    {
        strQuestion = CFoundation::String::Build( "Dann mal her mit dem Geld, %s! Wieviel zahlen Sie denn zurück?", m_spPlayer->GetName().c_str() );

        AddChoice( CFoundation::String::Build( "%s.", GetCreditRepay( 0.25f ).ToString( CFoundation::CURRENCY_DOLLAR ).c_str() ) );
        AddChoice( CFoundation::String::Build( "%s.", GetCreditRepay( 0.50f ).ToString( CFoundation::CURRENCY_DOLLAR ).c_str() ) );
        AddChoice( "Alles!" );
    }
    else
    {
        strQuestion = CFoundation::String::Build( "Löblich, %s, nur haben Sie leider gar keinen Kredit!", m_spPlayer->GetName().c_str() );
    }
    SetQuestion( strQuestion );
}

BossWindowRequestPtr BossWindowRepayCreditRequest::SelectChoice( Unsigned32 _u32Idx ) const
{
    // Get player image
    Unsigned8 u8Image = m_spPlayer->GetImage();

    Float32 f32Percent = 0.0f;
    switch ( _u32Idx )
    {
    case 0: // Quarter
        {
            f32Percent = 0.25f;
        }
        break;
    case 1: // Half
        {
            f32Percent = 0.50f;
        }
        break;
    case 2: // All
        {
            f32Percent = 1.0f;
        }
        break;
    }

    // Check if enough cash
    CFoundation::String strQuestion;
    CFoundation::Money creditRepay = GetCreditRepay( f32Percent );
    if ( creditRepay < m_spPlayer->GetBudget().GetCash() )
    {
        strQuestion = CFoundation::String::Build( "So, und jetzt an die Arbeit, %s!", m_spPlayer->GetName().c_str() );
        
        // Decrease credit
        m_spPlayer->GetBudget().Book( ACCOUNT_CASH, ACCOUNT_SUNDRY_COSTS, creditRepay, m_spPlayer->GetGame()->GetDateTime() );
        m_spPlayer->GetBudget().AddCredit( -creditRepay );
    }
    else
    {
        strQuestion = CFoundation::String::Build( "Soviel Geld haben Sie garnicht, %s!", m_spPlayer->GetName().c_str() );
    }

    BossWindowRequestPtr spRequest( new BossWindowRequest() );
    spRequest->SetQuestion( strQuestion );
    return spRequest;
}

CFoundation::Money BossWindowRepayCreditRequest::GetCreditRepay( Float32 _f32Percent ) const
{
    Float32 f32Amount = m_spPlayer->GetBudget().GetCredit().GetAmount( CFoundation::CURRENCY_DOLLAR );
    
    // Round up
    f32Amount = ceilf( f32Amount * _f32Percent / 100000.0 ) * 100000;

    return CFoundation::Money( f32Amount, CFoundation::CURRENCY_DOLLAR );
}
