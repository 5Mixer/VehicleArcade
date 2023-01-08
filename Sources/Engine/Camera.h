#ifndef ENGINE_CAMERA
#define ENGINE_CAMERA

#include <Kore/Math/Matrix.h>
#include <Kore/Math/Vector.h>
#include <Kore/System.h>

namespace Engine {
    class Camera {
    private:
    public:
        float zoom = 1;
        Kore::vec2 pos;
        Kore::mat3 getTransform();
        Kore::vec2 getWorldPos(Kore::vec2 screenPos);
    };
} // namespace Engine

#endif