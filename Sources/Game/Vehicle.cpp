#include "Vehicle.h"

const Game::Net::PlayerData Game::Vehicle::getData() {
    return Game::Net::PlayerData{
        id,
        Game::Net::Vec2{pos.x(), pos.y()},
        angle};
}

void Game::Vehicle::update(std::vector<Game::Wall> &walls, int worldWidth, int worldHeight) {
    float accelerationSpeed = .1;
    float breakSpeed = .1;
    float turnSpeed = .04;
    float frictionFactor = .99;
    float wallFrictionFactor = .95;
    float maximumSpeed = 10;

    if (Engine::Input::keysDown[Kore::KeyCode::KeyW]) {
        accelerate(accelerationSpeed);
    } else if (Engine::Input::keysDown[Kore::KeyCode::KeyS]) {
        accelerate(-breakSpeed);
    } else {
        forwardsVelocity *= frictionFactor;
    }

    float turnRate = this->turnRateVelocityFactor(forwardsVelocity, maximumSpeed);
    visualAngleDelta = visualAngleDelta * .8;
    if (Engine::Input::keysDown[Kore::KeyCode::KeyA]) {
        turn(-turnSpeed * turnRate);
    }
    if (Engine::Input::keysDown[Kore::KeyCode::KeyD]) {
        turn(turnSpeed * turnRate);
    }

    if (std::abs(forwardsVelocity) > maximumSpeed) {
        forwardsVelocity = maximumSpeed * (forwardsVelocity > 0 ? 1 : -1);
    }

    auto target = pos + Kore::vec2{std::cos(angle + visualAngleDelta * 20) * forwardsVelocity, std::sin(angle + visualAngleDelta * 20) * forwardsVelocity};

    auto collide = false;
    auto combinedRadius = 100;
    for (const Game::Wall &wall : walls) {
        auto overlap = target - wall.pos;
        if (overlap.squareLength() < (combinedRadius * combinedRadius)) {

            overlap.setLength(combinedRadius);
            target = wall.pos + overlap;

            collide = true;
        }
    }
    if (target.x() < 0) {
        target.x() = 0;
    }
    if (target.y() < 0) {
        target.y() = 0;
    }
    if (target.x() > worldWidth) {
        target.x() = worldWidth;
    }
    if (target.y() > worldHeight) {
        target.y() = worldHeight;
    }
    if (collide) {
        forwardsVelocity *= wallFrictionFactor;
    }
    pos = target;
}

float Game::Vehicle::turnRateVelocityFactor(float speed, float maximumSpeed) {
    return 0.3 + 0.7 * (speed / maximumSpeed);
}

void Game::Vehicle::render(Engine::Graphics &g) {
    g.drawVehicle(pos, angle, visualAngleDelta * 20);
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