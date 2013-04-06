#ifndef CMADTV_MISSION_CONDITION_ADVERTS_SENT_H
#define CMADTV_MISSION_CONDITION_ADVERTS_SENT_H

#include "cfoundation/Events/Delegate.h"

#include "cmadtv/Data/Game/MissionCondition.h"

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Advert );

    TYPEDEF_DELEGATE2( SchedulingAdvertEvent, AdvertPtr, bool );

    class MissionConditionAdvertsSent : public MissionCondition
    {
    public:
        /// Constructor
        MissionConditionAdvertsSent();

        /// Returns the type of this condition
        virtual CFoundation::String GetType() const;

        /// Serialization
        virtual void                Serialize( CFoundation::Serializer& _Serializer );

        /// Sets the player
        virtual void                SetPlayer( PlayerPtr _spPlayer );

        /// Sets the number of adverts that should be sent
        void                        SetNumAdverts( Unsigned32 _u32NumAdverts );

        /// Sets if the adverts should succeed of fail
        void                        SetAdvertSuccess( bool _bSucceed );

        /// Indicates if the condition is forfilled
        virtual bool                IsForfilled() const;

        /// Creates a description for this condition
        virtual CFoundation::String GetDescription() const;

    private:
        /// Callbacks
        void                    OnAdvertSent( AdvertPtr _spAdvert, bool _bSuccess );

        /// Delegates
        SchedulingAdvertEventDelegate2Ptr   m_spDelegateAdvertSent;

        /// Number of adverts to sent to forfill condition
        Unsigned32              m_u32NumSentTarget;

        /// Number of adverts sent
        Unsigned32              m_u32NumSent;
    };
}

#endif
