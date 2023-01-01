#include "Play.h"

Game::Play::Play(Game::Net::Client &client)
    : editingScene(false),
      controlledCar(std::make_shared<Game::Vehicle>()),
      client(client) {

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
    camera.pos = controlledCar->pos;

    graphics.transform(camera.getTransform());

    graphics.drawTexture();
    for (auto &bullet : bullets) {
        bullet.update();
        bullet.render(graphics);
    }

    // Draw walls
    for (int layer = 0; layer < 3; layer++) {
        for (auto &wall : walls) {
            graphics.drawSprite(5 + layer, wall.position);
        }
    }

    controlledCar->render(graphics);

    graphics.transform(camera.getTransform().Invert());
}

void Game::Play::onPlayerJoinMessage(uint8_t playerId) {
    std::cout << "New player with id " << static_cast<unsigned int>(playerId) << " joined" << std::endl;
}

void Game::Play::update() {

    client.service(*this);

    editingScene = Engine::Input::keysDown.at(Kore::KeyTab);

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&](Game::Bullet bullet) -> bool {
                      // TODO: Avoid n^2
                      for (auto &wall : walls) {
                          if ((bullet.pos - wall.position).squareLength() < std::pow(10, 2)) {
                              wall.health--;
                              return true;
                          }
                      }
                      return false;
                  }),
                  bullets.end());

    walls.erase(std::remove_if(walls.begin(), walls.end(), [&](Game::Wall wall) -> bool {
                    return wall.health <= 0;
                }),
                walls.end());

    controlledCar->update(walls);

    if (editingScene) {
        if (Engine::Input::mouseDown) {
            auto world = camera.getWorldPos(Kore::vec3{Engine::Input::mousePosition.x() - 8 * camera.zoom, Engine::Input::mousePosition.y() - 8 * camera.zoom});

            auto rounded = Kore::vec2{static_cast<int>(world.x()), static_cast<int>(world.y())};

            bool freeSpace = true;
            for (const Game::Wall wall : walls) {
                if (wall.position.x() == rounded.x() && wall.position.y() == rounded.y()) {
                    freeSpace = false;
                    break;
                }
            }

            if (freeSpace) {
                walls.push_back(Game::Wall{rounded});

                save(); // TODO: Optimise saving
            }
        }
    } else {
        if (Engine::Input::mouseDown) {
            auto bullet = new Game::Bullet();
            bullet->pos = controlledCar->pos;
            auto directAngle = atan2(Engine::Input::mousePosition.y() - Kore::System::windowHeight() / 2, Engine::Input::mousePosition.x() - Kore::System::windowWidth() / 2);
            auto angleRange = Engine::Core::getInstance().rand() - .5;
            bullet->angle = directAngle + angleRange * .01;

            auto startOffset = 20 * Engine::Core::getInstance().rand();
            bullet->pos += Kore::vec2{std::cos(bullet->angle) * startOffset, std::sin(bullet->angle) * startOffset};

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
