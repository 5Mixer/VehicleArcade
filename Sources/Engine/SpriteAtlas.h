#ifndef ENGINE_SPRITEATLAS
#define ENGINE_SPRITEATLAS

#include "../simdjson.h"
#include <Kore/Graphics2/Graphics.h>
#include <Kore/System.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace Engine {
    struct Sprite {
        std::string id;
        int x;
        int y;
        int width;
        int height;
        Sprite(std::string id, int x, int y, int width, int height)
            : id(id), x(x), y(y), width(width), height(height) {}
    };

    class SpriteAtlas {
    private:
        std::vector<Engine::Sprite> sprites;

    public:
        SpriteAtlas(std::string_view spritesJsonPath);
        const Engine::Sprite *get(std::string_view id);
    };

} // namespace Engine
#endif
