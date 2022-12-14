#include "Play.h"

Game::Play::Play() {
    // entities.push_back(Game::Vehicle());
    controlledCar = std::make_shared<Game::Vehicle>();
}

void Game::Play::render(std::shared_ptr<Engine::Graphics> graphics) {
    for (auto &entity : entities) {
        entity.pos.add(Kore::vec2(0, 1));
        entity.update();
        graphics->drawSprite(0, entity.pos);
    }
    controlledCar->render(graphics.get());
}

void Game::Play::update() {
    for (auto entity : entities) {
        entity.update();
    }
    controlledCar->update();
}

std::string Game::Play::toString() {
    return "Play";
}
