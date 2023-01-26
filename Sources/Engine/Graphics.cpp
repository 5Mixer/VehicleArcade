#include "Graphics.h"

namespace Engine {
    Engine::Graphics::Graphics(Engine::SpriteAtlas &spriteAtlas)
        : graphics(std::unique_ptr<Kore::Graphics2::Graphics2>(new Kore::Graphics2::Graphics2(Kore::System::windowWidth(), Kore::System::windowHeight()))),
          spriteAtlasTexture(Kore::Graphics4::Texture("./sprites.png")),
          spriteAtlas(spriteAtlas),
          grassTexture(Kore::Graphics4::Texture("../Assets/grass.png")) {
    }
    void Engine::Graphics::drawRotatedSprite(Engine::Sprite sprite, Kore::vec2 pos, float angle) {
        auto prior = graphics->transformation;
        transform(Engine::Transform::rotate(angle, pos.x(), pos.y()));
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

        auto rotation = Engine::Transform::rotate(angle, pos.x(), pos.y());
        auto leftWheelPos = rotation * Kore::vec3(pos.x() + 40, pos.y() - 35, 1);
        auto rightWheelPos = rotation * Kore::vec3(pos.x() + 40, pos.y() + 35, 1);

        drawRotatedSprite(*spriteAtlas.get("Assets/carWheel.png"), leftWheelPos, angle + angleDelta);
        drawRotatedSprite(*spriteAtlas.get("Assets/carWheel.png"), rightWheelPos, angle + angleDelta);
    }
    void Engine::Graphics::drawBar(Kore::vec2 pos, int filledBarElements, int totalBarElements) {
        graphics->setColor(0xf2f85b5b);
        auto barElementSprite = *spriteAtlas.get("Assets/barElement.png");
        for (int offset = 0; offset < filledBarElements; offset++) {
            drawSprite(barElementSprite, pos + Kore::vec2{(barElementSprite.width) * offset, 0});
        }
        graphics->setColor(0x80989898);
        for (int offset = filledBarElements; offset < totalBarElements; offset++) {
            drawSprite(barElementSprite, pos + Kore::vec2{(barElementSprite.width) * offset, 0});
        }
        graphics->setColor(Kore::Graphics1::Color::White);
    }
} // namespace Engine