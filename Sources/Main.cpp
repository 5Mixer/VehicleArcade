#include <Kore/IO/FileReader.h>
#include <Kore/System.h>
#include <csignal>
#include <limits>
#include <memory>

#include "Engine/Core.h"
#include "Engine/Input.h"
#include "Game/Net/Client.h"
#include "Game/Net/Server.h"
#include "Game/Play.h"

#include <Kore/Graphics2/Graphics.h>

Game::Net::Server *server = nullptr;
Game::Net::Client *client = nullptr;

void onSigInt(int sig) {
    if (client != nullptr) {
        client->disconnect();
    } else {
        server->kill();
    }
    exit(EXIT_SUCCESS);
}

void disconnectClient() {
    client->disconnect();
}

int kickstart(int argc, char **argv) {
    signal(SIGINT, onSigInt);

    if (argc == 2 && argv[1] == std::string("server")) {
        server = new Game::Net::Server();
        server->run(); // Blocking
        return 0;
    }

    client = new Game::Net::Client();
    Engine::Core &engine = Engine::Core::getInstance();

    Kore::System::setShutdownCallback(disconnectClient);

    auto playScene = std::unique_ptr<Game::Play>(new Game::Play(*client));
    engine.setScene(playScene.get());
    engine.start();

    return 0;
}
