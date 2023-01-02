#include "MessageReceiver.h"

void Game::Net::MessageReceiver::processRawPacket(const void *buffer, int size) {
    Kore::BufferReader reader(buffer, size);

    std::uint8_t messageType = reader.readU8();

    switch (static_cast<MessageType>(messageType)) {
        case MessageType::PLAYER_JOIN: {
            onPlayerJoinMessage(reader.readU8());
            break;
        }
        case MessageType::PLAYER_JOIN_DOWNLOAD: {
            onPlayerJoinDownloadMessage(reader.readU8());
            break;
        }
        case MessageType::PLAYER_MOVE: {
            PacketPlayerMove packet{reader};
            onPlayerMoveMessage(packet.playerId, packet.x, packet.y, packet.angle);
            break;
        }
        default: {
            std::cerr << "Received unknown message type " << static_cast<unsigned int>(messageType);
            exit(1);
        }
    }
}