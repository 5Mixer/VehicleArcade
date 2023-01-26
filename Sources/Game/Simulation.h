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
    void interactMissilesAndVehicles(std::vector<Game::Missile> &missiles, std::vector<Game::Vehicle> &vehicles);

    template <class T>
    void eraseDead(std::vector<T> &elements) {
        elements.erase(std::remove_if(elements.begin(), elements.end(), [](T el) -> bool {
                           return el.health <= 0;
                       }),
                       elements.end());
    }

    // TODO: Use acceleration structure to avoid N^2
    template <class T, class U, typename Functor>
    void intersect(std::vector<T> &firstList, std::vector<U> &secondList, Functor onIntersect) {
        for (T &first : firstList) {
            for (U &second : secondList) {
                int minDistanceSquared = std::pow(first.getColliderRadius() + second.getColliderRadius(), 2);

                if ((first.getColliderPos() - second.getColliderPos()).squareLength() < minDistanceSquared) {
                    onIntersect(first, second);
                }
            }
        }
    }
} // namespace Game

#endif