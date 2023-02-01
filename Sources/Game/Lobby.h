#ifndef GAME_LOBBY
#define GAME_LOBBY

#include "../Engine/Graphics.h"
#include "../Engine/Input.h"
#include "../Engine/Scene.h"
#include "./Net/Client.h"
#include <Kore/Math/Vector.h>
#include <chrono>
#include <codecvt>
#include <fmt/format.h>
#include <iostream>
#include <locale>
#include <string>

namespace Game {
    class Lobby : public Engine::Scene {
    private:
        Game::Net::Client &client;
        std::string nameField{"Username"};

        std::chrono::time_point<std::chrono::system_clock>
            keyTime{std::chrono::system_clock::now()};
        bool fastKey = false;

    public:
        Lobby(Game::Net::Client &client) : client(client){};
        void render(Engine::Graphics &g);
        void update();
    };
} // namespace Game

#endif