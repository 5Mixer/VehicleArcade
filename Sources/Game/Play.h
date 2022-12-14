#ifndef GAME_PLAY_H
#define GAME_PLAY_H

#include "../Engine/Graphics.h"
#include "../Engine/Scene.h"
#include "Entity.h"
#include "Tilemap.h"
#include "Vehicle.h"
#include <memory>
#include <string>
#include <vector>

namespace Game {
    class Play : public Engine::Scene {
    private:
        std::vector<Game::Entity> entities;
        std::shared_ptr<Game::Vehicle> controlledCar;
        std::unique_ptr<Game::Tilemap> tilemap;

    public:
        Play();
        void render(std::shared_ptr<Engine::Graphics> g);
        void update();
        std::string toString();
    };
} // namespace Game

#endif