#ifndef GAME_NET_SERVER
#define GAME_NET_SERVER

#include "Message.h"
#include "MessageReceiver.h"
#include "Packet.h"
#include <cstdint>
#include <enet/enet.h>
#include <iostream>
#include <memory>

namespace Game {
    namespace Net {
        class Server : public MessageReceiver {
        private:
            bool hosting = true;
            ENetHost *server;
            int serviceBlockTimeMs = 200;
            void service(MessageReceiver &receiver);
            std::uint8_t nextPlayerId = 0;

            ENetPacket *createPlayerJoinPacket(uint8_t playerId);

        public:
            Server();
            ~Server();
            void run();
            void kill();

            void onPlayerJoinDownloadMessage(uint8_t playerId){}; // server -> new client
            void onPlayerJoinMessage(uint8_t playerId){};         // server -> client
            void onDisconnect(){};                                // server -> client

            void onPlayerMoveMessage(PacketPlayerMove &packet);
        };
    } // namespace Net
} // namespace Game

#endif
