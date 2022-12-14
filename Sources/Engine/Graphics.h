#ifndef ENGINE_GRAPHICS
#define ENGINE_GRAPHICS

#include <Kore/Graphics2/Graphics.h>
#include <Kore/System.h>
#include <memory>

namespace Engine {
    class Graphics {
    private:
        void start();
        std::unique_ptr<Kore::Graphics2::Graphics2> graphics;

    public:
        Graphics();
        void begin();
        void end();
        void drawSprite(int x, int y);
    };
} // namespace Engine

#endif