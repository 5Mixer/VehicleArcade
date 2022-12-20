#include "Tilemap.h"

Game::Tilemap::Tilemap() {
    for (int tx = 0; tx < tilesWide; tx++) {
        for (int ty = 0; ty < tilesHigh; ty++) {
            float r = Engine::Core::getInstance().rand();
            tiles[ty * tilesWide + tx] = (r < .9 ? 1 : (r < .95 ? 2 : 3));
        }
    }
}

void Game::Tilemap::render(Engine::Graphics &g) {
    for (int tx = 0; tx < tilesWide; tx++) {
        for (int ty = 0; ty < tilesHigh; ty++) {
            g.drawSprite(tiles.at(ty * tilesWide + tx), Kore::vec2{tx * 64, ty * 64});
        }
    }
}

int Game::Tilemap::get(int tx, int ty) {
    return tiles[ty * tilesWide + tx];
}

void Game::Tilemap::set(int tx, int ty, int tile) {
    tiles[ty * tilesWide + tx] = tile;
}