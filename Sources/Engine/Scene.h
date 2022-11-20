#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include <Kore/Graphics2/Graphics.h>
#include <memory>
#include <string>

namespace Engine {
    class Scene {

    public:
        virtual void render(std::shared_ptr<Kore::Graphics2::Graphics2> g) = 0;
        virtual void update() = 0;
        virtual std::string toString() = 0;
    };
} // namespace Engine

#endif
