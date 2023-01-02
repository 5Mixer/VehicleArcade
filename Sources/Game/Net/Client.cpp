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

    if (client->peerCount < 1) {
        receiver.onTimeout();
        return;
    }

    while (enet_host_service(client, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                std::cout << "Connected to server [" << event.peer->address.host << ":" << event.peer->address.port << "]" << std::endl;
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                receiver.onDisconnect();
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
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

void Game::Net::Client::sendPlayerMove(int x, int y, float angle) {
    Packet packet;
    packet.write(MessageType::PLAYER_MOVE);
    packet.writeU8(0); //playerID
    packet.writeU32LE(x);
    packet.writeU32LE(y);
    packet.writeU8(static_cast<std::uint8_t>(std::fmod(angle, 2 * 3.14) / (2 * 3.14) * 255));

    enet_host_broadcast(client, 0, packet.generate(ENET_PACKET_FLAG_UNSEQUENCED));
}

Game::Net::Client::~Client() {
    enet_deinitialize();
}