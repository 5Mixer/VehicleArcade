#ifndef GAME_NET_PACKET
#define GAME_NET_PACKET

#include "Message.h"
#include <Kore/IO/BufferReader.h>
#include <Kore/IO/Writer.h>
#include <cmath>
#include <cstdint>
#include <enet/enet.h>
#include <vector>

namespace Game {
    namespace Net {
        class Packet : public Kore::Writer {
        private:
            std::vector<std::uint8_t> bytes;

        public:
            Packet(MessageType messageType) : bytes({static_cast<std::uint8_t>(messageType)}){};

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

        class PacketPlayerMove {
        public:
            // Note: Order sensitive due to buffer initialiser constructor
            const std::uint8_t playerId;
            const float x;
            const float y;
            const float angle;

            PacketPlayerMove(
                std::uint8_t playerId,
                float x,
                float y,
                float angle)
                : playerId(playerId),
                  x(x),
                  y(y),
                  angle(angle){};

            PacketPlayerMove(Kore::BufferReader buffer)
                : playerId(buffer.readU8()),
                  x(float(buffer.readS32LE()) / 10),
                  y(float(buffer.readS32LE()) / 10),
                  angle(float(buffer.readU8()) / 255 * 3.14 * 2){};

            ENetPacket *generate(enet_uint32 flags) {
                Packet packet{MessageType::PLAYER_MOVE};
                packet.writeU8(playerId);
                packet.writeS32LE(x * 10);
                packet.writeS32LE(y * 10);
                packet.writeU8(static_cast<std::uint8_t>(std::fmod(angle, 3.14 * 2) / (2 * 3.14) * 255));

                return packet.generate(flags);
            }
        };
    } // namespace Net

} // namespace Game

#endif