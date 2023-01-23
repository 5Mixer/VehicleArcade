#ifndef GAME_PLAY_H
#define GAME_PLAY_H

#include "../Engine/Camera.h"
#include "../Engine/Graphics.h"
#include "../Engine/Scene.h"
#include "Bullet.h"
#include "Grass.h"
#include "Net/Client.h"
#include "Net/MessageReceiver.h"
#include "Simulation.h"
#include "Trail.h"
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

        int worldWidth = 5000;
        int worldHeight = 2000;

        std::vector<Game::Bullet> bullets;
        std::vector<Game::Missile> missiles;
        std::vector<Game::Wall> walls;
        std::vector<Game::Grass> grass;
        std::vector<Game::Vehicle> vehicles;
        std::vector<Game::Trail> trails;

        Game::Vehicle controlledCar;
        Engine::Camera camera;
        std::chrono::steady_clock::time_point lastBulletShootTime;

        void shootBullet();
        void shootMissile();
        void placeWall();

        void renderBars(Engine::Graphics &g);

        Kore::vec2 getMouseWorldPos();

        bool editingScene;

    public:
        Play(Game::Net::Client &client);
        void render(Engine::Graphics &g);
        void update();

        void onPlayerPlaceWallMessage(const Net::PlayerPlaceWall *packet);
        void onPlayerJoinMessage(const Net::PlayerJoin *packet);
        void onPlayerJoinDownloadMessage(const Net::PlayerJoinDownload *packet); // server -> new client
        void onPlayerDisconnectMessage(const Net::PlayerDisconnect *packet);
        void onPlayerMoveMessage(const Net::PlayerMove *packet);
        void onPlayerStatusMessage(const Net::PlayerStatus *packet);
        void onPlayerShootMessage(const Net::PlayerShoot *packet);
        void onPlayerShootMissileMessage(const Net::PlayerShootMissile *packet);
        void onDisconnect();
    };
} // namespace Game

#endif
