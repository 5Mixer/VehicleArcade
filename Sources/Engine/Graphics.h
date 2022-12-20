#ifndef ENGINE_GRAPHICS
#define ENGINE_GRAPHICS

#include <Kore/Graphics2/Graphics.h>
#include <Kore/Graphics4/Graphics.h>
#include <Kore/Math/Matrix.h>
#include <Kore/Math/Vector.h>
#include <Kore/System.h>
#include <memory>
#include <string>

namespace Engine {
    class Graphics {
    private:
        void start();
        std::unique_ptr<Kore::Graphics2::Graphics2> graphics;
        std::unique_ptr<Kore::Graphics4::Texture> texture;
        std::unique_ptr<Kore::Graphics4::Texture> grassTexture;

    public:
        Graphics();
        void begin();
        void end();
        void drawTexture();
        void drawSprite(int sprite, Kore::vec2 pos, float angle = 0);
        void transform(Kore::mat3 transformation);
        Kore::mat3 rotate(float angle, float centerx, float centery);
    }; // namespace Engine
} // namespace Engine
#endif