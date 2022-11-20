#include <Kore/Graphics4/Graphics.h>
#include <Kore/Graphics2/Graphics.h>
#include <Kore/IO/FileReader.h>
#include <Kore/System.h>

#include <limits>
#include <stdlib.h>

#include "Sprite.h"

#include "math.h"
#include "Engine.h"
#include "Input.h"

using namespace Kore;
// using namespace Engine;

namespace {
	void update() {
		Graphics4::begin();
		Graphics4::clear(Kore::Graphics4::ClearColorFlag);

		auto g = Graphics2::Graphics2(System::windowWidth(), System::windowHeight());
		g.begin();

		g.fillRect(50, 50, 100, 100);

		g.end();

		Graphics4::end();
		Graphics4::swapBuffers();
	}
}


int kickstart(int argc, char **argv) {
	Kore::System::init("Vehicle Arcade", 1024, 768);
	Kore::System::setCallback(update);

	Engine::init();
	Kore::System::start();

	return 0;
}
