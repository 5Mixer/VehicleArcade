#ifndef GAME_VEHICLE
#define GAME_VEHICLE

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include "Entity.h"
#include <Kore/Math/Vector.h>
#include <cmath>

namespace Game {
    class Vehicle : public Game::Entity {
    private:
        float forwardsVelocity;
        float angle;
        float turnRateVelocityFactor(float speed, float maximumSpeed);

    public:
        void accelerate(float acceleration);
        void turn(float angleDelta);
        void update();
        void render(Engine::Graphics &g);
    };
} // namespace Game

#endif