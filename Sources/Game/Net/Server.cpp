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

    // std::cout << "Server initialised at " << address.host << ":" << address.port << std::endl;

    auto last = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now());

    while (hosting) {
        service(*this);

        auto now = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now());

        if (std::chrono::duration_cast<std::chrono::milliseconds>((now - last)).count() > 1000 / 60) {
            updateEntities();
            last = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now());
        }
    }
}

void Game::Net::Server::updateEntities() {
    for (auto &bullet : bullets) {
        bullet.update();
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
                    vehicleData.push_back(vehicle.second.getData());
                }
                auto vectorOfVehicleData = builder.CreateVectorOfStructs(vehicleData);

                // create vector of bullets to be sent in PlayerJoinDownload packet
                std::vector<BulletData> bulletData;
                for (auto &bullet : bullets) {
                    bulletData.push_back(bullet.getData());
                }
                auto vectorOfBulletData = builder.CreateVectorOfStructs(bulletData);

                // create vector of walls to be sent in PlayerJoinDownload packet
                std::vector<WallData> wallData;
                for (auto &wall : walls) {
                    wallData.push_back(wall.getData());
                }
                auto vectorOfWallData = builder.CreateVectorOfStructs(wallData);

                Vehicle newVehicle{playerId, Kore::vec2{}, 0};

                auto playerData = newVehicle.getData();

                auto packet = CreatePacket(
                    builder,
                    PacketType::PlayerJoinDownload,
                    CreatePlayerJoinDownload(
                        builder,
                        &playerData,
                        vectorOfVehicleData,
                        vectorOfBulletData,
                        vectorOfWallData
                    )
                        .Union()
                );
                builder.Finish(packet);
                auto netPacket = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(event.peer, 0, netPacket);

                // Associate the network peer with it's playerId, on the heap. Deleted on disconnect.
                event.peer->data = new std::uint8_t(playerId);

                vehicles.insert({playerId, newVehicle});

                enet_host_broadcast(server, 0, createPlayerJoinPacket(newVehicle));

                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                receiver.onDisconnect();

                vehicles.erase(*(std::uint8_t *)event.peer->data);

                flatbuffers::FlatBufferBuilder builder{50};
                auto packet = CreatePacket(
                    builder,
                    PacketType::PlayerDisconnect,
                    CreatePlayerDisconnect(
                        builder,
                        *(std::uint8_t *)event.peer->data
                    )
                        .Union()
                );
                builder.Finish(packet);
                auto netPacket = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_RELIABLE);
                enet_host_broadcast(server, 0, netPacket);

                if (event.peer->data != nullptr) {
                    delete (std::uint8_t *)event.peer->data;
                }

                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                if (event.peer->data == nullptr) {
                    // Reject packets from peers without an assigned playerId
                    enet_peer_disconnect(event.peer, 0);
                    break;
                }
                std::uint8_t peerPlayerId = *(std::uint8_t *)event.peer->data;

                auto deserialisedPacket = GetPacket(event.packet->data);

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
                        playerIdValid = deserialisedPacket->type_as_PlayerShoot()->bullet()->shooter() == peerPlayerId;
                        break;
                    }
                    case PacketType::PlayerPlaceWall: {
                        playerIdValid = deserialisedPacket->type_as_PlayerPlaceWall()->wall()->placer() == peerPlayerId;
                        break;
                    }
                    case PacketType::PlayerDisconnect: {
                        playerIdValid = deserialisedPacket->type_as_PlayerDisconnect()->player() == peerPlayerId;
                        break;
                    }
                    default: {
                    }
                }

                if (!playerIdValid) {
                    std::cout << "Warning: Received packet " << int(deserialisedPacket->type_type()) << " with invalid playerId" << std::endl;
                    break;
                }

                receiver.processRawPacket(deserialisedPacket);

                if (deserialisedPacket->type_type() == PacketType::PlayerDisconnect) {
                    enet_peer_disconnect(event.peer, 0);
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

void Game::Net::Server::onPlayerPlaceWallMessage(const PlayerPlaceWall *packet) {
    // update server side state for wall location
    walls.push_back(Wall(packet->wall()));

    // Broadcast placement of wall to other players
    flatbuffers::FlatBufferBuilder builder{50};

    auto placeWall = CreatePlayerPlaceWall(builder, packet->wall());
    auto outboundPacketData = CreatePacket(builder, PacketType::PlayerPlaceWall, placeWall.Union());
    builder.Finish(outboundPacketData);

    auto outboundPacket = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_UNSEQUENCED);

    enet_host_broadcast(server, 0, outboundPacket);
}

void Game::Net::Server::onPlayerShootMessage(const PlayerShoot *packet) {
    // update server side state for bullet location
    bullets.push_back(Bullet(packet->bullet()));

    // Broadcast shoot to other players
    flatbuffers::FlatBufferBuilder builder{50};

    auto shoot = CreatePlayerShoot(builder, packet->bullet());
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

ENetPacket *Game::Net::Server::createPlayerJoinPacket(Vehicle vehicle) {
    flatbuffers::FlatBufferBuilder builder{50};
    auto playerData = vehicle.getData();

    auto packet = CreatePacket(
        builder,
        PacketType::PlayerJoin,
        CreatePlayerJoin(
            builder,
            &playerData
        )
            .Union()
    );
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
