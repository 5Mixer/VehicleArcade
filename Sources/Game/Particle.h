#ifndef GAME_PARTICLE
#define GAME_PARTICLE

#include "../Engine/Colour.h"
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
        const static int maxLife = 60;

    public:
        Particle(Kore::vec2 pos, float angle)
            : pos(pos),
              angle(angle){};

        Kore::vec2 pos;
        std::uint8_t health = maxLife; // life in frames
        float angle;
        void update() {
            speed *= .95;
            pos += Kore::vec2{std::cos(angle) * speed, std::sin(angle) * speed};
            health--;
        }
        void render(Engine::Graphics &g) {
            // Todo: don't hardcode gradient
            float gradientProgression = static_cast<float>(maxLife - health) / static_cast<float>(maxLife);
            std::uint32_t colour = Engine::Colour::sampleLinearGradient(.6f, .3f, .25f, 1.f, .3f, .2f, .2f, 0.f, gradientProgression);
            g.drawParticle(pos, colour);
        }
    };
} // namespace Game

#endif
