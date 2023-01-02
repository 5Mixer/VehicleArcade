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
            std::uint8_t playerId = reader.readU8();
            float x = float(reader.readS32LE()) / 10;
            float y = float(reader.readS32LE()) / 10;
            float angle = float(reader.readU8()) / 255 * 3.14 * 2;

            onPlayerMoveMessage(playerId, x, y, angle);
            break;
        }
        default: {
            std::cerr << "Received unknown message type " << static_cast<unsigned int>(messageType);
            exit(1);
        }
    }
}