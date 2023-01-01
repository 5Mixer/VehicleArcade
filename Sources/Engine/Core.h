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
        std::unique_ptr<Graphics> graphics; // This must not be default initialised as K won't be initialised. Hence pointer.
        Scene *scene;

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
        void setScene(Scene *const scene);
    };
} // namespace Engine

#endif
