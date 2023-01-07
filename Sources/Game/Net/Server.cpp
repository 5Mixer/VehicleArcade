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

                flatbuffers::FlatBufferBuilder builder{50};

                // create vector of players to be sent in PlayerJoinDownload packet
                std::vector<PlayerData> vehicleData;
                for (auto &vehicle : vehicles) {
                    vehicleData.push_back(PlayerData{
                        vehicle.first,
                        Vec2{
                            vehicle.second.pos.x(),
                            vehicle.second.pos.y()},
                        vehicle.second.angle});
                }
                auto vectorOfVehicleData = builder.CreateVectorOfStructs(vehicleData);

                auto packet = CreatePacket(
                    builder,
                    PacketType::PlayerJoinDownload,
                    CreatePlayerJoinDownload(
                        builder,
                        playerId,
                        vectorOfVehicleData
                    )
                        .Union()
                );
                builder.Finish(packet);
                auto netPacket = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(event.peer, 0, netPacket);

                std::cout << "Client connected ["
                          << event.peer->address.host
                          << ":"
                          << event.peer->address.port
                          << "] playerID: "
                          << static_cast<unsigned int>(playerId)
                          << std::endl;

                event.peer->data = malloc(sizeof(std::uint8_t));
                *(std::uint8_t *)(event.peer->data) = playerId;

                vehicles.insert({playerId, Vehicle{playerId}});

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
                std::uint8_t peerPlayerId = *(std::uint8_t *)event.peer->data;

                auto deserialisedPacket = GetMutablePacket(event.packet->data);

                // Drop packets where the client is dishonest about their player ID
                // This should be more efficient than overriding the player ID (requiring unpacking, mutation, repacking)
                // Whilst having no downside cost - clients should know their assigned player ID as soon as they receive
                // their player join download packet
                bool playerIdValid = true;
                switch (deserialisedPacket->type_type()) {
                    case PacketType::PlayerMove: {
                        playerIdValid = deserialisedPacket->type_as_PlayerMove()->player() == peerPlayerId;
                        break;
                    }
                    case PacketType::PlayerShoot: {
                        playerIdValid = deserialisedPacket->type_as_PlayerShoot()->player() == peerPlayerId;
                        break;
                    }
                    default: {
                    }
                }

                if (!playerIdValid) {
                    break;
                }

                receiver.processRawPacket(*deserialisedPacket);
                enet_packet_destroy(event.packet);

                break;
            }
            case ENET_EVENT_TYPE_NONE: {
                break;
            }
        }
    }
}

void Game::Net::Server::onPlayerShootMessage(const PlayerShoot *packet) {
    // Broadcast shoot to other players
    flatbuffers::FlatBufferBuilder builder{50};

    auto pos = Vec2{packet->pos()->x(), packet->pos()->y()};
    auto shoot = CreatePlayerShoot(builder, packet->player(), &pos, packet->angle());
    auto outboundPacketData = CreatePacket(builder, PacketType::PlayerShoot, shoot.Union());

    builder.Finish(outboundPacketData);

    auto outboundPacket = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_UNSEQUENCED);

    enet_host_broadcast(server, 0, outboundPacket);
}

void Game::Net::Server::onPlayerMoveMessage(const PlayerMove *packet) {
    // update server side state for player location
    Vehicle &vehicle = vehicles.at(packet->player());
    vehicle.pos.x() = packet->pos()->x();
    vehicle.pos.y() = packet->pos()->y();
    vehicle.angle = packet->angle();

    // Broadcast move to other players
    flatbuffers::FlatBufferBuilder builder{50};

    auto pos = Vec2{packet->pos()->x(), packet->pos()->y()};
    auto move = CreatePlayerMove(builder, packet->player(), &pos, packet->angle());
    auto outboundPacketData = CreatePacket(builder, PacketType::PlayerMove, move.Union());

    builder.Finish(outboundPacketData);

    auto outboundPacket = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_UNSEQUENCED);

    enet_host_broadcast(server, 0, outboundPacket);
}

ENetPacket *Game::Net::Server::createPlayerJoinPacket(std::uint8_t playerId) {
    flatbuffers::FlatBufferBuilder builder{50};
    auto packet = CreatePacket(builder, PacketType::PlayerJoin, CreatePlayerJoin(builder, playerId).Union());
    builder.Finish(packet);
    return enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_RELIABLE);
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
