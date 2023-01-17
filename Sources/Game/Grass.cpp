#include "Grass.h"

void Game::Grass::render(Engine::Graphics &g) {
    g.drawGrass(pos, angle, variety);
}

const Game::Net::GrassData Game::Grass::getData() {
    return Game::Net::GrassData{
        variety,
        Game::Net::Vec2{
            pos.x(),
            pos.y()},
        angle};
}
