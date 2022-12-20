#include "Input.h"

namespace Engine {
    namespace Input {
        void onKeyDown(Kore::KeyCode code) {
            keysDown[code] = true;
        }
        void onKeyUp(Kore::KeyCode code) {
            keysDown[code] = false;
        }

        void onMousePress(int windowId, int button, int x, int y) {
            mousePosition.x() = x;
            mousePosition.y() = y;

            if (button == 0) {
                mouseDown = true;
            }
        }
        void onMouseRelease(int windowId, int button, int x, int y) {
            mousePosition.x() = x;
            mousePosition.y() = y;

            if (button == 0) {
                mouseDown = false;
            }
        }

        void onMouseMove(int windowId, int x, int y, int movementX, int movementY) {
            mousePosition.x() = x;
            mousePosition.y() = y;
        }

        void init() {
            auto keyboard = Kore::Keyboard::the();
            keyboard->KeyDown = onKeyDown;
            keyboard->KeyUp = onKeyUp;

            auto mouse = Kore::Mouse::the();
            mouse->Press = onMousePress;
            mouse->Move = onMouseMove;
            mouse->Release = onMouseRelease;
        }
    } // namespace Input
} // namespace Engine
