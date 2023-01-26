#ifndef GAME_BULLET
#define GAME_BULLET

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include "Collider.h"
#include "Net/EntityGenerated.h"
#include <Kore/Math/Vector.h>
#include <cmath>

namespace Game {
    class Bullet : public CircleCollider {
    private:
        float speed = 30;
        static const int radius = 50;

    public:
        Bullet(std::uint8_t shooter, Kore::vec2 pos, float angle)
            : shooter(shooter),
              pos(pos),
              angle(angle){};

        Bullet(const Net::BulletData *data)
            : shooter(data->shooter()),
              pos(Kore::vec2(data->pos().x(), data->pos().y())),
              angle(data->angle()){};

        Kore::vec2 getColliderPos() { return pos; };
        float getColliderRadius() { return radius; };

        const Net::BulletData getData();

        Kore::vec2 pos;
        std::uint8_t shooter;
        std::uint8_t health = 1;
        float angle;
        void update() {
            pos += Kore::vec2{std::cos(angle) * speed, std::sin(angle) * speed};
        }
        void render(Engine::Graphics &g) {
            g.drawBullet(pos);
        }
    };
} // namespace Game

#endif
