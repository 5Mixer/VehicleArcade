#include "Simulation.h"

// TODO: Deduplicate this!
void Game::interactBulletsAndWalls(std::vector<Game::Bullet> &bullets, std::vector<Game::Wall> &walls) {
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&](Game::Bullet bullet) -> bool {
                      // TODO: Avoid n^2
                      for (auto &wall : walls) {
                          if ((bullet.pos - wall.pos).squareLength() < std::pow(10, 2)) {
                              wall.health--;
                              return true;
                          }
                      }
                      return false;
                  }),
                  bullets.end());

    walls.erase(std::remove_if(walls.begin(), walls.end(), [&](Game::Wall wall) -> bool {
                    return wall.health <= 0;
                }),
                walls.end());
}
void Game::interactMissilesAndWalls(std::vector<Game::Missile> &missiles, std::vector<Game::Wall> &walls) {
    missiles.erase(std::remove_if(missiles.begin(), missiles.end(), [&](Game::Missile missile) -> bool {
                       // TODO: Avoid n^2
                       for (auto &wall : walls) {
                           if ((missile.pos - wall.pos).squareLength() < std::pow(50, 2)) {
                               wall.health -= 5;
                               return true;
                           }
                       }
                       return false;
                   }),
                   missiles.end());

    walls.erase(std::remove_if(walls.begin(), walls.end(), [&](Game::Wall wall) -> bool {
                    return wall.health <= 0;
                }),
                walls.end());
}
