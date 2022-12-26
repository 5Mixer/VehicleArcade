#include "Play.h"

Game::Play::Play()
    : editingScene(true),
      tilemap(std::make_unique<Game::Tilemap>()),
      controlledCar(std::make_shared<Game::Vehicle>()),
      reader() {

    auto reader = Kore::FileReader();

    if (reader.open("Save", Kore::FileReader::Save)) {
        int wallCount = reader.readU32LE();

        for (int i = 0; i < wallCount; i++) {
            int x = reader.readS32LE();
            int y = reader.readS32LE();
            auto wall = new Game::Wall{Kore::vec2{float(x), float(y)}};
            walls.push_back(*wall);
        }
        reader.close();
    }
}

void Game::Play::render(Engine::Graphics &graphics) {
    // TODO: Fix car shaking caused by int cast, which is for tilemap grid alignment.
    auto camera = Kore::mat3::Translation(int(Kore::System::windowWidth() / 2 - controlledCar->pos.x()), int(Kore::System::windowHeight() / 2 - controlledCar->pos.y()));
    graphics.transform(camera);

    graphics.drawTexture();
    for (auto &bullet : bullets) {
        bullet.update();
        bullet.render(graphics);
    }
    {
        for (int layer = 0; layer < 3; layer++) {
            for (auto &wall : walls) {
                graphics.drawSprite(5 + layer, wall.position);
            }
        }
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

    if (editingScene) {
        if (Engine::Input::mouseDown) {
            auto rounded = Kore::vec2{
                static_cast<int>(Engine::Input::mousePosition.x() / 4) * 4,
                static_cast<int>(Engine::Input::mousePosition.y() / 4) * 4};

            auto wall = new Game::Wall{rounded};
            walls.push_back(*wall);

            save(); // TODO: Optimise saving
        }
    } else {
        if (Engine::Input::mouseDown) {
            auto bullet = new Game::Bullet();
            bullet->pos = controlledCar->pos;
            bullet->angle = atan2(Engine::Input::mousePosition.y() - Kore::System::windowHeight() / 2, Engine::Input::mousePosition.x() - Kore::System::windowWidth() / 2);
            bullets.push_back(*bullet);
        }
    }
}

void Game::Play::save() {
    auto writer = Kore::FileWriter("./Save");
    writer.open("Save");

    writer.writeU32LE(walls.size());
    for (auto &wall : walls) {
        writer.writeS32LE(wall.position.x());
        writer.writeS32LE(wall.position.y());
    }

    writer.close();
}

std::string Game::Play::toString() {
    return "Play";
}
