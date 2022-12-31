#ifndef GAME_WALL
#define GAME_WALL

#include <Kore/Math/Vector.h>

namespace Game {
    class Wall {
    private:
    public:
        Kore::vec2 position;
        Wall(Kore::vec2 position);
        int health = 8;
    };
} // namespace Game

#endif
