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

            std::vector<Game::Vehicle> vehicles{};
            std::vector<Game::Bullet> bullets{};
            std::vector<Game::Missile> missiles{};
            std::vector<Game::Wall> walls{};
            int worldWidth = 5000;
            int worldHeight = 2000;

            Game::Vehicle *getVehicleById(std::uint8_t id);

            ENetPacket *createPlayerJoinPacket(Game::Vehicle vehicle);
            void updateEntities();

            template <class U, class T, typename Alloc = std::allocator<T>>
            std::vector<U> getDataOfVector(std::vector<T, Alloc> elements) {
                std::vector<U> elementsData;
                for (T &element : elements) {
                    elementsData.push_back(element.getData());
                }
                return elementsData;
            }

        public:
            Server();
            ~Server();
            void run();
            void kill();

            void onPlayerJoinDownloadMessage(const PlayerJoinDownload *packet){}; // server -> new client
            void onPlayerJoinMessage(const PlayerJoin *packet){};                 // server -> client
            void onDisconnect(){};                                                // server -> client
            void onPlayerDisconnectMessage(const PlayerDisconnect *packet){};     // server -> client
            void onPlayerStatusMessage(const PlayerStatus *packet){};             // server -> client

            void onPlayerPlaceWallMessage(const PlayerPlaceWall *packet);
            void onPlayerMoveMessage(const PlayerMove *packet);
            void onPlayerShootMessage(const PlayerShoot *packet);
            void onPlayerShootMissileMessage(const PlayerShootMissile *packet);

            void sendPlayerStatus(std::uint8_t playerId, std::uint8_t health);
        };
    } // namespace Net
} // namespace Game

#endif
