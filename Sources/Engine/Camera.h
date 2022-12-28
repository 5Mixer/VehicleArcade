#ifndef ENGINE_CAMERA
#define ENGINE_CAMERA

#include <Kore/Math/Matrix.h>
#include <Kore/Math/Vector.h>

namespace Engine {
    class Camera {
    private:
    public:
        Camera();
        ~Camera();
        Kore::mat3 getTransform();
    };

    Camera::Camera() {
    }

    Camera::~Camera() {
    }

} // namespace Engine

#endif