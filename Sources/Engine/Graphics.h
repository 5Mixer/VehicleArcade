#ifndef ENGINE_GRAPHICS
#define ENGINE_GRAPHICS

#include "../simdjson.h"
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
        Kore::Graphics4::Texture missileTexture;
        Kore::Graphics4::Texture wall1Texture;
        Kore::Graphics4::Texture wall2Texture;
        Kore::Graphics4::Texture wall3Texture;
        Kore::Graphics4::Texture wheelTexture;

    public:
        Graphics();
        void begin();
        void end();
        void drawSprite(int sprite, Kore::vec2 pos, float angle = 0);
        void drawGrass(Kore::vec2 pos, float angle, std::uint8_t variety);
        void drawVehicle(Kore::vec2 pos, float angle = 0, float angleDelta = 0);
        void drawBullet(Kore::vec2 pos, float angle = 0);
        void drawMissile(Kore::vec2 pos, float angle = 0);
        void drawWall(Kore::vec2 pos, int layer);
        void transform(Kore::mat3 transformation);
        Kore::mat3 rotate(float angle, float centerx, float centery);
    }; // namespace Engine
} // namespace Engine
#endif