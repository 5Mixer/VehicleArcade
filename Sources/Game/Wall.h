#ifndef GAME_WALL
#define GAME_WALL

#include <Kore/Math/Vector.h>
#include <cstdint>

namespace Game {
    class Wall {
    private:
    public:
        std::uint8_t placer;
        Kore::vec2 pos;
        Wall(Kore::vec2 pos);
        int health = 8;
    };
} // namespace Game

#endif
