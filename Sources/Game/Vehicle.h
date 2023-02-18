#ifndef GAME_VEHICLE
#define GAME_VEHICLE

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include "../Engine/Transform.h"
#include "Collider.h"
#include "Net/EntityGenerated.h"
#include "Wall.h"
#include <Kore/Math/Vector.h>
#include <cmath>
#include <string>
#include <vector>

namespace Game {
    class Vehicle : public CircleCollider {
    private:
        static const int radius = 50;
        float forwardsVelocity = 0;
        float turnRateVelocityFactor(float speed, float maximumSpeed);
        float visualAngleDelta = 0;

    public:
        Vehicle(std::uint8_t id, Kore::vec2 pos, float angle, std::string name)
            : id(id), pos(pos), angle(angle), name(name){};
        Vehicle(const Net::PlayerData *data, const std::string name)
            : id(data->id()),
              pos(Kore::vec2{data->pos().x(), data->pos().y()}),
              angle(data->angle()),
              name(name){};

        Kore::vec2 getColliderPos() { return pos; };
        float getColliderRadius() { return radius; };

        const Net::PlayerData getData();

        std::uint8_t id;
        std::string name;
        Kore::vec2 pos{0, 0};
        Kore::vec2 velocity{0, 0};
        float angle = 0;

        int health = 25;
        int maxHealth = 25;

        int money = 0;

        void changeHealth(int offset) {
            if (health + offset < 0) {
                health = 0;
            } else if (health + offset > maxHealth) {
                health = maxHealth;
            } else {
                health += offset;
            }
        }

        Kore::vec2 getBackLeftWheelPos();
        Kore::vec2 getBackRightWheelPos();
        Kore::vec2 getFrontLeftWheelPos();
        Kore::vec2 getFrontRightWheelPos();

        float skidFactor() {
            if (velocity.isZero())
                return 0;

            Kore::vec2 absVelocity{velocity.x(), velocity.y()};
            absVelocity.normalize();
            return absVelocity.dot(Kore::vec2{cos(angle), sin(angle)});
        }

        void accelerate(float acceleration);
        void turn(float angleDelta);
        void controlWithInput();
        void update(std::vector<Game::Wall> &walls, int worldWidth, int worldHeight);
        void render(Engine::Graphics &g);
    };
} // namespace Game
#endif
