#ifndef GAME_SIMULATION
#define GAME_SIMULATION

#include "Bullet.h"
#include "Missile.h"
#include "Vehicle.h"
#include "Wall.h"
#include <algorithm>
#include <iostream>
#include <vector>

namespace Game {
    void interactBulletsAndWalls(std::vector<Game::Bullet> &bullets, std::vector<Game::Wall> &walls);
    void interactMissilesAndWalls(std::vector<Game::Missile> &missiles, std::vector<Game::Wall> &walls);
    void interactMissilesAndVehicles(std::vector<Game::Missile> &missiles, std::vector<Game::Vehicle> &vehicles);
} // namespace Game

#endif