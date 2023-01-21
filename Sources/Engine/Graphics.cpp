#include "Graphics.h"

namespace Engine {
    Engine::Graphics::Graphics(Engine::SpriteAtlas &spriteAtlas)
        : graphics(std::unique_ptr<Kore::Graphics2::Graphics2>(new Kore::Graphics2::Graphics2(Kore::System::windowWidth(), Kore::System::windowHeight()))),
          spriteAtlasTexture(Kore::Graphics4::Texture("./sprites.png")),
          spriteAtlas(spriteAtlas),
          wall1Texture(Kore::Graphics4::Texture("../Assets/wall1.png")),
          wall2Texture(Kore::Graphics4::Texture("../Assets/wall2.png")),
          wall3Texture(Kore::Graphics4::Texture("../Assets/wall3.png")),
          grassTexture(Kore::Graphics4::Texture("../Assets/grass.png")) {
    }

    void Engine::Graphics::start() {
    }

    void Engine::Graphics::begin() {
        graphics->begin(false, Kore::System::windowWidth(), Kore::System::windowHeight());
        graphics->clear(0x217844);
    }
    void Engine::Graphics::end() {
        graphics->end();
    }
    void Engine::Graphics::drawSprite(Engine::Sprite sprite, Kore::vec2 pos) {
        graphics->drawScaledSubImage(&spriteAtlasTexture, sprite.x, sprite.y, sprite.width, sprite.height, pos.x() - sprite.width / 2, pos.y() - sprite.height / 2, sprite.width, sprite.height);
    }
    void Engine::Graphics::drawRotatedSprite(Engine::Sprite sprite, Kore::vec2 pos, float angle) {
        auto prior = graphics->transformation;
        transform(rotate(angle, pos.x(), pos.y()));
        graphics->drawScaledSubImage(&spriteAtlasTexture, sprite.x, sprite.y, sprite.width, sprite.height, pos.x() - sprite.width / 2, pos.y() - sprite.height / 2, sprite.width, sprite.height);
        graphics->transformation = prior;
    }
    void Engine::Graphics::drawGrass(Kore::vec2 pos, float angle, std::uint8_t variety) {
        const Kore::vec2 screenPos = graphics->transformation * Kore::vec3{pos.x(), pos.y(), 1};
        const int buffer = 20;
        if (screenPos.x() < -buffer || screenPos.y() < -buffer || screenPos.x() > Kore::System::windowWidth() + buffer || screenPos.y() > Kore::System::windowHeight() + buffer) {
            return;
        }
        graphics->drawScaledSubImage(&grassTexture, variety * 20, 0, 20, 20, pos.x() - 20 / 2, pos.y() - 20 / 2, 20, 20);
    }
    void Engine::Graphics::drawVehicle(Kore::vec2 pos, float angle, float angleDelta) {
        drawRotatedSprite(*spriteAtlas.get("Assets/car.png"), pos, angle);

        auto rotation = rotate(angle, pos.x(), pos.y());
        auto leftWheelPos = rotation * Kore::vec3(pos.x() + 40, pos.y() - 35, 1);
        auto rightWheelPos = rotation * Kore::vec3(pos.x() + 40, pos.y() + 35, 1);

        drawRotatedSprite(*spriteAtlas.get("Assets/carWheel.png"), leftWheelPos, angle + angleDelta);
        drawRotatedSprite(*spriteAtlas.get("Assets/carWheel.png"), rightWheelPos, angle + angleDelta);
    }
    void Engine::Graphics::drawBullet(Kore::vec2 pos, float angle) {
        drawSprite(*spriteAtlas.get("Assets/bullet.png"), pos);
    }
    void Engine::Graphics::drawMissile(Kore::vec2 pos, float angle) {
        drawRotatedSprite(*spriteAtlas.get("Assets/missile.png"), pos, angle);
    }
    void Engine::Graphics::drawWall(Kore::vec2 pos, int layer) {
        drawSprite(*spriteAtlas.get("Assets/wall" + std::to_string(layer + 1) + ".png"), pos);
    }
    void Engine::Graphics::transform(Kore::mat3 transformation) {
        graphics->transformation *= transformation;
    }
    Kore::mat3 Engine::Graphics::rotate(float angle, float centerx, float centery) {
        return (Kore::mat3::Translation(centerx, centery) * Kore::mat3::RotationZ(angle)) * Kore::mat3::Translation(-centerx, -centery);
    }
} // namespace Engine