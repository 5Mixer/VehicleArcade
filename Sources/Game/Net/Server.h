#ifndef GAME_NET_SERVER
#define GAME_NET_SERVER

#include "../Bullet.h"
#include "../Vehicle.h"
#include "../Wall.h"
#include "MessageReceiver.h"
#include <chrono>
#include <cstdint>
#include <enet/enet.h>
#include <iostream>
#include <memory>
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
            std::vector<Game::Wall> walls{};

            ENetPacket *createPlayerJoinPacket(uint8_t playerId);
            void updateEntities();

        public:
            Server();
            ~Server();
            void run();
            void kill();

            void onPlayerJoinDownloadMessage(const PlayerJoinDownload *packet){}; // server -> new client
            void onPlayerJoinMessage(const PlayerJoin *packet){};                 // server -> client
            void onDisconnect(){};                                                // server -> client

            void onPlayerPlaceWallMessage(const PlayerPlaceWall *packet);
            void onPlayerMoveMessage(const PlayerMove *packet);
            void onPlayerShootMessage(const PlayerShoot *packet);
        };
    } // namespace Net
} // namespace Game

#endif
