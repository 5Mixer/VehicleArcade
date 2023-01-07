#ifndef GAME_NET_CLIENT
#define GAME_NET_CLIENT

#include "../Bullet.h"
#include "Message.h"
#include "MessageReceiver.h"
#include <cmath>
#include <cstring>
#include <enet/enet.h>
#include <exception>
#include <iostream>

#include "PacketGenerated.h"

namespace Game {
    namespace Net {
        class Client {
        private:
            ENetHost *client;
            std::uint8_t id;

        public:
            Client();
            ~Client();

            void sendPlayerMove(float x, float y, float angle);
            void sendPlayerShoot(Bullet *bullet);

            void service(MessageReceiver &receiver);
        };
    } // namespace Net
} // namespace Game

#endif
