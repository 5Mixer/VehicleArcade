#ifndef GAME_NET_MESSAGE
#define GAME_NET_MESSAGE

#include <cstdint>

namespace Game {
    namespace Net {
        enum class MessageType : unsigned char {
            PLAYER_JOIN = 1,
            PLAYER_LEAVE = 2,
            PLAYER_MOVE = 3
        };

        /*class Message {
        public:
            const MessageType type;
            Message(MessageType type) : type(type){};
        };

        class MessagePlayerJoin : public Message {
        public:
            const unsigned char playerId;
        };*/

    } // namespace Net
} // namespace Game

#endif
