#include "Play.h"

Game::Play::Play()
    : editingScene(false),
      tilemap(std::make_unique<Game::Tilemap>()),
      controlledCar(std::make_shared<Game::Vehicle>()) {}

void Game::Play::render(Engine::Graphics &graphics) {
    // TODO: Fix car shaking caused by int cast, which is for tilemap grid alignment.
    auto camera = Kore::mat3::Translation(int(Kore::System::windowWidth() / 2 - controlledCar->pos.x()), int(Kore::System::windowHeight() / 2 - controlledCar->pos.y()));
    graphics.transform(camera);

    // tilemap->render(graphics);
    graphics.drawTexture();
    for (auto &bullet : bullets) {
        bullet.update();
        bullet.render(graphics);
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

    controlledCar->update();

    if (Engine::Input::mouseDown) {
        auto bullet = new Game::Bullet();
        bullet->pos = controlledCar->pos;
        bullet->angle = atan2(Engine::Input::mousePosition.y() - Kore::System::windowHeight() / 2, Engine::Input::mousePosition.x() - Kore::System::windowWidth() / 2);
        bullets.push_back(*bullet);
    }
}

std::string Game::Play::toString() {
    return "Play";
}
