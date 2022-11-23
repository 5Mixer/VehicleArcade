#ifndef PLAY_H
#define PLAY_H

#include "Engine/Scene.h"
#include <Kore/Graphics2/Graphics.h>
#include <memory>
#include <string>

class Play : public Engine::Scene {
public:
    Play();
    void render(Kore::Graphics2::Graphics2 &g);
    void update();
    std::string toString();
};

#endif