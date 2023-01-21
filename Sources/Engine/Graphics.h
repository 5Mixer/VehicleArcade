#ifndef ENGINE_GRAPHICS
#define ENGINE_GRAPHICS

#include "SpriteAtlas.h"
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
        Engine::SpriteAtlas spriteAtlas;
        Kore::Graphics4::Texture spriteAtlasTexture;
        Kore::Graphics4::Texture grassTexture;

    public:
        Graphics(Engine::SpriteAtlas &spriteAtlas);
        void begin();
        void end();
        void drawSprite(Engine::Sprite sprite, Kore::vec2 pos);
        void drawRotatedSprite(Engine::Sprite sprite, Kore::vec2 pos, float angle);
        void drawTrail(Kore::vec2 pos);
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