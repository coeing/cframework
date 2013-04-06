#ifndef CETRIS_INPUT_TRANSLATOR_H
#define CETRIS_INPUT_TRANSLATOR_H

#include "csystem/Input/InputTranslator.h"

namespace Cetris
{
    enum InputCommand
    {
        INPUT_COMMAND_NONE,

        INPUT_COMMAND_GAME_PAUSE,

        // Movement
        INPUT_COMMAND_GAME_MOVE_LEFT,
        INPUT_COMMAND_GAME_MOVE_RIGHT,
        INPUT_COMMAND_GAME_MOVE_DOWN,

        // Rotation
        INPUT_COMMAND_GAME_ROT_CCW,
        INPUT_COMMAND_GAME_ROT_CW,

        INPUT_COMMAND_END
    };

    class InputTranslator : public CSystem::InputTranslator< InputCommand >, public CFoundation::Singleton< InputTranslator >
    {
    public:
        /// Constructor
        InputTranslator();
    };
}

template<>
void CFoundation::EnumConverter< Cetris::InputCommand >::Initialize();

#endif
