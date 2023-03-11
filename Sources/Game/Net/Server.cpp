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

    auto last = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now());

    while (hosting) {
        service(*this);

        updateEntities();

        // spawnCollectable();

        // std::this_thread::sleep_until(last + std::chrono::duration<float>(1000 / 60.));
        std::this_thread::sleep_for(std::chrono::duration<float>(1 / 60.));

        last = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now());
    }
}

void Game::Net::Server::updateEntities() {
    Game::simulate(bullets, missiles, walls, vehicles, collectables);

    for (Game::Vehicle &vehicle : vehicles) {
        sendPlayerStatus(vehicle.id, vehicle.health);
    }
}

void Game::Net::Server::spawnCollectable() {
    Collectable collectable{
        Kore::vec2{
            rand() * worldWidth,
            rand() * worldHeight},
        rand() > .5 ? Game::CollectableType::Coin : Game::CollectableType::Health};

    flatbuffers::FlatBufferBuilder builder{50};
    const auto collectableData = collectable.getData();

    auto packetData = CreatePacket(
        builder,
        PacketType::SpawnCollectable,
        CreateSpawnCollectable(
            builder,
            &collectableData
        )
            .Union()
    );
    builder.Finish(packetData);

    auto packet = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(server, 0, packet);

    collectables.push_back(std::move(collectable));
}

void Game::Net::Server::service(MessageReceiver &receiver) {
    ENetEvent event;

    while (enet_host_service(server, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                auto playerId = nextPlayerId++;

                // Associate the network peer with it's playerId, on the heap. Deleted on disconnect.
                event.peer->data = new std::uint8_t(playerId);

                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                receiver.onDisconnect();

                vehicles.erase(std::remove_if(vehicles.begin(), vehicles.end(), [&](Game::Vehicle vehicle) -> bool {
                                   return vehicle.id == *(std::uint8_t *)event.peer->data;
                               }),
                               vehicles.end());

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

                receiver.processRawPacket(deserialisedPacket, *event.peer);

                auto ptype = deserialisedPacket->type_type();
                if (ptype == PacketType::PlayerShoot ||
                    ptype == PacketType::PlayerStatus ||
                    ptype == PacketType::PlayerPlaceWall ||
                    ptype == PacketType::PlayerShootMissile ||
                    ptype == PacketType::PlayerMove) {
                    // broadcast packet
                    auto outboundPacket = enet_packet_create(
                        event.packet->data,
                        event.packet->dataLength,
                        // TODO: More elegant assignment of packet flags
                        ptype == PacketType::PlayerMove ? ENET_PACKET_FLAG_UNSEQUENCED : ENET_PACKET_FLAG_RELIABLE
                    );

                    enet_host_broadcast(server, 0, outboundPacket);
                }

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

void Game::Net::Server::sendPlayerStatus(std::uint8_t playerId, std::uint8_t health) {
    flatbuffers::FlatBufferBuilder builder{50};

    auto playerStatus = CreatePlayerStatus(builder, playerId, health);
    auto outboundPacketData = CreatePacket(builder, PacketType::PlayerStatus, playerStatus.Union());
    builder.Finish(outboundPacketData);

    auto outboundPacket = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_UNSEQUENCED);

    enet_host_broadcast(server, 0, outboundPacket);
}

void Game::Net::Server::onPlayerRegisterMessage(const PlayerRegister *packet, ENetPeer &peer) {

    flatbuffers::FlatBufferBuilder builder{50};

    auto vectorOfVehicleData = builder.CreateVectorOfStructs(getDataOfVector<PlayerData>(vehicles));
    auto vectorOfBulletData = builder.CreateVectorOfStructs(getDataOfVector<BulletData>(bullets));
    auto vectorOfMissileData = builder.CreateVectorOfStructs(getDataOfVector<BulletData>(missiles));
    auto vectorOfWallData = builder.CreateVectorOfStructs(getDataOfVector<WallData>(walls));
    auto vectorOfCollectableData = builder.CreateVectorOfStructs(getDataOfVector<CollectableData>(collectables));

    std::vector<std::string> vehicleNames;
    for (Game::Vehicle &vehicle : vehicles) {
        vehicleNames.push_back(vehicle.name);
    }
    Vehicle newVehicle{*(std::uint8_t *)peer.data, Kore::vec2{500, 500}, 0, packet->name()->str()};

    auto playerData = newVehicle.getData();
    auto playerNameData = builder.CreateString(newVehicle.name);

    auto vehicleNamesData = builder.CreateVectorOfStrings(vehicleNames);

    auto playerJoinDownloadPacket = CreatePacket(
        builder,
        PacketType::PlayerJoinDownload,
        CreatePlayerJoinDownload(
            builder,
            &playerData,
            playerNameData,
            vectorOfVehicleData,
            vehicleNamesData,
            vectorOfBulletData,
            vectorOfMissileData,
            vectorOfWallData,
            vectorOfCollectableData
        )
            .Union()
    );
    builder.Finish(playerJoinDownloadPacket);
    auto netPacket = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(&peer, 0, netPacket);

    vehicles.push_back(std::move(newVehicle));

    enet_host_broadcast(server, 0, createPlayerJoinPacket(playerData, packet->name()->str()));
}

void Game::Net::Server::onPlayerPlaceWallMessage(const PlayerPlaceWall *packet) {
    walls.push_back(Wall(packet->wall()));
}

void Game::Net::Server::onPlayerShootMessage(const PlayerShoot *packet) {
    bullets.push_back(Bullet(packet->bullet()));
}

void Game::Net::Server::onPlayerShootMissileMessage(const PlayerShootMissile *packet) {
    missiles.push_back(Missile(packet->bullet()));
}

void Game::Net::Server::onPlayerMoveMessage(const PlayerMove *packet) {
    // update server side state for player location
    Vehicle *serverVehiclePtr = getVehicleById(packet->player());
    if (serverVehiclePtr == nullptr) {
        std::cerr << "Received move for nonexistant player" << std::endl;
        return;
    }
    Vehicle &vehicle = *serverVehiclePtr;

    vehicle.pos.x() = packet->pos()->x();
    vehicle.pos.y() = packet->pos()->y();
    vehicle.angle = packet->angle();
}

ENetPacket *Game::Net::Server::createPlayerJoinPacket(Game::Net::PlayerData vehicleData, std::string name) {
    flatbuffers::FlatBufferBuilder builder{50};
    auto nameData = builder.CreateString(name);
    auto packet = CreatePacket(
        builder,
        PacketType::PlayerJoin,
        CreatePlayerJoin(
            builder,
            &vehicleData,
            nameData
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
