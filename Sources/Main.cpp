#include <Kore/IO/FileReader.h>
#include <Kore/System.h>
#include <limits>
#include <memory>

#include "Engine/Core.h"
#include "Engine/Input.h"
#include "Game/Net/Client.h"
#include "Game/Net/Server.h"
#include "Game/Play.h"

#include <Kore/Graphics2/Graphics.h>

int kickstart(int argc, char **argv) {
    if (argc == 2 && argv[1] == std::string("server")) {
        Game::Net::Server();
        return 0;
    }

    std::unique_ptr<Game::Net::Client> client = std::make_unique<Game::Net::Client>();

    Engine::Core &engine = Engine::Core::getInstance();

    auto playScene = std::unique_ptr<Game::Play>(new Game::Play(*(client.get())));
    engine.setScene(playScene.get());
    engine.start();

    return 0;
}
