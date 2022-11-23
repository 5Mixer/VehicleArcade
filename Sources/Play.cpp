#include "Play.h"

Play::Play() {
}

void Play::render(Kore::Graphics2::Graphics2 &g) {
    g.fillRect(50, 50, 100, 100);
}

void Play::update() {
}

std::string Play::toString() {
    return "Play";
}
