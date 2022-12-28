#ifndef GAME_PLAY_H
#define GAME_PLAY_H

#include "../Engine/Graphics.h"
#include "../Engine/Scene.h"
#include "Bullet.h"
#include "Entity.h"
#include "Tilemap.h"
#include "Vehicle.h"
#include "Wall.h"
#include <Kore/IO/FileReader.h>
#include <Kore/IO/FileWriter.h>
#include <Kore/Math/Matrix.h>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace Game {
    class Play : public Engine::Scene {
    private:
        std::vector<Game::Entity> entities;
        std::vector<Game::Bullet> bullets;
        std::vector<Game::Wall> walls;
        std::shared_ptr<Game::Vehicle> controlledCar;
        std::unique_ptr<Game::Tilemap> tilemap;
        bool editingScene;
        void save();

    public:
        Play();
        void render(Engine::Graphics &g);
        void update();
        std::string toString();
    };
} // namespace Game

#endif