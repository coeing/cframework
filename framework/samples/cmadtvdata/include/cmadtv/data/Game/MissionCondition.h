#ifndef CMADTV_MISSION_CONDITION_H
#define CMADTV_MISSION_CONDITION_H

#include "cfoundation/Base/Types.h"
#include "cfoundation/Events/DelegateInvoker.h"
#include "cfoundation/Utils/SharedPtr.h"

#include "cmadtv/Data/Game/MissionEvents.h"

namespace CFoundation
{
    class Serializer;
    class String;
}

namespace CMadTV
{
    INCLUDE_SHARED_CLASS( Game );
    INCLUDE_SHARED_CLASS( Player );
    INCLUDE_SHARED_CLASS( World );

    class MissionCondition : public CFoundation::SharedPtrBase< MissionCondition >, public CFoundation::Delegate1Invoker< MissionConditionEvent, MissionConditionPtr >
    {
    public:
        DELEGATE1_INVOKER( MissionConditionEvent, MissionConditionPtr );

        /// Constructor
        MissionCondition();

        /// Returns the type of this condition
        virtual CFoundation::String GetType() const = 0;

        /// Initialization
        virtual void                Init();

        /// Deinitialization
        virtual void                Deinit();

        /// Sets the world
        virtual void                SetWorld( WorldPtr _spWorld );

        /// Returns the world
        WorldPtr                    GetWorld() const;

        /// Sets the game
        virtual void                SetGame( GamePtr _spGame );

        /// Returns the game
        GamePtr                     GetGame() const;

        /// Sets the player
        virtual void                SetPlayer( PlayerPtr _spPlayer );

        /// Returns the player
        PlayerPtr                   GetPlayer() const;

        /// Serialization
        virtual void                Serialize( CFoundation::Serializer& _Serializer );

        /// Per frame update
        virtual void                Update( Float32 _f32TimeDifference );

        /// Indicates if the condition is forfilled
        virtual bool                IsForfilled() const = 0;

        /// Creates a description for this condition
        virtual CFoundation::String GetDescription() const = 0;

    private:
        /// World
        WorldPtr                m_spWorld;

        /// Game
        GamePtr                 m_spGame;

        /// Player
        PlayerPtr               m_spPlayer;
    };
}

#endif
