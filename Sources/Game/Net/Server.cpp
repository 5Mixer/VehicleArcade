#include "Server.h"

Game::Net::Server::Server() {
}

void Game::Net::Server::run() {
    if (enet_initialize() != 0) {
        std::cerr << "Could not initialise enet" << std::endl;
        exit(1);
    }

    ENetAddress address;

    address.host = ENET_HOST_ANY;
    address.port = 9320;

    const int allowedConnections = 64;
    const int channels = 2;
    const int incomingBandwidthLimit = 0;
    const int outgoingBandwidthLimit = 0;

    server = enet_host_create(&address, allowedConnections, channels, incomingBandwidthLimit, outgoingBandwidthLimit);

    if (server == nullptr) {
        std::cerr << "Could not setup server" << std::endl;
        exit(1);
    }

    std::cout << "Server initialised at " << address.host << ":" << address.port << std::endl;

    while (hosting) {
        service(*this);
    }
}

void Game::Net::Server::service(MessageReceiver &receiver) {
    ENetEvent event;

    while (enet_host_service(server, &event, 0) > 0) {
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
                receiver.onDisconnect();

                if (event.peer->data != nullptr) {
                    free(event.peer->data);
                }

                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                if (event.peer->data == nullptr) {
                    enet_peer_disconnect(event.peer, static_cast<std::uint32_t>(DisconnectReason::MESSAGE_BEFORE_JOIN));
                }

                // Don't trust the client to give their own playerID
                // TODO: Revisit if there is a better way to deal with player ID's, passing the peer, etc, while
                //       keeping MessageReceiver client applicable
                if (static_cast<MessageType>(event.packet->data[0]) == MessageType::PLAYER_MOVE) {
                    event.packet->data[1] = *static_cast<std::uint8_t *>(event.peer->data);
                }

                receiver.processRawPacket(event.packet->data, event.packet->dataLength);
                enet_packet_destroy(event.packet);

                break;
            }
            case ENET_EVENT_TYPE_NONE: {
                break;
            }
        }
    }
}

void Game::Net::Server::onPlayerMoveMessage(uint8_t playerId, float x, float y, float angle) {
    Packet packet;
    packet.write(MessageType::PLAYER_MOVE);
    packet.writeU8(playerId);  //playerID, as overriden server-side
    packet.writeU32LE(x * 10); // TODO: *10, /10 could be done outside MessageReceiver, or not at all?
    packet.writeU32LE(y * 10);
    packet.writeU8(static_cast<std::uint8_t>(angle / (2 * 3.14) * 255));

    enet_host_broadcast(server, 0, packet.generate(ENET_PACKET_FLAG_UNSEQUENCED));
}

ENetPacket *Game::Net::Server::createPlayerJoinPacket(std::uint8_t playerId) {
    uint8_t data[2] = {
        static_cast<std::uint8_t>(MessageType::PLAYER_JOIN), playerId};
    ENetPacket *packet = enet_packet_create(data, sizeof(data), ENET_PACKET_FLAG_RELIABLE);

    return packet;
}

void Game::Net::Server::kill() {
    for (size_t i = 0; i < server->peerCount; i++) {
        enet_peer_disconnect_now(&server->peers[i], 0);
    }
    enet_host_destroy(server);
    enet_deinitialize();
}

Game::Net::Server::~Server() {
    kill();
}
