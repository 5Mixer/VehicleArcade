#ifndef GAME_WALL
#define GAME_WALL

#include <Kore/Math/Vector.h>
#include <cstdint>

namespace Game {
    class Wall {
    private:
    public:
        Wall(std::uint8_t placer, Kore::vec2 pos, int health = 8)
            : placer(placer), pos(pos), health(health){};

        std::uint8_t placer;
        Kore::vec2 pos;
        int health = 8;
    };
} // namespace Game

#endif
