#ifndef GAME_NET_SERVER
#define GAME_NET_SERVER

#include "Message.h"
#include <cstdint>
#include <enet/enet.h>
#include <iostream>
#include <memory>

namespace Game {
    namespace Net {
        class Server {
        private:
            bool hosting = true;
            ENetHost *server;
            int serviceBlockTimeMs = 200;
            void service();
            std::uint8_t nextPlayerId = 0;

            ENetPacket *createPlayerJoinPacket(uint8_t playerId);

        public:
            Server();
            ~Server();
            void run();
            void kill();
        };
    } // namespace Net
} // namespace Game

#endif
