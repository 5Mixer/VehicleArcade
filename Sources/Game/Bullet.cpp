#include "Bullet.h"

void Game::Bullet::update() {
    pos += Kore::vec2{std::cos(angle) * speed, std::sin(angle) * speed};
}

void Game::Bullet::render(Engine::Graphics &g) {
    g.drawSprite(4, pos, angle);
}
