#include "Engine.h"

namespace Engine {
    void update() {
        Kore::Graphics4::begin();
        Kore::Graphics4::clear(Kore::Graphics4::ClearColorFlag);

        auto g = Kore::Graphics2::Graphics2(Kore::System::windowWidth(), Kore::System::windowHeight());
        g.begin();

        g.fillRect(50, 50, 100, 100);

        g.end();

        Kore::Graphics4::end();
        Kore::Graphics4::swapBuffers();
    }
} // namespace Engine
