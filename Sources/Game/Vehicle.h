#ifndef GAME_VEHICLE
#define GAME_VEHICLE

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include "Net/EntityGenerated.h"
#include "Wall.h"
#include <Kore/Math/Vector.h>
#include <cmath>
#include <vector>

namespace Game {
    class Vehicle {
    private:
        float forwardsVelocity = 0;
        float turnRateVelocityFactor(float speed, float maximumSpeed);
        float visualAngleDelta = 0;

    public:
        Vehicle(std::uint8_t id, Kore::vec2 pos, float angle)
            : id(id), pos(pos), angle(angle){};
        Vehicle(const Net::PlayerData *data)
            : id(data->id()),
              pos(Kore::vec2{data->pos().x(), data->pos().y()}),
              angle(data->angle()){};

        const Net::PlayerData getData();

        std::uint8_t id;
        Kore::vec2 pos{0, 0};
        float angle = 0;

        void accelerate(float acceleration);
        void turn(float angleDelta);
        void update(std::vector<Game::Wall> &walls);
        void render(Engine::Graphics &g);
    };
} // namespace Game
#endif
