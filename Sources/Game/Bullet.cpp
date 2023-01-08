#include "Bullet.h"

void Game::Bullet::update() {
    pos += Kore::vec2{std::cos(angle) * speed, std::sin(angle) * speed};
}

void Game::Bullet::render(Engine::Graphics &g) {
    g.drawBullet(pos, angle);
}

const Game::Net::BulletData Game::Bullet::getData() {
    return Game::Net::BulletData{
        shooter,
        Game::Net::Vec2{
            pos.x(),
            pos.y()},
        angle};
}
