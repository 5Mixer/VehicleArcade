#ifndef GAME_NET_MESSAGE_RECEIVER
#define GAME_NET_MESSAGE_RECEIVER

#include "Message.h"
#include "Packet.h"
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

            virtual void onPlayerJoinMessage(uint8_t playerId) = 0;
            virtual void onPlayerJoinDownloadMessage(uint8_t playerId) = 0; // server -> new client
            virtual void onPlayerMoveMessage(PacketPlayerMove &packet) = 0;
            virtual void onDisconnect() = 0;
            void onTimeout() { onDisconnect(); };

            void processRawPacket(const void *buffer, int size);
        };

    } // namespace Net
} // namespace Game

#endif
