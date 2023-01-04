#ifndef GAME_NET_MESSAGE_RECEIVER
#define GAME_NET_MESSAGE_RECEIVER

#include "Message.h"
#include "PacketGenerated.h"
#include <Kore/IO/BufferReader.h>
#include <cstdint>
#include <iostream>

/**
 * MessageReceiver is an abstract class that allows concrete classes to directly
 * handle every type of deserialised network message
 */
namespace Game {
    namespace Net {
        class MessageReceiver {
        public:
            MessageReceiver() {}
            virtual ~MessageReceiver() {}

            virtual void onPlayerJoinMessage(std::uint8_t playerId) = 0;
            virtual void onPlayerJoinDownloadMessage(std::uint8_t playerId) = 0; // server -> new client
            virtual void onPlayerMoveMessage(const PlayerMove *packet) = 0;
            virtual void onDisconnect() = 0;
            void onTimeout() { onDisconnect(); };

            void processRawPacket(Packet &packet);
        };

    } // namespace Net
} // namespace Game

#endif
