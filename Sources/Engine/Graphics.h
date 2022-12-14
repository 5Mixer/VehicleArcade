#ifndef ENGINE_GRAPHICS
#define ENGINE_GRAPHICS

#include <Kore/Graphics2/Graphics.h>
#include <Kore/Graphics4/Graphics.h>
#include <Kore/System.h>
#include <memory>
#include <string>

namespace Engine {
    class Graphics {
    private:
        void start();
        std::unique_ptr<Kore::Graphics2::Graphics2> graphics;
        std::unique_ptr<Kore::Graphics4::Texture> texture;

    public:
        Graphics();
        void begin();
        void end();
        void drawSprite(int sprite, Kore::vec2 pos, float angle = 0);
    };
} // namespace Engine

#endif