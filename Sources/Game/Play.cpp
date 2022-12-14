#include "Play.h"

Game::Play::Play() {
    entities.push_back(Game::Vehicle());
}

void Game::Play::render(std::shared_ptr<Engine::Graphics> graphics) {
    graphics->drawSprite(0, 50, 50);
}

void Game::Play::update() {
}

std::string Game::Play::toString() {
    return "Play";
}
