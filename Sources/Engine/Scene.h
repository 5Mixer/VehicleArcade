#ifndef SCENE_H
#define SCENE_H

#include <Kore/Graphics2/Graphics.h>

namespace Engine {
    class Scene {
        Scene();

    public:
        void render(Kore::Graphics2::Graphics2 g);
        void update();
    };
} // namespace Engine

#endif