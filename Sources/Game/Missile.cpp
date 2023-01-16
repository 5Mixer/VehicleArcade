#include "Missile.h"

void Game::Missile::update() {
    pos += Kore::vec2{std::cos(angle) * speed, std::sin(angle) * speed};
}

void Game::Missile::render(Engine::Graphics &g) {
    g.drawMissile(pos, angle);
}

const Game::Net::BulletData Game::Missile::getData() {
    return Game::Net::BulletData{
        shooter,
        Game::Net::Vec2{
            pos.x(),
            pos.y()},
        angle};
}
