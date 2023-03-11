#ifndef ENGINE_RECTANGLE
#define ENGINE_RECTANGLE

#include <Kore/Math/Vector.h>
#include <algorithm>
#include <cstdint>

namespace Engine {
    inline Kore::vec2 constrainPointToRect(Kore::vec2 point, Kore::vec2 topLeft, Kore::vec2 bottomRight) {
        return Kore::vec2{
            std::max(topLeft.x(), std::min(point.x(), bottomRight.x())),
            std::max(topLeft.y(), std::min(point.y(), bottomRight.y())),
        };
    }
    inline bool isPointInRect(Kore::vec2 point, Kore::vec2 topLeft, Kore::vec2 bottomRight) {
        return topLeft.x() < point.x() && point.x() < bottomRight.x() &&
               topLeft.y() < point.y() && point.y() < bottomRight.y();
    }

} // namespace Engine

#endif