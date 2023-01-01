#ifndef GAME_NET_MESSAGE_RECEIVER
#define GAME_NET_MESSAGE_RECEIVER

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
            virtual void onPlayerMoveMessage(uint8_t playerId, int x, int y, float angle) = 0;
        };

    } // namespace Net
} // namespace Game

#endif
