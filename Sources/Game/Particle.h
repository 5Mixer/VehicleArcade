#ifndef GAME_PARTICLE
#define GAME_PARTICLE

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include "Collider.h"
#include "Net/EntityGenerated.h"
#include <Kore/Math/Vector.h>
#include <cmath>

namespace Game {
    class Particle {
    private:
        float speed = 5;

    public:
        Particle(Kore::vec2 pos, float angle)
            : pos(pos),
              angle(angle){};

        Kore::vec2 pos;
        std::uint8_t health = 255;
        float angle;
        void update() {
            speed *= .95;
            pos += Kore::vec2{std::cos(angle) * speed, std::sin(angle) * speed};
            health--;
        }
        void render(Engine::Graphics &g) {
            g.drawParticle(pos);
        }
    };
} // namespace Game

#endif
