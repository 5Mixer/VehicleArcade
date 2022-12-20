#include "Vehicle.h"

void Game::Vehicle::update() {
    float accelerationSpeed = .5;
    float breakSpeed = .5;
    float turnSpeed = .07;
    float frictionFactor = .9;
    float maximumSpeed = 25;

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

    if (std::abs(forwardsVelocity) > 5) {
        forwardsVelocity = 5 * (forwardsVelocity > 0 ? 1 : -1);
    }

    pos += Kore::vec2{std::cos(angle) * forwardsVelocity, std::sin(angle) * forwardsVelocity};
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
