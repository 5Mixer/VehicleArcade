#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <Kore/Input/Keyboard.h>
#include <array>

namespace Engine
{
    namespace Input
    {
        inline std::array<bool, 256> keysDown;
        void init();
    } // namespace Input
} // namespace Engine

#endif