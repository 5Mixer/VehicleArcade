#ifndef GAME_ITEM
#define GAME_ITEM

namespace Game {
    enum class Item {
        Bullets,
        Missiles,
        Walls,
    };
    inline int maxItems = 3;
} // namespace Game

#endif