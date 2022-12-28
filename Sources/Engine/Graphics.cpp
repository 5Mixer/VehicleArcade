#include "Graphics.h"

namespace Engine {
    Engine::Graphics::Graphics()
        : graphics(std::unique_ptr<Kore::Graphics2::Graphics2>(new Kore::Graphics2::Graphics2(Kore::System::windowWidth(), Kore::System::windowHeight()))),
          texture(std::unique_ptr<Kore::Graphics4::Texture>(new Kore::Graphics4::Texture("../Assets/textures.png"))),
          grassTexture(std::unique_ptr<Kore::Graphics4::Texture>(new Kore::Graphics4::Texture("../Assets/grass.png"))) {
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
        graphics->clear(0x585685);
        graphics->drawScaledSubImage(grassTexture.get(), 0, 0, 1000, 600, 0, 0, 1000, 600);
    }
    void Engine::Graphics::drawSprite(int sprite, Kore::vec2 pos, float angle) {
        auto prior = graphics->transformation;
        auto rotation = rotate(angle, pos.x() + 8, pos.y() + 8);
        transform(rotation);
        graphics->drawScaledSubImage(texture.get(), sprite * 16, 0, 16, 16, pos.x(), pos.y(), 16, 16);
        graphics->transformation = prior;
    }
    void Engine::Graphics::transform(Kore::mat3 transformation) {
        graphics->transformation *= transformation;
    }
    Kore::mat3 Engine::Graphics::rotate(float angle, float centerx, float centery) {
        return (Kore::mat3::Translation(centerx, centery) * Kore::mat3::RotationZ(angle)) * Kore::mat3::Translation(-centerx, -centery);
    }
} // namespace Engine