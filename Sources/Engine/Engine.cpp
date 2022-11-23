#include "Engine.h"

#include <stdio.h>

namespace Engine {
    Engine::Engine() : g(Kore::Graphics2::Graphics2(Kore::System::windowWidth(), Kore::System::windowHeight())) {
    }

    void Engine::start() {
    }

    void Engine::update() {
        scene->update();

        Kore::Graphics4::begin();
        Kore::Graphics4::clear(Kore::Graphics4::ClearColorFlag);

        g.begin(false, Kore::System::windowWidth(), Kore::System::windowHeight());
        scene->render(g);
        g.end();

        Kore::Graphics4::end();
        Kore::Graphics4::swapBuffers();
    }
} // namespace Engine
