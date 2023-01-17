#include "Graphics.h"

#include <iostream>

namespace Engine {
    Engine::Graphics::Graphics()
        : graphics(std::unique_ptr<Kore::Graphics2::Graphics2>(new Kore::Graphics2::Graphics2(Kore::System::windowWidth(), Kore::System::windowHeight()))),
          texture(Kore::Graphics4::Texture("../Assets/textures.png")),
          carTexture(Kore::Graphics4::Texture("../Assets/car.png")),
          bulletTexture(Kore::Graphics4::Texture("../Assets/bullet.png")),
          missileTexture(Kore::Graphics4::Texture("../Assets/missile.png")),
          wall1Texture(Kore::Graphics4::Texture("../Assets/wall1.png")),
          wall2Texture(Kore::Graphics4::Texture("../Assets/wall2.png")),
          wall3Texture(Kore::Graphics4::Texture("../Assets/wall3.png")),
          wheelTexture(Kore::Graphics4::Texture("../Assets/carWheel.png")),
          grassTexture(Kore::Graphics4::Texture("../Assets/grass.png")) {
    }

    void Engine::Graphics::start() {
    }

    void Engine::Graphics::begin() {
        graphics->begin(false, Kore::System::windowWidth(), Kore::System::windowHeight());
        graphics->clear(0x217844);
    }
    void Engine::Graphics::end() {
        graphics->end();
    }
    void Engine::Graphics::drawSprite(int sprite, Kore::vec2 pos, float angle) {
        auto prior = graphics->transformation;
        auto rotation = rotate(angle, pos.x() + 8, pos.y() + 8);
        transform(rotation);
        graphics->drawScaledSubImage(&texture, sprite * 16, 0, 16, 16, pos.x(), pos.y(), 16, 16);
        graphics->transformation = prior;
    }
    void Engine::Graphics::drawGrass(Kore::vec2 pos, float angle, std::uint8_t variety) {
        const Kore::vec2 screenPos = graphics->transformation * Kore::vec3{pos.x(), pos.y(), 1};
        const int buffer = 20;
        if (screenPos.x() < -buffer || screenPos.y() < -buffer || screenPos.x() > Kore::System::windowWidth() + buffer || screenPos.y() > Kore::System::windowHeight() + buffer) {
            return;
        }
        graphics->drawScaledSubImage(&grassTexture, variety * 20, 0, 20, 20, pos.x() - 20 / 2, pos.y() - 20 / 2, 20, 20);
    }
    void Engine::Graphics::drawVehicle(Kore::vec2 pos, float angle, float angleDelta) {
        auto prior = graphics->transformation;
        auto rotation = rotate(angle, pos.x(), pos.y());
        transform(rotation);
        graphics->drawScaledSubImage(&carTexture, 0, 0, 196, 100, pos.x() - 196 / 2, pos.y() - 100 / 2, 196, 100);

        auto leftWheelPos = rotation * Kore::vec3(pos.x() + 40, pos.y() - 35, 1);
        auto rightWheelPos = rotation * Kore::vec3(pos.x() + 40, pos.y() + 35, 1);

        auto leftRotation = rotate(angle + angleDelta, leftWheelPos.x(), leftWheelPos.y());
        graphics->transformation = prior * (leftRotation);
        graphics->drawScaledSubImage(&wheelTexture, 0, 0, 30, 15, leftWheelPos.x() - 30 / 2, leftWheelPos.y() - 15 / 2, 30, 15);

        auto rightRotation = rotate(angle + angleDelta, rightWheelPos.x(), rightWheelPos.y());
        graphics->transformation = prior * (rightRotation);
        graphics->drawScaledSubImage(&wheelTexture, 0, 0, 30, 15, rightWheelPos.x() - 30 / 2, rightWheelPos.y() - 15 / 2, 30, 15);

        graphics->transformation = prior;
    }
    void Engine::Graphics::drawBullet(Kore::vec2 pos, float angle) {
        graphics->drawScaledSubImage(&bulletTexture, 0, 0, 18, 18, pos.x() - 18 / 2, pos.y() - 18 / 2, 18, 18);
    }
    void Engine::Graphics::drawMissile(Kore::vec2 pos, float angle) {
        auto prior = graphics->transformation;
        auto rotation = rotate(angle, pos.x(), pos.y());
        transform(rotation);
        graphics->drawImage(&missileTexture, pos.x() - 196 / 2, pos.y() - 100 / 2);
        graphics->transformation = prior;
    }
    void Engine::Graphics::drawWall(Kore::vec2 pos, int layer) {
        if (layer == 0)
            graphics->drawScaledSubImage(&wall1Texture, 0, 0, 96, 103, pos.x() - 96 / 2, pos.y() - 103 / 2, 96, 103);
        if (layer == 1)
            graphics->drawScaledSubImage(&wall2Texture, 0, 0, 96, 103, pos.x() - 96 / 2, pos.y() - 103 / 2, 96, 103);
        if (layer == 2)
            graphics->drawScaledSubImage(&wall3Texture, 0, 0, 96, 103, pos.x() - 96 / 2, pos.y() - 103 / 2, 96, 103);
    }
    void Engine::Graphics::transform(Kore::mat3 transformation) {
        graphics->transformation *= transformation;
    }
    Kore::mat3 Engine::Graphics::rotate(float angle, float centerx, float centery) {
        return (Kore::mat3::Translation(centerx, centery) * Kore::mat3::RotationZ(angle)) * Kore::mat3::Translation(-centerx, -centery);
    }
} // namespace Engine