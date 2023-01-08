#include "Graphics.h"

namespace Engine {
    Engine::Graphics::Graphics()
        : graphics(std::unique_ptr<Kore::Graphics2::Graphics2>(new Kore::Graphics2::Graphics2(Kore::System::windowWidth(), Kore::System::windowHeight()))),
          texture(Kore::Graphics4::Texture("../Assets/textures.png", true)),
          carTexture(Kore::Graphics4::Texture("../Assets/car.png", true)),
          bulletTexture(Kore::Graphics4::Texture("../Assets/bullet.png", true)),
          grassTexture(Kore::Graphics4::Texture("../Assets/grass.png", true)) {
    }

    void Engine::Graphics::start() {
    }

    void Engine::Graphics::begin() {
        graphics->begin(false, Kore::System::windowWidth(), Kore::System::windowHeight());
    }
    void Engine::Graphics::end() {
        graphics->end();
    }
    void Engine::Graphics::drawTexture() {
        graphics->clear(0x217844);
        graphics->drawScaledSubImage(&grassTexture, 0, 0, 1000, 600, 0, 0, 1000, 600);
    }
    void Engine::Graphics::drawSprite(int sprite, Kore::vec2 pos, float angle) {
        auto prior = graphics->transformation;
        auto rotation = rotate(angle, pos.x() + 8, pos.y() + 8);
        transform(rotation);
        graphics->drawScaledSubImage(&texture, sprite * 16, 0, 16, 16, pos.x(), pos.y(), 16, 16);
        graphics->transformation = prior;
    }
    void Engine::Graphics::drawVehicle(Kore::vec2 pos, float angle) {
        auto prior = graphics->transformation;
        auto rotation = rotate(angle, pos.x(), pos.y());
        transform(rotation);
        graphics->drawScaledSubImage(&carTexture, 0, 0, 196, 100, pos.x() - 196 / 2, pos.y() - 100 / 2, 196, 100);
        graphics->transformation = prior;
    }
    void Engine::Graphics::drawBullet(Kore::vec2 pos, float angle) {
        graphics->drawScaledSubImage(&bulletTexture, 0, 0, 18, 18, pos.x() - 18 / 2, pos.y() - 18 / 2, 18, 18);
    }
    void Engine::Graphics::transform(Kore::mat3 transformation) {
        graphics->transformation *= transformation;
    }
    Kore::mat3 Engine::Graphics::rotate(float angle, float centerx, float centery) {
        return (Kore::mat3::Translation(centerx, centery) * Kore::mat3::RotationZ(angle)) * Kore::mat3::Translation(-centerx, -centery);
    }
} // namespace Engine