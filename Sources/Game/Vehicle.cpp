#include "Vehicle.h"

void Game::Vehicle::update() {
    // pos.x() = Engine::Core::getInstance().rand() * 500;

    auto speed = 10;

    if (Engine::Input::keysDown[Kore::KeyCode::KeyW]) {
        pos.y() -= speed;
    }
    if (Engine::Input::keysDown[Kore::KeyCode::KeyA]) {
        pos.x() -= speed;
    }
    if (Engine::Input::keysDown[Kore::KeyCode::KeyS]) {
        pos.y() += speed;
    }
    if (Engine::Input::keysDown[Kore::KeyCode::KeyD]) {
        pos.x() += speed;
    }
}

void Game::Vehicle::render(Engine::Graphics *g) {
    g->drawSprite(0, pos);
}