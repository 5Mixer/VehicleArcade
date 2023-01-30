#include "Simulation.h"

void Game::interactBulletsAndWalls(std::vector<Game::Bullet> &bullets, std::vector<Game::Wall> &walls) {
    Game::intersect(bullets, walls, [](Game::Bullet &bullet, Game::Wall &wall) -> void {
        wall.health--;
        bullet.health = 0;
    });
}
void Game::interactMissilesAndWalls(std::vector<Game::Missile> &missiles, std::vector<Game::Wall> &walls) {
    Game::intersect(missiles, walls, [](Game::Missile &missile, Game::Wall &wall) -> void {
        wall.health -= 5;
        missile.health = 0;
    });
}
void Game::interactBulletsAndVehicles(std::vector<Game::Bullet> &bullets, std::vector<Game::Vehicle> &vehicles) {
    Game::intersect(bullets, vehicles, [](Game::Bullet &bullet, Game::Vehicle &vehicle) -> void {
        if (vehicle.id == bullet.shooter) {
            return;
        }
        vehicle.changeHealth(-1);
        bullet.health = 0;
    });
}
void Game::interactMissilesAndVehicles(std::vector<Game::Missile> &missiles, std::vector<Game::Vehicle> &vehicles) {
    Game::intersect(missiles, vehicles, [](Game::Missile &missile, Game::Vehicle &vehicle) -> void {
        if (vehicle.id == missile.shooter) {
            return;
        }
        vehicle.changeHealth(-5);
        missile.health = 0;
    });
}
void Game::interactCollectablesAndVehicles(std::vector<Game::Collectable> &collectables, std::vector<Game::Vehicle> &vehicles) {
    Game::intersect(collectables, vehicles, [](Game::Collectable &collectable, Game::Vehicle &vehicle) -> void {
        if (collectable.type == Game::CollectableType::Coin) {
            vehicle.money += 5;
        } else if (collectable.type == Game::CollectableType::Health) {
            vehicle.changeHealth(5);
        }

        collectable.health = 0;
    });
}
