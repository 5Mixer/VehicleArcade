#include "Bullet.h"

const Game::Net::BulletData Game::Bullet::getData() {
    return Game::Net::BulletData{
        shooter,
        Game::Net::Vec2{
            pos.x(),
            pos.y()},
        angle};
}
