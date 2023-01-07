#ifndef GAME_NET_SERVER
#define GAME_NET_SERVER

#include "../Vehicle.h"
#include "Message.h"
#include "MessageReceiver.h"
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

            ENetPacket *createPlayerJoinPacket(uint8_t playerId);

        public:
            Server();
            ~Server();
            void run();
            void kill();

            void onPlayerJoinDownloadMessage(const PlayerJoinDownload *packet){}; // server -> new client
            void onPlayerJoinMessage(uint8_t playerId){};                         // server -> client
            void onDisconnect(){};                                                // server -> client

            void onPlayerMoveMessage(const PlayerMove *packet);
        };
    } // namespace Net
} // namespace Game

#endif
