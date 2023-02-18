#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <Kore/Input/Keyboard.h>
#include <Kore/Input/Mouse.h>
#include <Kore/Math/Vector.h>
#include <array>
#include <cstdint>
#include <fmt/core.h>
#include <limits>
#include <set>

namespace Engine {
    namespace Input {
        inline int scrollPosition = 0;
        inline std::array<bool, std::numeric_limits<std::uint8_t>::max() + 1> keysDown;
        inline Kore::vec2 mousePosition;
        inline bool mouseDown;
        inline std::wstring pressedKeys;
        void init();
    } // namespace Input
} // namespace Engine

#endif