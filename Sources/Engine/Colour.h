#ifndef ENGINE_COLOUR
#define ENGINE_COLOUR

#include <cstdint>

namespace Engine {
    class Color {
    public:
        static std::uint32_t fromRGBA(float r, float g, float b, float a) {
            return (static_cast<int>(a * 255) << 24) +
                   (static_cast<int>(r * 255) << 16) +
                   (static_cast<int>(b * 255) << 8) +
                   (static_cast<int>(a * 255));
        }
    };
} // namespace Engine

#endif