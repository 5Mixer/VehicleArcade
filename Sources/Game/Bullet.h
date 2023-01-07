#ifndef GAME_BULLET
#define GAME_BULLET

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include "Net/EntityGenerated.h"
#include <Kore/Math/Vector.h>
#include <cmath>

namespace Game {
    class Bullet {
    private:
        float speed = .1; // 6 + Engine::Core::getInstance().rand() * .1;

    public:
        Bullet(std::uint8_t shooter, Kore::vec2 pos, float angle)
            : shooter(shooter), pos(pos), angle(angle){};

        Bullet(const Net::BulletData *data)
            : shooter(data->shooter()),
              pos(Kore::vec2(data->pos().x(), data->pos().y())),
              angle(data->angle()){};

        const Net::BulletData getData();

        Kore::vec2 pos;
        std::uint8_t shooter;
        float angle;
        void update();
        void render(Engine::Graphics &g);
    };
} // namespace Game

#endif
