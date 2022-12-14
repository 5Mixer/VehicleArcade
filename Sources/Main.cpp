#include <Kore/IO/FileReader.h>
#include <Kore/System.h>
#include <limits>
#include <memory>

#include "Engine/Core.h"
#include "Engine/Input.h"
#include "Game/Play.h"

#include <Kore/Graphics2/Graphics.h>

int kickstart(int argc, char **argv) {
    Engine::Core &engine = Engine::Core::getInstance();
    engine.setScene(std::make_shared<Game::Play>());
    engine.start();

    return 0;
}
