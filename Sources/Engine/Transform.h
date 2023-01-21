#ifndef ENGINE_TRANSFORM
#define ENGINE_TRANSFORM

#include <Kore/Math/Matrix.h>

namespace Engine {
    namespace Transform {
        inline Kore::mat3 rotate(float angle, float centerx, float centery) {
            return (Kore::mat3::Translation(centerx, centery) * Kore::mat3::RotationZ(angle)) * Kore::mat3::Translation(-centerx, -centery);
        }

    } // namespace Transform
} // namespace Engine
#endif