#include "SpriteAtlas.h"

Engine::SpriteAtlas::SpriteAtlas(std::string_view spritesJsonPath) {
    simdjson::ondemand::parser parser;
    simdjson::padded_string json = simdjson::padded_string::load(spritesJsonPath);
    simdjson::ondemand::document spriteDefinition = parser.iterate(json);
    auto spriteDefinitions = spriteDefinition.get_object();

    sprites.reserve(spriteDefinitions.count_fields());
    for (auto field : spriteDefinition.get_object()) {
        auto value = field.value();
        sprites.push_back(Sprite{
            std::string(field.unescaped_key().take_value()),
            static_cast<int>(value["x"].get_int64().value()),
            static_cast<int>(value["y"].get_int64().value()),
            static_cast<int>(value["width"].get_int64().value()),
            static_cast<int>(value["height"].get_int64().value())});
    }
}

const Engine::Sprite *Engine::SpriteAtlas::get(std::string_view id) {
    for (const auto &sprite : sprites) {
        if (sprite.id == id) {
            return &sprite;
        }
    }
    return nullptr;
}