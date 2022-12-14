#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#include "Graphics.h"
#include "Input.h"
#include "Scene.h"
#include <Kore/System.h>
#include <iostream>
#include <memory>
#include <random>

namespace Engine {
    class Core {
    private:
        std::shared_ptr<Graphics> graphics;
        std::shared_ptr<Scene> scene;

    public:
        static Core &getInstance() {
            static Core instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }

        Core(Core const &) = delete;
        void operator=(Core const &) = delete;
        Core();

        std::minstd_rand random;
        float rand() { return random() / static_cast<float>(random.max()); }

        void init();
        void start();
        void update();
        void setScene(std::shared_ptr<Scene> scene);
    };
} // namespace Engine

#endif
