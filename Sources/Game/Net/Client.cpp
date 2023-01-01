#include "Client.h"

Game::Net::Client::Client() {
    if (enet_initialize() != 0) {
        std::cerr << "Could not initialise enet";
        exit(1);
    }

    client = enet_host_create(nullptr, 1, 2, 0, 0);

    if (client == NULL) {
        std::cerr << "An error occurred while trying to create an ENet client host." << std::endl;
        exit(1);
    }

    ENetAddress address;
    ENetEvent event;
    ENetPeer *peer;
    enet_address_set_host(&address, "localhost");
    address.port = 9320;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == nullptr) {
        std::cerr << "No available peers for initiating an ENet connection." << std::endl;
        exit(1);
    }

    /* Wait for the connection attempt to succeed. */
    if (enet_host_service(client, &event, 2000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connected to server" << std::endl;
    } else {
        enet_peer_reset(peer);

        std::cerr << "Failed to connect to server" << std::endl;
        exit(1);
    }
}

void Game::Net::Client::service(MessageReceiver &receiver) {
    ENetEvent event;

    while (enet_host_service(client, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                std::cout << "Connected to server [" << event.peer->address.host << ":" << event.peer->address.port << "]" << std::endl;
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                switch (event.packet->data[0]) {
                    case static_cast<std::uint8_t>(MessageType::PLAYER_JOIN): {
                        receiver.onPlayerJoinMessage(event.packet->data[1]);
                        break;
                    }
                    case static_cast<std::uint8_t>(MessageType::PLAYER_MOVE): {
                        receiver.onPlayerMoveMessage(
                            event.packet->data[1],
                            int((unsigned char)(event.packet->data[2 + 0]) << 24 |
                                (unsigned char)(event.packet->data[2 + 1]) << 16 |
                                (unsigned char)(event.packet->data[2 + 2]) << 8 |
                                (unsigned char)(event.packet->data[2 + 3])),
                            int((unsigned char)(event.packet->data[6 + 0]) << 24 |
                                (unsigned char)(event.packet->data[6 + 1]) << 16 |
                                (unsigned char)(event.packet->data[6 + 2]) << 8 |
                                (unsigned char)(event.packet->data[6 + 3])),
                            float((unsigned char)(event.packet->data[10 + 0]) << 24 |
                                  (unsigned char)(event.packet->data[10 + 1]) << 16 |
                                  (unsigned char)(event.packet->data[10 + 2]) << 8 |
                                  (unsigned char)(event.packet->data[10 + 3])));
                        break;
                    }
                    default: {
                        std::cerr << "Received unknown message type " << static_cast<unsigned int>(event.packet->data[0]);
                        exit(1);
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

void Game::Net::Client::sendPlayerMove(int x, int y, float angle) {
    int8_t data[1 + 1 + 2 * sizeof(int) + sizeof(float)];

    data[0] = static_cast<std::uint8_t>(MessageType::PLAYER_MOVE);
    data[1] = 0; // playerID
    std::memcpy(&data[2], &x, 4);
    std::memcpy(&data[2 + 4], &y, 4);
    std::memcpy(&data[2 + 4 + 4], &angle, 4);

    ENetPacket *packet = enet_packet_create(data, sizeof(data), ENET_PACKET_FLAG_UNSEQUENCED);

    enet_host_broadcast(client, 0, packet);
}

Game::Net::Client::~Client() {
    enet_deinitialize();
}