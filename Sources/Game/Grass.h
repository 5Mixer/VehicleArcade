#ifndef GAME_GRASS
#define GAME_GRASS

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include "Net/EntityGenerated.h"
#include <Kore/Graphics1/Color.h>
#include <Kore/Math/Vector.h>
#include <cmath>

namespace Game {
    class Grass {
    private:
    public:
        Grass(Kore::vec2 pos, float angle, std::uint8_t variety)
            : pos(pos), angle(angle), variety(variety){};

        Grass(const Net::GrassData *data)
            : variety(data->variety()),
              pos(Kore::vec2(data->pos().x(), data->pos().y())),
              angle(data->angle()){};

        const Net::GrassData getData();

        Kore::vec2 pos;
        float angle;
        std::uint8_t variety;

        void render(Engine::Graphics &g) {
            g.drawGrass(pos, angle, variety);
        }
    };
} // namespace Game

#endif