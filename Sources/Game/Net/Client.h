#ifndef GAME_NET_CLIENT
#define GAME_NET_CLIENT

#include "Message.h"
#include "MessageReceiver.h"
#include <cstring>
#include <enet/enet.h>
#include <exception>
#include <iostream>

namespace Game {
    namespace Net {
        class Client {
        private:
            ENetHost *client;

        public:
            Client();
            ~Client();

            void sendPlayerMove(int x, int y, float angle);

            void service(MessageReceiver &receiver);
        };
    } // namespace Net
} // namespace Game

#endif
