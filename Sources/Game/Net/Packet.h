#ifndef GAME_NET_PACKET
#define GAME_NET_PACKET

#include "Message.h"
#include <Kore/IO/Writer.h>
#include <cstdint>
#include <enet/enet.h>
#include <vector>

namespace Game {
    namespace Net {
        class Packet : public Kore::Writer {
        private:
            std::vector<std::uint8_t> bytes;

        public:
            void write(void *data, int size) {
                for (int i = 0; i < size; i++) {
                    bytes.push_back(*(reinterpret_cast<std::uint8_t *>(data) + i));
                }
            }

            void write(MessageType messageType) {
                writeU8(static_cast<std::uint8_t>(messageType));
            }

            ENetPacket *generate(enet_uint32 flags) {
                return enet_packet_create(bytes.data(), bytes.size(), flags);
            }
        };
    } // namespace Net
} // namespace Game

#endif