#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include "Graphics.h"
#include <memory>
#include <string>

namespace Engine {
    class Scene {

    public:
        virtual void render(std::shared_ptr<Engine::Graphics> g) = 0;
        virtual void update() = 0;
        virtual std::string toString() = 0;
    };
} // namespace Engine

#endif
