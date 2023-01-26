#ifndef GAME_WALL
#define GAME_WALL

#include "Collider.h"
#include "Net/EntityGenerated.h"
#include <Kore/Math/Vector.h>
#include <cstdint>

namespace Game {
    constexpr int wallRadius = 50;
    class Wall : public CircleCollider {
    private:
    public:
        Wall(std::uint8_t placer, Kore::vec2 pos, int health = 8)
            : placer(placer), pos(pos), health(health){};
        Wall(const Net::WallData *data)
            : placer(data->placer()),
              pos(Kore::vec2{
                  data->pos().x(),
                  data->pos().y()}),
              health(data->health()){};

        Kore::vec2 getColliderPos() { return pos; };
        float getColliderRadius() { return wallRadius; };

        const Net::WallData getData();

        std::uint8_t placer;
        Kore::vec2 pos;
        int health = 8;
    };
} // namespace Game

#endif
