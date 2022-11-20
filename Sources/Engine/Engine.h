#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include "Input.h"
#include "Scene.h"
#include <Kore/Graphics2/Graphics.h>
#include <Kore/Graphics4/Graphics.h>
#include <Kore/System.h>
#include <iostream>
#include <memory>

namespace Engine {
    inline std::shared_ptr<Scene> scene;
    inline void init() {
        Input::init();
    }
    void update();
} // namespace Engine

#endif
