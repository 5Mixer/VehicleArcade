#ifndef GAME_NET_SERVER
#define GAME_NET_SERVER

#include <enet/enet.h>
#include <iostream>

namespace Game {
    namespace Net {
        class Server {
        private:
            bool hosting = true;
            int serviceBlockTimeMs = 200;
            ENetHost *server;
            void service();

        public:
            Server();
            ~Server();
        };
    } // namespace Net
} // namespace Game

#endif
