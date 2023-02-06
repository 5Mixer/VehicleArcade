#ifndef GAME_PLAY_H
#define GAME_PLAY_H

#include "../Engine/Camera.h"
#include "../Engine/Graphics.h"
#include "../Engine/Scene.h"
#include "../enet.h"
#include "Bullet.h"
#include "Collectable.h"
#include "Grass.h"
#include "Net/Client.h"
#include "Net/MessageReceiver.h"
#include "Particle.h"
#include "Simulation.h"
#include "Trail.h"
#include "Vehicle.h"
#include "Wall.h"
#include <Kore/Audio1/Audio.h>
#include <Kore/IO/FileReader.h>
#include <Kore/IO/FileWriter.h>
#include <Kore/Math/Matrix.h>
#include <Kore/Math/Vector.h>
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
        std::vector<Game::Particle> particles;
        std::vector<Game::Collectable> collectables;

        Kore::Sound missileShootSound{"../Assets/sounds/impactPunch_medium_004.ogg"};

        std::chrono::time_point<std::chrono::system_clock>
            endOfRoundTime{std::chrono::system_clock::now() + std::chrono::seconds{3 * 60}};

        std::uint8_t controlledVehicleId = 255;
        Engine::Camera camera;
        std::chrono::steady_clock::time_point lastBulletShootTime;

        Game::Vehicle *getVehicleById(std::uint8_t id) {
            for (auto &vehicle : vehicles) {
                if (vehicle.id == id) {
                    return &vehicle;
                }
            }
            return nullptr;
        }

        void shootBullet();
        void shootMissile();
        void placeWall();
        void spawnParticle(Kore::vec2 pos);

        void renderBars(Engine::Graphics &g);

        Kore::vec2 getMouseWorldPos();

        bool editingScene;

    public:
        Play(Game::Net::Client &client, std::string name);
        void render(Engine::Graphics &g);
        void update();

        void onPlayerPlaceWallMessage(const Net::PlayerPlaceWall *packet);
        void onPlayerJoinMessage(const Net::PlayerJoin *packet);
        void onPlayerRegisterMessage(const Net::PlayerRegister *packet, ENetPeer &peer){}; // client -> server
        void onPlayerJoinDownloadMessage(const Net::PlayerJoinDownload *packet);           // server -> new client
        void onPlayerDisconnectMessage(const Net::PlayerDisconnect *packet);
        void onPlayerMoveMessage(const Net::PlayerMove *packet);
        void onPlayerStatusMessage(const Net::PlayerStatus *packet);
        void onPlayerShootMessage(const Net::PlayerShoot *packet);
        void onPlayerShootMissileMessage(const Net::PlayerShootMissile *packet);
        void onSpawnCollectableMessage(const Net::SpawnCollectable *packet);
        void onDisconnect();
    };
} // namespace Game

#endif
