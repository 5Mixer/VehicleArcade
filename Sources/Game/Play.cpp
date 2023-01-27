#include "Play.h"

Game::Play::Play(Game::Net::Client &client)
    : editingScene(false),
      client(client) {

    for (int i = 0; i < 10000; i++) {
        grass.push_back(Grass(
            Kore::vec2{
                Engine::Core::getInstance().rand() * 5000,
                Engine::Core::getInstance().rand() * 2000},
            Engine::Core::getInstance().rand() * 3.14 * 2,
            std::floor(Engine::Core::getInstance().rand() * static_cast<float>(4))
        ));
    }

    Kore::Mouse::the()->show(false);
}

void Game::Play::render(Engine::Graphics &graphics) {
    Game::Vehicle *controlledVehicle = getVehicleById(controlledVehicleId);
    if (controlledVehicle == nullptr) {
        return;
    }

    camera.pos = controlledVehicle->pos;
    if (camera.pos.x() < Kore::System::windowWidth() / 2) {
        camera.pos.x() = Kore::System::windowWidth() / 2;
    }
    if (camera.pos.y() < Kore::System::windowHeight() / 2) {
        camera.pos.y() = Kore::System::windowHeight() / 2;
    }
    if (camera.pos.x() > worldWidth - Kore::System::windowWidth() / 2) {
        camera.pos.x() = worldWidth - Kore::System::windowWidth() / 2;
    }
    if (camera.pos.y() > worldHeight - Kore::System::windowHeight() / 2) {
        camera.pos.y() = worldHeight - Kore::System::windowHeight() / 2;
    }

    graphics.transform(camera.getTransform());

    for (auto &singleGrass : grass) {
        singleGrass.render(graphics);
    }
    for (auto &trail : trails) {
        graphics.drawTrail(trail.pos);
    }
    for (auto &bullet : bullets) {
        bullet.render(graphics);
    }
    for (auto &missile : missiles) {
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

    for (auto &particle : particles) {
        particle.render(graphics);
    }

    graphics.transform(camera.getTransform().Invert());

    graphics.drawBar(Kore::vec2{40, 60}, controlledVehicle->health, controlledVehicle->maxHealth);
    graphics.drawCursor(Engine::Input::mousePosition);
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
    vehicles.push_back(Vehicle(packet->player()));
    controlledVehicleId = packet->player()->id();

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
    Game::Vehicle *vehicle = getVehicleById(packet->player());
    if (vehicle == nullptr) {
        std::cerr << "Could not find player referred to in player move packet" << std::endl;
        return;
    }
    vehicle->pos.x() = packet->pos()->x();
    vehicle->pos.y() = packet->pos()->y();
    vehicle->angle = packet->angle();
}
void Game::Play::onPlayerStatusMessage(const Net::PlayerStatus *packet) {
    Game::Vehicle *vehicle = getVehicleById(packet->player());
    if (vehicle == nullptr) {
        std::cerr << "Could not find player referred to in player move packet" << std::endl;
        return;
    }
    vehicle->health = static_cast<int>(packet->health());
};

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

// TODO: Deduplicate with missile shooting
void Game::Play::shootBullet() {
    if (std::chrono::steady_clock::now() < lastBulletShootTime + std::chrono::milliseconds(100)) {
        return;
    }

    Game::Vehicle *controlledVehicle = getVehicleById(controlledVehicleId);
    if (controlledVehicle == nullptr) {
        std::cerr << "Could not find player" << std::endl;
        return;
    }

    Kore::vec2 mouseWorldPos = getMouseWorldPos();
    float directAngle = atan2(mouseWorldPos.y() - controlledVehicle->pos.y(), mouseWorldPos.x() - controlledVehicle->pos.x());
    float angleRange = Engine::Core::getInstance().rand() - .5;

    Game::Bullet bullet{
        client.getId(),
        controlledVehicle->pos,
        directAngle + angleRange * .01f};

    auto startOffset = 80 + 20 * Engine::Core::getInstance().rand();
    bullet.pos += Kore::vec2{std::cos(bullet.angle) * startOffset, std::sin(bullet.angle) * startOffset};

    client.sendPlayerShootBullet(bullet);
    bullets.push_back(std::move(bullet));

    lastBulletShootTime = std::chrono::steady_clock::now();
}

void Game::Play::spawnParticle(Kore::vec2 pos) {
    particles.push_back(Particle{
        pos,
        Engine::Core::getInstance().rand() * (3.14 * 2)});
}

void Game::Play::shootMissile() {
    if (std::chrono::steady_clock::now() < lastBulletShootTime + std::chrono::milliseconds(400)) {
        return;
    }
    Game::Vehicle *controlledVehicle = getVehicleById(controlledVehicleId);
    if (controlledVehicle == nullptr) {
        std::cerr << "Could not find player" << std::endl;
        return;
    }

    Kore::vec2 mouseWorldPos = getMouseWorldPos();
    float directAngle = atan2(mouseWorldPos.y() - controlledVehicle->pos.y(), mouseWorldPos.x() - controlledVehicle->pos.x());
    float angleRange = Engine::Core::getInstance().rand() - .5;

    Game::Missile missile{
        client.getId(),
        controlledVehicle->pos,
        directAngle + angleRange * .005f};

    auto startOffset = 80 + 20 * Engine::Core::getInstance().rand();
    missile.pos += Kore::vec2{std::cos(missile.angle) * startOffset, std::sin(missile.angle) * startOffset};

    client.sendPlayerShootMissile(missile);
    missiles.push_back(std::move(missile));

    lastBulletShootTime = std::chrono::steady_clock::now();
}

void Game::Play::placeWall() {
    Kore::vec2 world = getMouseWorldPos();
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

Kore::vec2 Game::Play::getMouseWorldPos() {
    auto worldPos = camera.getTransform().Invert() * Kore::vec3{Engine::Input::mousePosition.x(), Engine::Input::mousePosition.y(), 1};
    return Kore::vec2{worldPos.x(), worldPos.y()}; // Drop irrelevent z homogeneous coordinate
}

void Game::Play::update() {
    client.service(*this);

    Game::Vehicle *controlledVehicle = getVehicleById(controlledVehicleId);
    if (controlledVehicle == nullptr) {
        return;
    }

    client.sendPlayerMove(controlledVehicle->pos.x(), controlledVehicle->pos.y(), controlledVehicle->angle);

    editingScene = Engine::Input::keysDown.at(Kore::KeyTab);

    Game::simulate(bullets, missiles, walls, vehicles);
    Game::updateVector(particles);
    Game::eraseDead(particles);

    controlledVehicle->controlWithInput();
    controlledVehicle->update(walls, worldWidth, worldHeight);

    for (Game::Vehicle &vehicle : vehicles) {
        trails.push_back(Game::Trail{vehicle.getBackLeftWheelPos()});
        trails.push_back(Game::Trail{vehicle.getBackRightWheelPos()});
        trails.push_back(Game::Trail{vehicle.getFrontLeftWheelPos()});
        trails.push_back(Game::Trail{vehicle.getFrontRightWheelPos()});
        spawnParticle(vehicle.pos);
    }

    if (editingScene) {
        if (Engine::Input::mouseDown) {
            placeWall();
        }
    } else {
        if (Engine::Input::mouseDown) {
            shootBullet();
        }
    }

    // Don't wait until the next frame to service network events
    client.service(*this);
}
