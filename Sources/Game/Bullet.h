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
        float speed = 30;

    public:
        float angle;
        void update();
        void render(Engine::Graphics &g);
    };
} // namespace Game

#endif