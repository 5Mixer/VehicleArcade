#ifndef ENGINE_H
#define ENGINE_H

#include <Kore/Graphics2/Graphics.h>
#include <Kore/Graphics4/Graphics.h>
#include <Kore/System.h>

#include "Input.h"

namespace Engine {
    inline void init() {
        Input::init();
    }
    void update();
} // namespace Engine

#endif