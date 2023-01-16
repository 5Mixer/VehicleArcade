#ifndef GAME_PLAY_H
#define GAME_PLAY_H

#include "../Engine/Camera.h"
#include "../Engine/Graphics.h"
#include "../Engine/Scene.h"
#include "Bullet.h"
#include "Net/Client.h"
#include "Net/MessageReceiver.h"
#include "Simulation.h"
#include "Vehicle.h"
#include "Wall.h"
#include <Kore/IO/FileReader.h>
#include <Kore/IO/FileWriter.h>
#include <Kore/Math/Matrix.h>
#include <algorithm>
#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace Game {
    class Play : public Engine::Scene, public Game::Net::MessageReceiver {
    private:
        Game::Net::Client &client;

        std::vector<Game::Bullet> bullets;
        std::vector<Game::Missile> missiles;
        std::vector<Game::Wall> walls;
        std::vector<Game::Vehicle> vehicles;
        Game::Vehicle controlledCar;
        Engine::Camera camera;
        std::chrono::steady_clock::time_point lastBulletShootTime;

        void shootBullet();
        void shootMissile();
        void placeWall();

        bool editingScene;

    public:
        Play(Game::Net::Client &client);
        void render(Engine::Graphics &g);
        void update();
        std::string toString();

        void onPlayerPlaceWallMessage(const Net::PlayerPlaceWall *packet);
        void onPlayerJoinMessage(const Net::PlayerJoin *packet);
        void onPlayerJoinDownloadMessage(const Net::PlayerJoinDownload *packet); // server -> new client
        void onPlayerDisconnectMessage(const Net::PlayerDisconnect *packet);
        void onPlayerMoveMessage(const Net::PlayerMove *packet);
        void onPlayerShootMessage(const Net::PlayerShoot *packet);
        void onPlayerShootMissileMessage(const Net::PlayerShootMissile *packet);
        void onDisconnect();
    };
} // namespace Game

#endif
