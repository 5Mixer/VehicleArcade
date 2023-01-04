#ifndef GAME_NET_MESSAGE
#define GAME_NET_MESSAGE

#include <cstdint>

namespace Game {
    namespace Net {
        enum class DisconnectReason : std::uint32_t {
            MESSAGE_BEFORE_JOIN = 1, // A message was received from a peer without a playerId set
        };
    } // namespace Net
} // namespace Game

#endif
