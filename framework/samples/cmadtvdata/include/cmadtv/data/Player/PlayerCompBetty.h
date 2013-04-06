#ifndef CMADTV_PLAYER_COMP_BETTY_H
#define CMADTV_PLAYER_COMP_BETTY_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/DateTime/Date.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cmadtv/Data/World/Hour.h"

namespace CFoundation
{
    class Serializer;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( PlayerCompBetty );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( Present );

    /// Events
    enum PlayerCompBettyEventU8U8
    {
        EVENT_BETTY_LOVE_CHANGED,       // Parameters: New love value, Old love value
    };
    TYPEDEF_DELEGATE2( PlayerCompBettyEventU8U8, Unsigned8, Unsigned8 );

    enum PlayerCompBettyEventPresent
    {
        EVENT_BETTY_PRESENT_CHANGED,    // Parameters: New present
    };
    TYPEDEF_DELEGATE1( PlayerCompBettyEventPresent, PresentPtr );

    ////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// PlayerCompBetty
    ///
    ////////////////////////////////////////////////////////////////////////////////////////////

    /**
	 * \brief
	 * This component contains all the player data that belongs to Betty
	 * 
	 * 
	 * \remarks
	 * 
	 * \see
	 */

    class PlayerCompBetty : public CFoundation::Delegate2Invoker< PlayerCompBettyEventU8U8, Unsigned8, Unsigned8 >,
                                   CFoundation::Delegate1Invoker< PlayerCompBettyEventPresent, PresentPtr >
    {
    public:
        DELEGATE2_INVOKER( PlayerCompBettyEventU8U8, Unsigned8, Unsigned8 );
        DELEGATE1_INVOKER( PlayerCompBettyEventPresent, PresentPtr );

        /// Constructor
        PlayerCompBetty();

        /// Destructor
        ~PlayerCompBetty();

        /// Serialization
        virtual void        Serialize( CFoundation::Serializer& _Serializer );

        /// Sets the present that's currently on the hold for Betty
        void                SetPresent( PresentPtr _spPresent );

        /// Returns the present that's currently on the hold for Betty
        PresentPtr          GetPresent() const;

        /// Enables/Disables if it's possible to return the present
        void                EnablePresentReturn( bool _bEnabled );

        /// Indicates if it's possible to return the present
        bool                IsPresentReturnEnabled() const;

        /// Adds the passed amount of love from betty
        void                AddLove( Integer8 _i8Love );

        /// Returns the love from betty (0-100)
        Unsigned8           GetLove() const;
        
    private:
        /// Sets the love from betty
        void                SetLove( Unsigned8 _u8Love );

        /// Current present for betty
        PresentPtr          m_spPresent;

        /// Indicates if it's possible to return the present
        bool                m_bPresentReturnEnabled;

        /// Love from betty
        Unsigned8           m_u8Love;
    };
}

#endif
