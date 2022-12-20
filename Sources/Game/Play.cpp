#include "Play.h"

Game::Play::Play() {
    controlledCar = std::make_shared<Game::Vehicle>();
    tilemap = std::make_unique<Game::Tilemap>();
}

void Game::Play::render(Engine::Graphics &graphics) {
    // TODO: Fix car shaking caused by int cast, which is for tilemap grid alignment.
    auto camera = Kore::mat3::Translation(int(Kore::System::windowWidth() / 2 - controlledCar->pos.x()), int(Kore::System::windowHeight() / 2 - controlledCar->pos.y()));
    graphics.transform(camera);

    tilemap->render(graphics);
    for (auto &entity : entities) {
        entity.pos.add(Kore::vec2(0, 1));
        entity.update();
        graphics.drawSprite(0, entity.pos);
    }
    controlledCar->render(graphics);

    graphics.transform(camera.Invert());
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
