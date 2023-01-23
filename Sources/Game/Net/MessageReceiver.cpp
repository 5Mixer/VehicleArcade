#include "MessageReceiver.h"

void Game::Net::MessageReceiver::processRawPacket(const Packet *packet) {
    switch (packet->type_type()) {
        case PacketType::PlayerJoin: {
            onPlayerJoinMessage(packet->type_as_PlayerJoin());
            break;
        }
        case PacketType::PlayerJoinDownload: {
            onPlayerJoinDownloadMessage(packet->type_as_PlayerJoinDownload());
            break;
        }
        case PacketType::PlayerDisconnect: {
            onPlayerDisconnectMessage(packet->type_as_PlayerDisconnect());
            break;
        }
        case PacketType::PlayerMove: {
            onPlayerMoveMessage(packet->type_as_PlayerMove());
            break;
        }
        case PacketType::PlayerStatus: {
            onPlayerStatusMessage(packet->type_as_PlayerStatus());
            break;
        }
        case PacketType::PlayerShoot: {
            onPlayerShootMessage(packet->type_as_PlayerShoot());
            break;
        }
        case PacketType::PlayerShootMissile: {
            onPlayerShootMissileMessage(packet->type_as_PlayerShootMissile());
            break;
        }
        case PacketType::PlayerPlaceWall: {
            onPlayerPlaceWallMessage(packet->type_as_PlayerPlaceWall());
            break;
        }
        default: {
            std::cerr << "Received unknown message type " << int(packet->type_type()) << std::endl;
        }
    }
}