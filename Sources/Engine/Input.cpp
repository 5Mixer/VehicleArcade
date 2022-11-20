#include "Input.h"

namespace Engine {
    namespace Input {
        void onKeyDown(Kore::KeyCode code) {
            keysDown[code] = true;
        }
        void onKeyUp(Kore::KeyCode code) {
            keysDown[code] = false;
        }

        void init() {
            auto keyboard = Kore::Keyboard::the();
            keyboard->KeyDown = onKeyDown;
            keyboard->KeyUp = onKeyUp;
        }
    } // namespace Input
} // namespace Engine
