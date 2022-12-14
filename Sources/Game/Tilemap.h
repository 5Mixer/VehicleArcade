#ifndef GAME_TILEMAP
#define GAME_TILEMAP

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include <Kore/Math/Vector.h>
#include <array>

namespace Game {
    class Tilemap {
    private:
        int tilesWide = 64;
        int tilesHigh = 32;
        std::array<int, 32 * 64> tiles;

    public:
        Tilemap();
        int get(int x, int y);
        void set(int x, int y, int tile);
        void render(Engine::Graphics *g);
    };
} // namespace Game

#endif