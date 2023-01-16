#ifndef GAME_NET_MESSAGE_RECEIVER
#define GAME_NET_MESSAGE_RECEIVER

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

            virtual void onPlayerJoinMessage(const PlayerJoin *packet) = 0;
            virtual void onPlayerJoinDownloadMessage(const PlayerJoinDownload *packet) = 0; // server -> new client
            virtual void onPlayerDisconnectMessage(const PlayerDisconnect *packet) = 0;
            virtual void onPlayerMoveMessage(const PlayerMove *packet) = 0;
            virtual void onPlayerShootMessage(const PlayerShoot *packet) = 0;
            virtual void onPlayerShootMissileMessage(const PlayerShootMissile *packet) = 0;
            virtual void onPlayerPlaceWallMessage(const PlayerPlaceWall *packet) = 0;
            virtual void onDisconnect() = 0;
            void onTimeout() { onDisconnect(); };

            void processRawPacket(const Packet *packet);
        };

    } // namespace Net
} // namespace Game

#endif
