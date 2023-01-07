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

void Game::Net::Client::disconnect() {
    enet_peer_disconnect_now(&(client->peers[0]), 0);
    enet_host_destroy(client);
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
                auto packet = GetPacket(event.packet->data);

                if (packet->type_type() == PacketType::PlayerJoinDownload) {
                    id = packet->type_as_PlayerJoinDownload()->player()->id();
                    ready = true;
                }

                receiver.processRawPacket(packet);
                enet_packet_destroy(event.packet);

                break;
            }
            case ENET_EVENT_TYPE_NONE: {
                break;
            }
        }
    }
}

void Game::Net::Client::sendPlayerMove(float x, float y, float angle) {
    if (!ready) {
        return;
    }

    flatbuffers::FlatBufferBuilder builder{50};

    auto pos = Vec2{x, y};
    auto move = CreatePlayerMove(builder, id, &pos, angle);
    auto packet = CreatePacket(builder, PacketType::PlayerMove, move.Union());

    builder.Finish(packet);

    auto netPacket = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_UNSEQUENCED);
    enet_host_broadcast(client, 0, netPacket);
}

void Game::Net::Client::sendPlayerShoot(Bullet &bullet) {
    flatbuffers::FlatBufferBuilder builder{50};

    auto bulletData = bullet.getData();
    auto shoot = CreatePlayerShoot(builder, &bulletData);
    auto packet = CreatePacket(builder, PacketType::PlayerShoot, shoot.Union());

    builder.Finish(packet);

    auto netPacket = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(client, 0, netPacket);
}

void Game::Net::Client::sendPlayerPlaceWall(Wall &wall) {
    flatbuffers::FlatBufferBuilder builder{50};

    auto wallData = wall.getData();
    auto wallPacketSerialisation = CreatePlayerPlaceWall(builder, &wallData);
    auto packet = CreatePacket(builder, PacketType::PlayerPlaceWall, wallPacketSerialisation.Union());

    builder.Finish(packet);

    auto netPacket = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(client, 0, netPacket);
}

Game::Net::Client::~Client() {
    enet_deinitialize();
}