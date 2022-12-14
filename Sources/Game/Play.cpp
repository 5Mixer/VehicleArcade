#include "Play.h"

Game::Play::Play() {
    controlledCar = std::make_shared<Game::Vehicle>();
    tilemap = std::make_unique<Game::Tilemap>();
}

void Game::Play::render(std::shared_ptr<Engine::Graphics> graphics) {
    tilemap->render(graphics.get());
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

    auto carTx = (controlledCar->pos.x() + 32) / 64;
    auto carTy = (controlledCar->pos.y() + 32) / 64;

    if (tilemap->get(carTx, carTy) == 2) {
        tilemap->set(carTx, carTy, 3);
    }

    controlledCar->update();
}

std::string Game::Play::toString() {
    return "Play";
}
