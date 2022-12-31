#include <Kore/IO/FileReader.h>
#include <Kore/System.h>
#include <limits>
#include <memory>

#include "Engine/Core.h"
#include "Engine/Input.h"
#include "Game/Net/Server.h"
#include "Game/Play.h"

#include <Kore/Graphics2/Graphics.h>

int kickstart(int argc, char **argv) {
    if (argc == 2 && argv[1] == std::string("server")) {
        Game::Net::Server(); // TODO: Might stack alloc be problematic if server memory usage large?
        return 0;
    }
    Engine::Core &engine = Engine::Core::getInstance();
    engine.setScene(std::make_shared<Game::Play>());
    engine.start();

    return 0;
}
