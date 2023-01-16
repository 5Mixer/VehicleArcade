#ifndef GAME_NET_SERVER
#define GAME_NET_SERVER

#include "../../enet.h"
#include "../Bullet.h"
#include "../Simulation.h"
#include "../Vehicle.h"
#include "../Wall.h"
#include "MessageReceiver.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <thread>
#include <unordered_map>

namespace Game {
    namespace Net {
        class Server : public MessageReceiver {
        private:
            bool hosting = true;
            ENetHost *server;
            int serviceBlockTimeMs = 200;
            void service(MessageReceiver &receiver);
            std::uint8_t nextPlayerId = 0;

            std::unordered_map<std::uint8_t, Game::Vehicle> vehicles{};
            std::vector<Game::Bullet> bullets{};
            std::vector<Game::Missile> missiles{};
            std::vector<Game::Wall> walls{};

            ENetPacket *createPlayerJoinPacket(Game::Vehicle vehicle);
            void updateEntities();

        public:
            Server();
            ~Server();
            void run();
            void kill();

            void onPlayerJoinDownloadMessage(const PlayerJoinDownload *packet){}; // server -> new client
            void onPlayerJoinMessage(const PlayerJoin *packet){};                 // server -> client
            void onDisconnect(){};                                                // server -> client
            void onPlayerDisconnectMessage(const PlayerDisconnect *packet){};     // server -> client

            void onPlayerPlaceWallMessage(const PlayerPlaceWall *packet);
            void onPlayerMoveMessage(const PlayerMove *packet);
            void onPlayerShootMessage(const PlayerShoot *packet);
            void onPlayerShootMissileMessage(const PlayerShootMissile *packet);
        };
    } // namespace Net
} // namespace Game

#endif
