#include "Core.h"

namespace Engine {
    Core::Core() {
        this->init();
    }
    void Core::init() {
        Kore::System::init("Vehicle Arcade", 1024, 768);

        Engine::SpriteAtlas spriteAtlas{SpriteAtlas("sprites.json")};
        graphics = std::unique_ptr<Graphics>(new Graphics(spriteAtlas));

        Input::init();
    }

    void Core::setScene(Engine::Scene *const scene) {
        this->scene = scene;
    }

    void Core::start() {
        Kore::System::setCallback([]() {
            Engine::Core::getInstance().update();
        });

        Kore::System::start();
    }

    void Core::update() {
        scene->update();

        Kore::Graphics4::begin();
        Kore::Graphics4::clear(Kore::Graphics4::ClearColorFlag);

        graphics->begin();
        scene->render(*graphics.get());
        graphics->end();

        Kore::Graphics4::end();
        Kore::Graphics4::swapBuffers();
    }
} // namespace Engine
