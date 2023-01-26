#ifndef GAME_COLLIDER
#define GAME_COLLIDER

#include <Kore/Math/Vector.h>

namespace Game {
    class CircleCollider {
    public:
        virtual Kore::vec2 getColliderPos() = 0;
        virtual float getColliderRadius() = 0;
    };
} // namespace Game

#endif