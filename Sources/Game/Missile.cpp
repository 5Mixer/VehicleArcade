#include "Missile.h"

#include <cmath>

const Game::Net::BulletData Game::Missile::getData() {
    return Game::Net::BulletData{
        shooter,
        Game::Net::Vec2{
            pos.x(),
            pos.y()},
        angle};
}
