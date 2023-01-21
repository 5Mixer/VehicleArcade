#ifndef GAME_TRAIL
#define GAME_TRAIL

#include "Net/EntityGenerated.h"
#include <Kore/Math/Vector.h>
#include <cstdint>

namespace Game {
    class Trail {
    private:
    public:
        Trail(Kore::vec2 pos)
            : pos(pos){};
        Trail(const Net::WallData *data)
            : pos(Kore::vec2{
                  data->pos().x(),
                  data->pos().y()}) {}

        const Net::WallData getData();

        Kore::vec2 pos;
    };
} // namespace Game

#endif
