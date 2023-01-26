#ifndef GAME_BULLET
#define GAME_BULLET

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include "Collider.h"
#include "Net/EntityGenerated.h"
#include <Kore/Math/Vector.h>
#include <cmath>

namespace Game {
    constexpr int bulletRadius = 50;
    class Bullet : public CircleCollider {
    private:
        float speed = 30;

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
        float getColliderRadius() { return bulletRadius; };

        const Net::BulletData getData();

        Kore::vec2 pos;
        std::uint8_t shooter;
        std::uint8_t health = 1;
        float angle;
        void update();
        void render(Engine::Graphics &g);
    };
} // namespace Game

#endif
