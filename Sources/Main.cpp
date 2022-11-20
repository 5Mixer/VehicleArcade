#include <Kore/IO/FileReader.h>
#include <Kore/System.h>
#include <limits>

#include "Engine/Engine.h"
#include "Engine/Input.h"

int kickstart(int argc, char **argv) {
    Kore::System::init("Vehicle Arcade", 1024, 768);
    Kore::System::setCallback(Engine::update);

    Engine::init();
    Kore::System::start();

    return 0;
}
