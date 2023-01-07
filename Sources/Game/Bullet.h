#ifndef GAME_BULLET
#define GAME_BULLET

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include <Kore/Math/Vector.h>
#include <cmath>

namespace Game {
    class Bullet {
    private:
        float speed = .1; // 6 + Engine::Core::getInstance().rand() * .1;

    public:
        Kore::vec2 pos;
        std::uint8_t shooter;
        float angle;
        void update();
        void render(Engine::Graphics &g);
    };
} // namespace Game

#endif
