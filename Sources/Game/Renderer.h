#ifndef GAME_RENDERER
#define GAME_RENDERER

#include "../Engine/Graphics.h"
#include "Vehicle.h"

namespace Game {
    class Renderer {
    private:
        Graphics graphics;

    public:
        void renderVehicle(Vehicle &vehicle);
    };
} // namespace Game

#endif
