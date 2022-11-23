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
    class Engine {
    private:
        void start();
        Kore::Graphics2::Graphics2 g;

    public:
        Engine();
        std::shared_ptr<Scene> scene;

        void init() {
            Input::init();
            start();
        }

        void update();
    };
} // namespace Engine

#endif
