#include "Vehicle.h"

void Game::Vehicle::update(std::vector<Game::Wall> &walls) {
    float accelerationSpeed = .3;
    float breakSpeed = .5;
    float turnSpeed = .07;
    float frictionFactor = .99;
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
        if ((target - wall.position).squareLength() < (10 * 10 * 4 * 4)) {

            auto d = (wall.position - pos).getLength();
            auto theta = std::atan2(target.y() - pos.y(), target.x() - pos.x()) - std::atan2(wall.position.y() - pos.y(), wall.position.x() - pos.x());
            auto t = d * std::cos(theta) - std::sqrt(std::pow(10 + 10, 2) - std::pow(d, 2) * std::pow(std::sin(theta), 2));

            auto tVec = (target - pos);
            tVec.multiply(t);
            target = pos + tVec;

            collide = true;

            ////
            /*
            auto collision = target - wall.position;

            auto correction = collision;
            correction.setLength(10);

            auto newTarget = target + correction;

            auto newTargetFromPos = newTarget - pos;
            // auto angleToNewTarget = std::atan2(newTargetFromPos.y(), newTargetFromPos.x());

            // forwardsVelocity =

            // angle = angleToNewTarget;
            target = newTarget; //pos + Kore::vec2{std::cos(angle) * forwardsVelocity, std::sin(angle) * forwardsVelocity};
            // break;

            // target = newTarget;

            // collide = true;
            // break;
            */
        }
    }
    if (!collide) {
        pos = target;
    } else {
        forwardsVelocity = .5;
    }
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
