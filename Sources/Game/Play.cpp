#include "Play.h"

Game::Play::Play(Game::Net::Client &client)
    : editingScene(false),
      controlledCar(std::shared_ptr<Game::Vehicle>(new Game::Vehicle(255))),
      client(client) {}

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
            graphics.drawSprite(5 + layer, wall.pos);
        }
    }

    for (auto &vehicle : vehicles) {
        vehicle.render(graphics);
    }

    controlledCar->render(graphics);

    graphics.transform(camera.getTransform().Invert());
}

void Game::Play::onPlayerJoinMessage(const Net::PlayerJoin *packet) {
    if (client.getId() == packet->player()) {
        return;
    }

    std::cout << "New player with id " << static_cast<unsigned int>(packet->player()) << " joined" << std::endl;
    vehicles.push_back(Game::Vehicle{packet->player()});
}
void Game::Play::onDisconnect() {
    std::cerr << "Disconnected from server" << std::endl;
    exit(1);
}

void Game::Play::onPlayerJoinDownloadMessage(const Net::PlayerJoinDownload *packet) {
    controlledCar->id = packet->id();

    for (auto playerData : *packet->players()) {
        Vehicle newVehicle{playerData->id()};
        newVehicle.pos.x() = playerData->pos().x();
        newVehicle.pos.y() = playerData->pos().y();
        newVehicle.angle = playerData->angle();

        vehicles.push_back(newVehicle);
    }
    for (const Net::BulletData *bulletData : *packet->bullets()) {
        Bullet newBullet;
        newBullet.shooter = bulletData->shooter();
        newBullet.pos.x() = bulletData->pos().x();
        newBullet.pos.y() = bulletData->pos().y();
        newBullet.angle = bulletData->angle();

        bullets.push_back(newBullet);
    }
    for (const Net::WallData *wallData : *packet->walls()) {
        Wall newWall{Kore::vec2{wallData->pos().x(), wallData->pos().y()}};
        newWall.placer = wallData->placer();
        newWall.health = wallData->health();

        walls.push_back(newWall);
    }
}

void Game::Play::onPlayerMoveMessage(const Net::PlayerMove *packet) {
    if (client.getId() == packet->player()) {
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

void Game::Play::onPlayerPlaceWallMessage(const Net::PlayerPlaceWall *packet) {
    if (packet->wall()->placer() == client.getId()) {
        return;
    }

    Game::Wall wall{Kore::vec2{packet->wall()->pos().x(), packet->wall()->pos().y()}};
    wall.health = packet->wall()->health();
    wall.placer = packet->wall()->placer();
    walls.push_back(wall);
}

void Game::Play::onPlayerShootMessage(const Net::PlayerShoot *packet) {
    if (client.getId() == packet->player()) {
        return;
    }

    Game::Bullet bullet{};
    bullet.pos.x() = packet->pos()->x();
    bullet.pos.y() = packet->pos()->y();
    bullet.angle = packet->angle();

    bullets.push_back(bullet);
}

void Game::Play::shoot() {
    Game::Bullet bullet{};
    bullet.pos = controlledCar->pos;
    auto directAngle = atan2(Engine::Input::mousePosition.y() - Kore::System::windowHeight() / 2, Engine::Input::mousePosition.x() - Kore::System::windowWidth() / 2);
    auto angleRange = Engine::Core::getInstance().rand() - .5;
    bullet.angle = directAngle + angleRange * .01;

    auto startOffset = 20 * Engine::Core::getInstance().rand();
    bullet.pos += Kore::vec2{std::cos(bullet.angle) * startOffset, std::sin(bullet.angle) * startOffset};

    client.sendPlayerShoot(bullet);
    bullets.push_back(bullet);
}

void Game::Play::placeWall() {
    Kore::vec2 world{camera.getWorldPos(Kore::vec3{Engine::Input::mousePosition.x() - 8 * camera.zoom, Engine::Input::mousePosition.y() - 8 * camera.zoom})};
    Kore::vec2 rounded{std::round(world.x()), std::round(world.y())};

    bool freeSpace = true;
    for (const Game::Wall wall : walls) {
        if (wall.pos.x() == rounded.x() && wall.pos.y() == rounded.y()) {
            freeSpace = false;
            break;
        }
    }

    if (freeSpace) {
        Game::Wall wall{rounded};
        client.sendPlayerPlaceWall(wall);
        walls.push_back(wall);
    }
}

void Game::Play::update() {
    client.service(*this);
    client.sendPlayerMove(controlledCar->pos.x(), controlledCar->pos.y(), controlledCar->angle);

    editingScene = Engine::Input::keysDown.at(Kore::KeyTab);

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&](Game::Bullet bullet) -> bool {
                      // TODO: Avoid n^2
                      for (auto &wall : walls) {
                          if ((bullet.pos - wall.pos).squareLength() < std::pow(10, 2)) {
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
            placeWall();
        }
    } else {
        if (Engine::Input::mouseDown) {
            shoot();
        }
    }
}

std::string Game::Play::toString() {
    return "Play";
}
