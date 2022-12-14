#include "Graphics.h"

namespace Engine {
    Engine::Graphics::Graphics()
        : graphics(std::unique_ptr<Kore::Graphics2::Graphics2>(new Kore::Graphics2::Graphics2(Kore::System::windowWidth(), Kore::System::windowHeight()))),
          texture(std::unique_ptr<Kore::Graphics4::Texture>(new Kore::Graphics4::Texture("../Assets/textures.png"))) {
    }

    void Engine::Graphics::start() {
    }

    void Engine::Graphics::begin() {
        graphics->begin(false, Kore::System::windowWidth(), Kore::System::windowHeight());
    }
    void Engine::Graphics::end() {
        graphics->end();
    }
    void Engine::Graphics::drawSprite(int sprite, Kore::vec2 pos, float angle) {
        auto prior = graphics->transformation;
        graphics->pushRotation(angle, pos.x() + 32, pos.y() + 32);
        graphics->drawScaledSubImage(texture.get(), 0, 0, 16, 16, pos.x(), pos.y(), 16 * 4, 16 * 4);
        graphics->transformation = prior;
    }
} // namespace Engine