#ifndef CMADTV_INPUT_TRANSLATOR_H
#define CMADTV_INPUT_TRANSLATOR_H

#include "csystem/Input/InputTranslator.h"

namespace CMadTV
{
    enum InputCommand
    {
        INPUT_COMMAND_NONE,

        INPUT_COMMAND_PLANNING_ADVERT_ARCHIV_WINDOW_CLOSE,
        INPUT_COMMAND_PLANNING_BROADCAST_ARCHIV_WINDOW_CLOSE,
        INPUT_COMMAND_PLANNING_RACK_SHELF_WINDOW_CLOSE,

        INPUT_COMMAND_DEBUG_SPEED_PAUSE,
        INPUT_COMMAND_DEBUG_SPEED_1,
        INPUT_COMMAND_DEBUG_SPEED_2,
        INPUT_COMMAND_DEBUG_SPEED_3,
        INPUT_COMMAND_DEBUG_GET_MONEY,

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
void CFoundation::EnumConverter< CMadTV::InputCommand >::Initialize();

#endif
