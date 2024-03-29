#ifndef GAME_SIMULATION
#define GAME_SIMULATION

#include "Bullet.h"
#include "Collectable.h"
#include "Collider.h"
#include "Missile.h"
#include "Vehicle.h"
#include "Wall.h"
#include <algorithm>
#include <iostream>
#include <vector>

namespace Game {
    void interactBulletsAndWalls(std::vector<Game::Bullet> &bullets, std::vector<Game::Wall> &walls);
    void interactMissilesAndWalls(std::vector<Game::Missile> &missiles, std::vector<Game::Wall> &walls);
    void interactBulletsAndVehicles(std::vector<Game::Bullet> &bullets, std::vector<Game::Vehicle> &vehicles);
    void interactMissilesAndVehicles(std::vector<Game::Missile> &missiles, std::vector<Game::Vehicle> &vehicles);
    void interactCollectablesAndVehicles(std::vector<Game::Collectable> &collectables, std::vector<Game::Vehicle> &vehicles);

    template <class T>
    inline void eraseDead(std::vector<T> &elements) {
        elements.erase(std::remove_if(elements.begin(), elements.end(), [](T el) -> bool {
                           return el.health <= 0;
                       }),
                       elements.end());
    }

    template <class T>
    inline void killOutOfBounds(std::vector<T> &elements, Kore::vec2 topLeft, Kore::vec2 bottomRight) {
        for (T &element : elements) {
            if (!Engine::isPointInRect(element.getColliderPos(), topLeft, bottomRight)) {
                element.health = 0;
            }
        }
    }

    template <class T>
    inline void updateVector(std::vector<T> &elements) {
        for (T &element : elements) {
            element.update();
        }
    }

    // TODO: Use acceleration structure to avoid N^2
    // NOTE: No equality check
    template <class T, class U, typename Functor>
    inline void intersect(std::vector<T> &firstList, std::vector<U> &secondList, Functor onIntersect) {
        for (T &first : firstList) {
            for (U &second : secondList) {
                int minDistanceSquared = std::pow(first.getColliderRadius() + second.getColliderRadius(), 2);

                if ((first.getColliderPos() - second.getColliderPos()).squareLength() < minDistanceSquared) {
                    onIntersect(first, second);
                }
            }
        }
    }

    inline void respawnDeadPlayers(std::vector<Game::Vehicle> &vehicles) {
        Kore::vec2 respawnPoint{100, 100}; // TODO: Don't hardcode, organise world definition
        for (Game::Vehicle &vehicle : vehicles) {
            if (vehicle.health <= 0) {
                vehicle.pos = respawnPoint;
                vehicle.health = vehicle.maxHealth;
            }
        }
    }

    inline void simulate(
        std::vector<Game::Bullet> &bullets,
        std::vector<Game::Missile> &missiles,
        std::vector<Game::Wall> &walls,
        std::vector<Game::Vehicle> &vehicles,
        std::vector<Game::Collectable> &collectables,
        int worldWidth = 5000, // TODO: Use centrally defined world size
        int worldHeight = 2000
    ) {
        updateVector(bullets);
        updateVector(missiles);
        interactBulletsAndWalls(bullets, walls);
        interactMissilesAndWalls(missiles, walls);
        interactMissilesAndVehicles(missiles, vehicles);
        interactBulletsAndVehicles(bullets, vehicles);
        interactCollectablesAndVehicles(collectables, vehicles);
        killOutOfBounds(bullets, Kore::vec2{0, 0}, Kore::vec2{worldWidth, worldHeight});
        killOutOfBounds(missiles, Kore::vec2{0, 0}, Kore::vec2{worldWidth, worldHeight});
        eraseDead(bullets);
        eraseDead(missiles);
        eraseDead(walls);
        eraseDead(collectables);
        respawnDeadPlayers(vehicles);
    }
} // namespace Game

#endif