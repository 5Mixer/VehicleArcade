#ifndef GAME_BULLET
#define GAME_BULLET

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include <Kore/Math/Vector.h>
#include <cmath>

namespace Game {
    class Bullet {
    private:
        float speed = 6; // 6 + Engine::Core::getInstance().rand() * .1;

    public:
        Kore::vec2 pos;
        float angle;
        void update();
        void render(Engine::Graphics &g);
    };
} // namespace Game

#endif
