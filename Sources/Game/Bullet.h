#ifndef GAME_BULLET
#define GAME_BULLET

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include "Entity.h"
#include <Kore/Math/Vector.h>
#include <cmath>

namespace Game {
    class Bullet : public Game::Entity {
    private:
        float speed = 25 + 4 * Engine::Core::getInstance().rand();

    public:
        float angle;
        void update();
        void render(Engine::Graphics &g);
    };
} // namespace Game

#endif