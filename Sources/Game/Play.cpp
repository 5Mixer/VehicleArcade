#include "Play.h"

Game::Play::Play(Game::Net::Client &client)
    : editingScene(false),
      controlledCar(std::shared_ptr<Game::Vehicle>(new Game::Vehicle(255))),
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

    for (auto &vehicle : vehicles) {
        vehicle.render(graphics);
    }

    controlledCar->render(graphics);

    graphics.transform(camera.getTransform().Invert());
}

void Game::Play::onPlayerJoinMessage(uint8_t playerId) {
    if (controlledCar->id == playerId) {
        return;
    }

    std::cout << "New player with id " << static_cast<unsigned int>(playerId) << " joined" << std::endl;
    vehicles.push_back(Game::Vehicle{playerId});
}
void Game::Play::onDisconnect() {
    std::cerr << "Disconnected from server" << std::endl;
    exit(1);
}

void Game::Play::onPlayerJoinDownloadMessage(const Net::PlayerJoinDownload *packet) {
    std::cout << "Received player id assignment " << static_cast<unsigned int>(packet->id()) << std::endl;
    controlledCar->id = packet->id();

    for (auto playerData : *packet->players()) {
        std::cout << "Adding player id "
                  << playerData->id()
                  << " at "
                  << playerData->pos().x()
                  << ", "
                  << playerData->pos().y()
                  << ", angle: "
                  << playerData->angle()
                  << std::endl;

        Vehicle newVehicle = Vehicle{playerData->id()};
        newVehicle.pos.x() = playerData->pos().x();
        newVehicle.pos.y() = playerData->pos().y();
        newVehicle.angle = playerData->angle();

        vehicles.push_back(newVehicle);
    }
    for (const Net::BulletData *bulletData : *packet->bullets()) {
        Bullet newBullet = Bullet();
        newBullet.shooter = bulletData->shooter();
        newBullet.pos.x() = bulletData->pos().x();
        newBullet.pos.y() = bulletData->pos().y();
        newBullet.angle = bulletData->angle();

        bullets.push_back(newBullet);
    }
}

void Game::Play::onPlayerMoveMessage(const Net::PlayerMove *packet) {
    if (controlledCar->id == packet->player()) {
        return;
    }
    bool found = false;
    for (auto &vehicle : vehicles) {

        if (vehicle.id == packet->player()) {
            vehicle.pos.x() = packet->pos()->x();
            vehicle.pos.y() = packet->pos()->y();
            vehicle.angle = packet->angle();
            found = true;
            break;
        }
    }
}

void Game::Play::onPlayerShootMessage(const Net::PlayerShoot *packet) {
    if (controlledCar->id == packet->player()) {
        return;
    }

    auto bullet = Game::Bullet();
    bullet.pos.x() = packet->pos()->x();
    bullet.pos.y() = packet->pos()->y();
    bullet.angle = packet->angle();

    bullets.push_back(bullet);
}

void Game::Play::shoot() {
    auto bullet = Game::Bullet();
    bullet.pos = controlledCar->pos;
    auto directAngle = atan2(Engine::Input::mousePosition.y() - Kore::System::windowHeight() / 2, Engine::Input::mousePosition.x() - Kore::System::windowWidth() / 2);
    auto angleRange = Engine::Core::getInstance().rand() - .5;
    bullet.angle = directAngle + angleRange * .01;

    auto startOffset = 20 * Engine::Core::getInstance().rand();
    bullet.pos += Kore::vec2{std::cos(bullet.angle) * startOffset, std::sin(bullet.angle) * startOffset};

    bullets.push_back(bullet);

    client.sendPlayerShoot(&bullet);
}

void Game::Play::update() {

    client.service(*this);
    client.sendPlayerMove(controlledCar->pos.x(), controlledCar->pos.y(), controlledCar->angle);

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

            auto rounded = Kore::vec2{std::round(world.x()), std::round(world.y())};

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
            shoot();
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
