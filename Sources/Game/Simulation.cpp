#include "Simulation.h"

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
