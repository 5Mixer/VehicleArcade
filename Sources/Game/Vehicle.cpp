#include "Vehicle.h"

const Game::Net::PlayerData Game::Vehicle::getData() {
    return Game::Net::PlayerData{
        id,
        Game::Net::Vec2{pos.x(), pos.y()},
        angle};
}

void Game::Vehicle::controlWithInput() {
    float accelerationSpeed = .005;
    float reverseSpeed = .004;
    float frictionFactor = .95;
    float maximumSpeed = .3;
    float turnRate = this->turnRateVelocityFactor(forwardsVelocity, maximumSpeed);
    if (Engine::Input::keysDown[Kore::KeyCode::KeyW]) {
        accelerate(accelerationSpeed);
    } else if (Engine::Input::keysDown[Kore::KeyCode::KeyS]) {
        accelerate(-reverseSpeed);
    } else {
        forwardsVelocity *= frictionFactor;
    }

    if (std::abs(forwardsVelocity) > maximumSpeed) {
        forwardsVelocity = maximumSpeed * (forwardsVelocity > 0 ? 1 : -1);
    }

    float turnSpeed = .03 * (0.1 + 0.9 * std::abs(forwardsVelocity) / std::abs(maximumSpeed));

    if (Engine::Input::keysDown[Kore::KeyCode::KeyA]) {
        turn(-turnSpeed * turnRate);
    }
    if (Engine::Input::keysDown[Kore::KeyCode::KeyD]) {
        turn(turnSpeed * turnRate);
    }
}

void Game::Vehicle::update(std::vector<Game::Wall> &walls, std::vector<Game::Vehicle> &vehicles, int worldWidth, int worldHeight) {
    float wallFrictionFactor = .95;

    visualAngleDelta = visualAngleDelta * .8;

    auto acc = Kore::vec2{std::cos(angle + visualAngleDelta * 20) * forwardsVelocity, std::sin(angle + visualAngleDelta * 20) * forwardsVelocity};

    auto collide = false;
    auto combinedRadius = 100;

    if (collide) {
        forwardsVelocity *= wallFrictionFactor;
    }

    velocity *= .965;
    velocity += acc;

    auto target = pos + velocity;

    for (const Game::Wall &wall : walls) {
        auto overlap = target - wall.pos;
        if (overlap.squareLength() < (combinedRadius * combinedRadius)) {

            overlap.setLength(combinedRadius);
            target = wall.pos + overlap;

            collide = true;
        }
    }
    for (const Game::Vehicle &vehicle : vehicles) {
        if (vehicle.id == id) {
            continue;
        }
        auto overlap = target - vehicle.pos;
        if (overlap.squareLength() != 0 && overlap.squareLength() < (combinedRadius * combinedRadius)) {

            overlap.setLength(combinedRadius);
            target = vehicle.pos + overlap;

            collide = true;
        }
    }

    target = Engine::constrainPointToRect(target, Kore::vec2{0, 0}, Kore::vec2{worldWidth, worldHeight});

    pos = target;
}

float Game::Vehicle::turnRateVelocityFactor(float speed, float maximumSpeed) {
    return 0.3 + 0.7 * (std::abs(speed) / maximumSpeed);
}

void Game::Vehicle::render(Engine::Graphics &g) {
    g.drawVehicle(pos, angle, visualAngleDelta * 40);
    g.drawText(pos + Kore::vec2{0, -100}, name);
}

void Game::Vehicle::accelerate(float acceleration) {
    forwardsVelocity += acceleration;
}
void Game::Vehicle::turn(float angleDelta) {
    angle += angleDelta;
    // visualAngleDelta = angleDelta;
    visualAngleDelta += (angleDelta - visualAngleDelta) / 10;
}

Kore::vec2 Game::Vehicle::getBackLeftWheelPos() {
    return Engine::Transform::rotate(angle, pos.x(), pos.y()) * Kore::vec3(pos.x() - 40, pos.y() - 35, 1);
}
Kore::vec2 Game::Vehicle::getBackRightWheelPos() {
    return Engine::Transform::rotate(angle, pos.x(), pos.y()) * Kore::vec3(pos.x() - 40, pos.y() + 35, 1);
}
Kore::vec2 Game::Vehicle::getFrontLeftWheelPos() {
    return Engine::Transform::rotate(angle, pos.x(), pos.y()) * Kore::vec3(pos.x() + 40, pos.y() - 35, 1);
}
Kore::vec2 Game::Vehicle::getFrontRightWheelPos() {
    return Engine::Transform::rotate(angle, pos.x(), pos.y()) * Kore::vec3(pos.x() + 40, pos.y() + 35, 1);
}