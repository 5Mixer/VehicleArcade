#ifndef GAME_NET_CLIENT
#define GAME_NET_CLIENT

#include "../../enet.h"
#include "../Bullet.h"
#include "../Missile.h"
#include "../Wall.h"
#include "MessageReceiver.h"
#include <cmath>
#include <cstring>
#include <exception>
#include <iostream>

#include "PacketGenerated.h"

namespace Game {
    namespace Net {
        class Client {
        private:
            ENetHost *client;
            std::uint8_t id;
            bool ready = false;

        public:
            Client();
            ~Client();

            void sendPlayerMove(float x, float y, float angle);
            void sendPlayerShootBullet(Bullet &bullet);
            void sendPlayerShootMissile(Missile &missile);
            void sendPlayerPlaceWall(Wall &wall);
            void sendPlayerRegister(const std::string &name);
            std::uint8_t getId() { return id; };
            void disconnect();

            void service(MessageReceiver &receiver);
        };
    } // namespace Net
} // namespace Game

#endif
