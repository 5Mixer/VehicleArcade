#ifndef GAME_VEHICLE
#define GAME_VEHICLE

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include "Entity.h"
#include <Kore/Math/Vector.h>

namespace Game {
    class Vehicle : public Game::Entity {
    private:
        double acceleration;
        double angle;

    public:
        void accelerate(double accelerationDelta);
        void turn(double angleDelta);
        void update();
        void render(Engine::Graphics *g);
    };
} // namespace Game

#endif