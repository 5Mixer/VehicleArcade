#include "Vehicle.h"

void Game::Vehicle::update(std::vector<Game::Wall> &walls) {
    float accelerationSpeed = .3;
    float breakSpeed = .5;
    float turnSpeed = .07;
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
    for (const Game::Wall wall : walls) {
        auto overlap = target - wall.position;
        if (overlap.squareLength() < (10 * 10 * 4 * 4)) {

            overlap.setLength(40);
            target = wall.position + overlap;

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
    g.drawSprite(0, pos, angle + 3.14 / 2);
}

void Game::Vehicle::accelerate(float acceleration) {
    forwardsVelocity += acceleration;
}
void Game::Vehicle::turn(float angleDelta) {
    angle += angleDelta;
}
