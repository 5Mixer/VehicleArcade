#include "Server.h"
#include <memory>

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
                auto playerId = nextPlayerId++;

                uint8_t joinResponseData[2] = {
                    static_cast<std::uint8_t>(MessageType::PLAYER_JOIN_DOWNLOAD), playerId};
                ENetPacket *joinResponsePacket = enet_packet_create(joinResponseData, sizeof(joinResponseData), ENET_PACKET_FLAG_RELIABLE);

                enet_peer_send(event.peer, 0, joinResponsePacket);

                std::cout << "Client connected ["
                          << event.peer->address.host
                          << ":"
                          << event.peer->address.port
                          << "] playerID: "
                          << static_cast<unsigned int>(playerId)
                          << std::endl;

                event.peer->data = malloc(sizeof(std::uint8_t));
                *(std::uint8_t *)(event.peer->data) = playerId;

                enet_host_broadcast(server, 0, createPlayerJoinPacket(playerId));

                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                free(event.peer->data);
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                switch (event.packet->data[0]) {
                    case static_cast<std::uint8_t>(MessageType::PLAYER_JOIN): {
                        // receiver.onPlayerJoinMessage(event.packet->data[1]);
                        break;
                    }
                    case static_cast<std::uint8_t>(MessageType::PLAYER_MOVE): {

                        // receiver.onPlayerJoinMessage(event.packet->data[1]);
                        (event.packet->data)[1] = *(std::uint8_t *)(event.peer->data);
                        enet_host_broadcast(server, 0, event.packet); // TODO: Remove this hack!
                        enet_host_flush(server);
                        return;
                        break;
                    }
                    default: {
                        std::cerr << "Received unknown message type " << static_cast<unsigned int>(event.packet->data[0]) << std::endl;
                        break;
                    }
                }
                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_NONE: {
                break;
            }
        }
    }
}

ENetPacket *Game::Net::Server::createPlayerJoinPacket(std::uint8_t playerId) {
    uint8_t data[2] = {
        static_cast<std::uint8_t>(MessageType::PLAYER_JOIN), playerId};
    ENetPacket *packet = enet_packet_create(data, sizeof(data), ENET_PACKET_FLAG_RELIABLE);

    return packet;
}

Game::Net::Server::~Server() {
    enet_host_destroy(server);
    enet_deinitialize();
}
