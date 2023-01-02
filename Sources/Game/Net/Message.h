#ifndef GAME_NET_MESSAGE
#define GAME_NET_MESSAGE

#include <cstdint>

namespace Game {
    namespace Net {
        enum class MessageType : std::uint8_t {
            PLAYER_JOIN = 1,
            PLAYER_LEAVE = 2,
            PLAYER_MOVE = 3,
            PLAYER_JOIN_DOWNLOAD = 4,
        };

        enum class DisconnectReason : std::uint32_t {
            MESSAGE_BEFORE_JOIN = 1, // A message was received from a peer without a playerId set
        };
    } // namespace Net
} // namespace Game

#endif
