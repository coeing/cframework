#ifndef CED_INPUT_TRANSLATOR_H
#define CED_INPUT_TRANSLATOR_H

#include "csystem/Input/InputTranslator.h"

namespace Ced
{
    enum InputCommand
    {
        INPUT_COMMAND_NONE,

        INPUT_COMMAND_DELETE_SELECTED_OBJECT,

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
void CFoundation::EnumConverter< Ced::InputCommand >::Initialize();

#endif
