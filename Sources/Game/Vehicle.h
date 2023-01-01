#ifndef GAME_VEHICLE
#define GAME_VEHICLE

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include "Wall.h"
#include <Kore/Math/Vector.h>
#include <cmath>
#include <vector>

namespace Game {
    class Vehicle {
    private:
        float forwardsVelocity;
        float turnRateVelocityFactor(float speed, float maximumSpeed);

    public:
        Kore::vec2 pos;
        float angle;
        void accelerate(float acceleration);
        void turn(float angleDelta);
        void update(std::vector<Game::Wall> &walls);
        void render(Engine::Graphics &g);
    };
} // namespace Game
#endif
