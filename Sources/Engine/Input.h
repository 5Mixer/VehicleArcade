#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <Kore/Input/Keyboard.h>
#include <Kore/Input/Mouse.h>
#include <Kore/Math/Vector.h>
#include <array>

namespace Engine {
    namespace Input {
        inline std::array<bool, 256> keysDown;
        inline Kore::vec2 mousePosition;
        inline bool mouseDown;
        void init();
    } // namespace Input
} // namespace Engine

#endif