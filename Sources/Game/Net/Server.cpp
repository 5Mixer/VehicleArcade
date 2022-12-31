#include "Server.h"

Game::Net::Server::Server() {
    if (enet_initialize() != 0) {
        std::cerr << "Could not initialise enet" << std::endl;
        exit(1);
    }

    ENetAddress address;

    address.host = ENET_HOST_ANY;
    address.port = 9320;

    const int allowedConnections = 8;
    const int channels = ENET_PROTOCOL_MAXIMUM_CHANNEL_COUNT;
    const int incomingBandwidthLimit = 0;
    const int outgoingBandwidthLimit = 0;

    server = enet_host_create(&address, allowedConnections, channels, incomingBandwidthLimit, outgoingBandwidthLimit);

    if (server == nullptr) {
        std::cerr << "Could not setup server" << std::endl;
        exit(1);
    }

    std::cout << "Server initialised at " << address.host << ":" << address.port << std::endl;

    while (hosting) {
        service();
    }
}

void Game::Net::Server::service() {
    ENetEvent event;

    if (enet_host_service(server, &event, serviceBlockTimeMs)) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                std::cout << "Client connected [" << event.peer->address.host << ":" << event.peer->address.port << "]" << std::endl;
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                break;
            }
            case ENET_EVENT_TYPE_NONE: {
                break;
            }
        }
    }
}

Game::Net::Server::~Server() {
    enet_host_destroy(server);
    enet_deinitialize();
}
