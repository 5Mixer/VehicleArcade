#ifndef GAME_ENTITY
#define GAME_ENTITY

#include <Kore/Math/Vector.h>
#include <stdio.h>

namespace Game {
    class Entity {
    public:
        Kore::vec2 pos;
        void update(){};
    };
} // namespace Game

#endif