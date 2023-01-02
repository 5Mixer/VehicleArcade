#ifndef GAME_PLAY_H
#define GAME_PLAY_H

#include "../Engine/Camera.h"
#include "../Engine/Graphics.h"
#include "../Engine/Scene.h"
#include "Bullet.h"
#include "Net/Client.h"
#include "Net/MessageReceiver.h"
#include "Net/Packet.h"
#include "Vehicle.h"
#include "Wall.h"
#include <Kore/IO/FileReader.h>
#include <Kore/IO/FileWriter.h>
#include <Kore/Math/Matrix.h>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace Game {
    class Play : public Engine::Scene, public Game::Net::MessageReceiver {
    private:
        Game::Net::Client &client;

        std::vector<Game::Bullet> bullets;
        std::vector<Game::Wall> walls;
        std::vector<Game::Vehicle> vehicles;
        std::shared_ptr<Game::Vehicle> controlledCar;
        Engine::Camera camera;

        bool editingScene;
        void save();

    public:
        Play(Game::Net::Client &client);
        void render(Engine::Graphics &g);
        void update();
        std::string toString();

        void onPlayerJoinMessage(uint8_t playerId);
        void onPlayerMoveMessage(Net::PacketPlayerMove &packet);
        void onPlayerJoinDownloadMessage(uint8_t playerId); // server -> new client
        void onDisconnect();
    };
} // namespace Game

#endif
