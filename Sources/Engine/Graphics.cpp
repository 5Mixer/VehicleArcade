#include "Graphics.h"

namespace Engine {
    Engine::Graphics::Graphics() : graphics(std::unique_ptr<Kore::Graphics2::Graphics2>(new Kore::Graphics2::Graphics2(Kore::System::windowWidth(), Kore::System::windowHeight()))) {
    }

    void Engine::Graphics::start() {
    }

    void Engine::Graphics::begin() {
        graphics->begin(false, Kore::System::windowWidth(), Kore::System::windowHeight());
    }
    void Engine::Graphics::end() {
        graphics->end();
    }
    void Engine::Graphics::drawSprite(int x, int y) {
        graphics->fillRect(x, y, 10, 10);
    }
} // namespace Engine