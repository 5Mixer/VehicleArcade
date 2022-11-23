#include <Kore/IO/FileReader.h>
#include <Kore/System.h>
#include <limits>
#include <memory>

#include "Engine/Engine.h"
#include "Engine/Input.h"
#include "Play.h"

std::unique_ptr<Engine::Engine> engine;

int kickstart(int argc, char **argv) {
    Kore::System::init("Vehicle Arcade", 1024, 768);

    engine = std::make_unique<Engine::Engine>();
    engine->scene = std::make_shared<Play>();
    engine->init();

    Kore::System::setCallback([] {
        engine->update();
    });
    Kore::System::setShutdownCallback([] {
        delete engine.get();
    });

    Kore::System::start();

    return 0;
}
