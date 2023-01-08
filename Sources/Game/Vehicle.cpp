#include "Vehicle.h"

const Game::Net::PlayerData Game::Vehicle::getData() {
    return Game::Net::PlayerData{
        id,
        Game::Net::Vec2{pos.x(), pos.y()},
        angle};
}

void Game::Vehicle::update(std::vector<Game::Wall> &walls) {
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

    auto target = pos + Kore::vec2{std::cos(angle) * forwardsVelocity, std::sin(angle) * forwardsVelocity};

    auto collide = false;
    auto combinedRadius = 70;
    for (const Game::Wall &wall : walls) {
        auto overlap = target - wall.pos;
        if (overlap.squareLength() < (combinedRadius * combinedRadius)) {

            overlap.setLength(combinedRadius);
            target = wall.pos + overlap;

            collide = true;
        }
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
    // g.drawSprite(0, pos, angle + 3.14 / 2);
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
