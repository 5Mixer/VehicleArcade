#ifndef GAME_SIMULATION
#define GAME_SIMULATION

#include "Bullet.h"
#include "Wall.h"
#include <algorithm>
#include <vector>

namespace Game {
    void interactBulletsAndWalls(std::vector<Game::Bullet> &bullets, std::vector<Game::Wall> &walls);
}

#endif