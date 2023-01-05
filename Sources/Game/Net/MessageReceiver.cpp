#include "MessageReceiver.h"

void Game::Net::MessageReceiver::processRawPacket(Packet &packet) {
    switch (packet.type_type()) {
        case PacketType::PlayerJoin: {
            onPlayerJoinMessage(packet.type_as_PlayerJoin()->player());
            break;
        }
        case PacketType::PlayerJoinDownload: {

            onPlayerJoinDownloadMessage(packet.type_as_PlayerJoinDownload()->player());
            break;
        }
        case PacketType::PlayerMove: {
            std::cout << "Player with id " << static_cast<unsigned int>(packet.type_as_PlayerMove()->player()) << " moved. x: " << packet.type_as_PlayerMove()->pos()->x() << ", y: " << packet.type_as_PlayerMove()->pos()->y() << ", Θ: " << packet.type_as_PlayerMove()->angle() << std::endl;
            onPlayerMoveMessage(packet.type_as_PlayerMove());
            break;
        }
        case PacketType::PlayerShoot: {
            break;
        }
        default: {
            std::cerr << "Received unknown message type" << int(packet.type_type()) << std::endl;
        }
    }
}