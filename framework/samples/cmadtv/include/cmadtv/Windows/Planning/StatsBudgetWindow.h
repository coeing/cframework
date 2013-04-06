#ifndef CMADTV_STATS_BUDGET_WINDOW_H
#define CMADTV_STATS_BUDGET_WINDOW_H

#include "cfoundation/Math/Vector.h"

#include "cgui/Gui/GuiObjectEvents.h"

#include "cctrl/Controls/ButtonGroup.h"

#include "cmadtv/Windows/Window.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( GuiObject );
    INCLUDE_SHARED_CLASS( TooltipController );
    TYPEDEF_DELEGATE3( TooltipControllerEvent, TooltipControllerPtr, GuiObjectPtr, const CFoundation::Vector2Di& );
    INCLUDE_SHARED_CLASS( CheckBox );
    TYPEDEF_DELEGATE3( CheckBoxEvent, CheckBoxPtr, bool, bool );
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( Transmitter );

    enum AccountIdentifier;

    class StatsBudgetWindow : public Window
    {
    public:
        /// Returns the xml file name
        static CFoundation::String  GetXMLFileName();

        /// Constructor
        StatsBudgetWindow();

        /// Destructor
        ~StatsBudgetWindow();

        /// Initializes the window
        virtual void            InitWindow();

        /// Deinitializes the window
        virtual void            DeinitWindow();

        /// Processes input (when window is active)
        virtual void            ProcessInput( const CSystem::InputState& _State );

        /// Callback when context changed
        virtual void            OnContextChanged();

    private:
        typedef std::map< AccountIdentifier, CGui::TextBoxPtr > AccountTextBoxMap;

        /// Sets the player
        void                    SetPlayer( PlayerPtr _spPlayer );

        /// Gets the value textbox of the passed value panel
        CGui::TextBoxPtr        GetValueTextBox( const CFoundation::String& _strPanelName ) const;

        /// Updates the stats
        void                    UpdateStats();

        /// Callbacks
        void                    OnDayPushed();

        /// Delegates
        CGui::ObjectInputEventDelegate0Ptr  m_spDelegateDayPushed;

        /// Player
        PlayerPtr               m_spPlayer;

        /// Indicates if the stats for today or yesterday are shown
        bool                    m_bDateToday;

        /// Controls
        AccountTextBoxMap       m_mapAccountTextBoxes;
        CGui::TextBoxPtr        m_spTbExpensesTotal;
        CGui::TextBoxPtr        m_spTbIncomeTotal;

        CGui::TextBoxPtr        m_spTbOverviewPrev;
        CGui::TextBoxPtr        m_spTbOverviewExpenses;
        CGui::TextBoxPtr        m_spTbOverviewIncome;
        CGui::TextBoxPtr        m_spTbOverviewInterest;
        CGui::TextBoxPtr        m_spTbOverviewCurrent;

        CGui::ButtonPtr         m_spBtDay;
    };
}

#endif
