#ifndef ENGINE_COLOUR
#define ENGINE_COLOUR

#include <cstdint>

namespace Engine {
    class Colour {
    private:
        static float lerp(float a, float b, float t) {
            return a + (b - a) * t;
        }

    public:
        static std::uint32_t fromRGBA(float r, float g, float b, float a) {
            return (static_cast<int>(a * 255) << 24) +
                   (static_cast<int>(r * 255) << 16) +
                   (static_cast<int>(g * 255) << 8) +
                   (static_cast<int>(b * 255));
        }
        static std::uint32_t sampleLinearGradient(float r1, float g1, float b1, float a1, float r2, float g2, float b2, float a2, float t) {
            return fromRGBA(
                lerp(r1, r2, t),
                lerp(g1, g2, t),
                lerp(b1, b2, t),
                lerp(a1, a2, t)
            );
        }
    };
} // namespace Engine

#endif