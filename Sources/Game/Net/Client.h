#ifndef GAME_NET_CLIENT
#define GAME_NET_CLIENT

#include "Message.h"
#include "MessageReceiver.h"
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

            void service(MessageReceiver &receiver);
        };
    } // namespace Net
} // namespace Game

#endif
