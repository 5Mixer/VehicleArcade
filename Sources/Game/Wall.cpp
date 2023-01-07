#include "Wall.h"

const Game::Net::WallData Game::Wall::getData() {
    return Game::Net::WallData{
        placer,
        Game::Net::Vec2{
            pos.x(),
            pos.y()},
        health};
}