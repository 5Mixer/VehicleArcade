#ifndef GAME_SIMULATION
#define GAME_SIMULATION

#include "Bullet.h"
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

    template <class T>
    inline void eraseDead(std::vector<T> &elements) {
        elements.erase(std::remove_if(elements.begin(), elements.end(), [](T el) -> bool {
                           return el.health <= 0;
                       }),
                       elements.end());
    }

    template <class T>
    inline void updateVector(std::vector<T> &elements) {
        for (T &element : elements) {
            element.update();
        }
    }

    // TODO: Use acceleration structure to avoid N^2
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
        std::vector<Game::Vehicle> &vehicles
    ) {
        updateVector(bullets);
        updateVector(missiles);
        interactBulletsAndWalls(bullets, walls);
        interactMissilesAndWalls(missiles, walls);
        interactMissilesAndVehicles(missiles, vehicles);
        interactBulletsAndVehicles(bullets, vehicles);
        eraseDead(bullets);
        eraseDead(missiles);
        eraseDead(walls);
        respawnDeadPlayers(vehicles);
    }
} // namespace Game

#endif