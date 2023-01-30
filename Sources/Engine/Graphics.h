#ifndef ENGINE_GRAPHICS
#define ENGINE_GRAPHICS

#include "SpriteAtlas.h"
#include "Transform.h"
#include <Kore/Graphics2/Graphics.h>
#include <Kore/Graphics4/Graphics.h>
#include <Kore/Math/Matrix.h>
#include <Kore/Math/Vector.h>
#include <Kore/System.h>
#include <memory>
#include <string>
#include <vector>

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
        void begin() {
            graphics->begin(false, Kore::System::windowWidth(), Kore::System::windowHeight());
            graphics->clear(0x217844);
        }
        void end() {
            graphics->end();
        }
        template <class T>
        void renderVector(std::vector<T> &elements) {
            for (T &element : elements) {
                element.render(*this);
            }
        }
        void drawSprite(Engine::Sprite sprite, Kore::vec2 pos) {
            graphics->drawScaledSubImage(&spriteAtlasTexture, sprite.x, sprite.y, sprite.width, sprite.height, pos.x() - sprite.width / 2, pos.y() - sprite.height / 2, sprite.width, sprite.height);
        }
        void drawScaledSprite(Engine::Sprite sprite, Kore::vec2 pos, float scale) {
            graphics->drawScaledSubImage(&spriteAtlasTexture, sprite.x, sprite.y, sprite.width, sprite.height, pos.x() - sprite.width * scale / 2, pos.y() - sprite.height * scale / 2, sprite.width * scale, sprite.height * scale);
        }
        void drawRotatedSprite(Engine::Sprite sprite, Kore::vec2 pos, float angle);
        void drawTrail(Kore::vec2 pos) {
            drawSprite(*spriteAtlas.get("Assets/trail.png"), pos);
        }
        void drawCursor(Kore::vec2 pos) {
            drawSprite(*spriteAtlas.get("Assets/cursor.png"), pos);
        }
        void drawBar(Kore::vec2 pos, int filledBarElements, int totalBarElements);
        void drawGrass(Kore::vec2 pos, float angle, std::uint8_t variety);
        void drawVehicle(Kore::vec2 pos, float angle = 0, float angleDelta = 0);
        void drawParticle(Kore::vec2 pos, std::uint32_t colour) {
            graphics->setColor(colour);
            drawSprite(*spriteAtlas.get("Assets/particle.png"), pos);
            graphics->setColor(Kore::Graphics1::Color::White);
        }
        void drawBullet(Kore::vec2 pos) {
            drawSprite(*spriteAtlas.get("Assets/bullet.png"), pos);
        }
        void drawCoin(Kore::vec2 pos) {
            drawSprite(*spriteAtlas.get("Assets/coin.png"), pos);
        }
        void drawMissile(Kore::vec2 pos, float angle = 0) {
            drawRotatedSprite(*spriteAtlas.get("Assets/missile.png"), pos, angle);
        }
        void drawWall(Kore::vec2 pos, int layer) {
            drawSprite(*spriteAtlas.get("Assets/wall" + std::to_string(layer + 1) + ".png"), pos);
        }
        void transform(Kore::mat3 transformation) {
            graphics->transformation *= transformation;
        }
        void drawMoney(Kore::vec2 pos, int money) {
            FontStyle style;
            style.bold = false;
            style.italic = false;
            style.underlined = false;
            graphics->setFontColor(Kore::Graphics1::Color::White);
            graphics->setFont(Kore::Kravur::load("fonts/font", style, 48));
            graphics->drawString(std::to_string(money).c_str(), pos.x(), pos.y());
        }
    }; // namespace Engine
} // namespace Engine
#endif