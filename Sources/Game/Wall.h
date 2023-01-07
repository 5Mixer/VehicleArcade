#ifndef GAME_WALL
#define GAME_WALL

#include "Net/EntityGenerated.h"
#include <Kore/Math/Vector.h>
#include <cstdint>

namespace Game {
    class Wall {
    private:
    public:
        Wall(std::uint8_t placer, Kore::vec2 pos, int health = 8)
            : placer(placer), pos(pos), health(health){};
        Wall(const Net::WallData *data)
            : placer(data->placer()),
              pos(Kore::vec2{
                  data->pos().x(),
                  data->pos().y()}),
              health(data->health()){};

        const Net::WallData getData();

        std::uint8_t placer;
        Kore::vec2 pos;
        int health = 8;
    };
} // namespace Game

#endif
