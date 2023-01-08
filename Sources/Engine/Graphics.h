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
        Kore::Graphics4::Texture texture;
        Kore::Graphics4::Texture carTexture;
        Kore::Graphics4::Texture grassTexture;
        Kore::Graphics4::Texture bulletTexture;
        Kore::Graphics4::Texture wall1Texture;
        Kore::Graphics4::Texture wall2Texture;
        Kore::Graphics4::Texture wall3Texture;

    public:
        Graphics();
        void begin();
        void end();
        void drawTexture();
        void drawSprite(int sprite, Kore::vec2 pos, float angle = 0);
        void drawVehicle(Kore::vec2 pos, float angle = 0);
        void drawBullet(Kore::vec2 pos, float angle = 0);
        void drawWall(Kore::vec2 pos, int layer);
        void transform(Kore::mat3 transformation);
        Kore::mat3 rotate(float angle, float centerx, float centery);
    }; // namespace Engine
} // namespace Engine
#endif