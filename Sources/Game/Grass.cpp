#include "Grass.h"

const Game::Net::GrassData Game::Grass::getData() {
    return Game::Net::GrassData{
        variety,
        Game::Net::Vec2{
            pos.x(),
            pos.y()},
        angle};
}
