#include "Play.h"

Game::Play::Play(Game::Net::Client &client)
    : editingScene(false),
      controlledCar(Game::Vehicle(255, Kore::vec2(), 0)),
      client(client) {}

void Game::Play::render(Engine::Graphics &graphics) {
    camera.pos = controlledCar.pos;

    graphics.transform(camera.getTransform());

    graphics.drawTexture();
    for (auto &bullet : bullets) {
        bullet.update();
        bullet.render(graphics);
    }
    for (auto &missile : missiles) {
        missile.update();
        missile.render(graphics);
    }

    // Draw walls
    for (int layer = 0; layer < 3; layer++) {
        for (auto &wall : walls) {
            // graphics.drawSprite(5 + layer, wall.pos);
            graphics.drawWall(wall.pos, layer);
        }
    }

    for (auto &vehicle : vehicles) {
        vehicle.render(graphics);
    }

    controlledCar.render(graphics);

    graphics.transform(camera.getTransform().Invert());
}

void Game::Play::onPlayerJoinMessage(const Net::PlayerJoin *packet) {
    if (client.getId() == packet->player()->id()) {
        return;
    }

    vehicles.push_back(Vehicle(packet->player()));
}
void Game::Play::onPlayerDisconnectMessage(const Net::PlayerDisconnect *packet) {
    vehicles.erase(std::remove_if(vehicles.begin(), vehicles.end(), [&](Vehicle vehicle) -> bool {
                       return vehicle.id == packet->player();
                   }),
                   vehicles.end());
}
void Game::Play::onDisconnect() {
    std::cerr << "Disconnected from server" << std::endl;
    exit(1);
}

void Game::Play::onPlayerJoinDownloadMessage(const Net::PlayerJoinDownload *packet) {
    controlledCar = Vehicle(packet->player());

    for (auto playerData : *packet->players()) {
        vehicles.push_back(Vehicle(playerData));
    }
    for (const Net::BulletData *bulletData : *packet->bullets()) {
        bullets.push_back(Bullet(bulletData));
    }
    for (const Net::BulletData *missileData : *packet->missiles()) {
        missiles.push_back(Missile(missileData));
    }
    for (const Net::WallData *wallData : *packet->walls()) {
        walls.push_back(Wall(wallData));
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

    walls.push_back(Wall(packet->wall()));
}

void Game::Play::onPlayerShootMessage(const Net::PlayerShoot *packet) {
    if (client.getId() == packet->bullet()->shooter()) {
        return;
    }

    bullets.push_back(Bullet(packet->bullet()));
}

void Game::Play::onPlayerShootMissileMessage(const Net::PlayerShootMissile *packet) {
    if (client.getId() == packet->bullet()->shooter()) {
        return;
    }

    missiles.push_back(Missile(packet->bullet()));
}

void Game::Play::shootBullet() {
    if (std::chrono::steady_clock::now() < lastBulletShootTime + std::chrono::milliseconds(150)) {
        return;
    }

    float directAngle = atan2(Engine::Input::mousePosition.y() - Kore::System::windowHeight() / 2, Engine::Input::mousePosition.x() - Kore::System::windowWidth() / 2);
    float angleRange = Engine::Core::getInstance().rand() - .5;

    Game::Bullet bullet{
        client.getId(),
        controlledCar.pos,
        directAngle + angleRange * .01f};

    auto startOffset = 80 + 20 * Engine::Core::getInstance().rand();
    bullet.pos += Kore::vec2{std::cos(bullet.angle) * startOffset, std::sin(bullet.angle) * startOffset};

    client.sendPlayerShootBullet(bullet);
    bullets.push_back(std::move(bullet));

    lastBulletShootTime = std::chrono::steady_clock::now();
}

void Game::Play::shootMissile() {
    float directAngle = atan2(Engine::Input::mousePosition.y() - Kore::System::windowHeight() / 2, Engine::Input::mousePosition.x() - Kore::System::windowWidth() / 2);
    float angleRange = Engine::Core::getInstance().rand() - .5;

    Game::Missile missile{
        client.getId(),
        controlledCar.pos,
        directAngle + angleRange * .005f};

    auto startOffset = 80 + 20 * Engine::Core::getInstance().rand();
    missile.pos += Kore::vec2{std::cos(missile.angle) * startOffset, std::sin(missile.angle) * startOffset};

    client.sendPlayerShootMissile(missile);
    missiles.push_back(std::move(missile));
}

void Game::Play::placeWall() {
    Kore::vec2 world{camera.getWorldPos(Kore::vec3{Engine::Input::mousePosition.x() - 8 * camera.zoom, Engine::Input::mousePosition.y() - 8 * camera.zoom})};
    Kore::vec2 rounded{std::round(world.x() / 100) * 100, std::round(world.y() / 100) * 100};

    bool freeSpace = true;
    for (const Game::Wall wall : walls) {
        if (wall.pos.x() == rounded.x() && wall.pos.y() == rounded.y()) {
            freeSpace = false;
            break;
        }
    }

    if (freeSpace) {
        Game::Wall wall{client.getId(), rounded};
        client.sendPlayerPlaceWall(wall);
        walls.push_back(std::move(wall));
    }
}

void Game::Play::update() {
    client.service(*this);
    client.sendPlayerMove(controlledCar.pos.x(), controlledCar.pos.y(), controlledCar.angle);

    editingScene = Engine::Input::keysDown.at(Kore::KeyTab);

    Game::interactBulletsAndWalls(bullets, walls);
    Game::interactMissilesAndWalls(missiles, walls);

    controlledCar.update(walls);

    if (editingScene) {
        if (Engine::Input::mouseDown) {
            placeWall();
        }
    } else {
        if (Engine::Input::mouseDown) {
            shootBullet();
        }
    }
}

std::string Game::Play::toString() {
    return "Play";
}
