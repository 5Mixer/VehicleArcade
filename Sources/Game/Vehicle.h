#ifndef GAME_VEHICLE
#define GAME_VEHICLE

#include "Entity.h"
#include <Kore/Math/Vector.h>

namespace Game {
    class Vehicle : public Game::Entity {
    private:
        Kore::vec2 pos;
        double acceleration;
        double angle;

    public:
        void accelerate(double accelerationDelta);
        void turn(double angleDelta);
    };
} // namespace Game

#endif