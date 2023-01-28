#ifndef GAME_COLLECTABLE
#define GAME_COLLECTABLE

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include "Collider.h"
#include "Net/EntityGenerated.h"
#include <Kore/Math/Vector.h>
#include <cmath>

namespace Game {
    using CollectableType = Net::CollectableType;

    class Collectable : public CircleCollider {
    private:
        static const int radius = 40;

    public:
        Collectable(Kore::vec2 pos, CollectableType type)
            : pos(pos), type(type){};

        Collectable(const Net::CollectableData *data)
            : pos(Kore::vec2(data->pos().x(), data->pos().y())),
              type(data->type()){};

        Kore::vec2 getColliderPos() { return pos; };
        float getColliderRadius() { return radius; };
        int health = 1;

        const Net::CollectableData getData() {
            return Game::Net::CollectableData{
                Game::Net::Vec2{
                    pos.x(),
                    pos.y()},
                type};
        }

        Kore::vec2 pos;
        CollectableType type;

        void render(Engine::Graphics &g) {
            switch (type) {
                case CollectableType::Coin:
                    g.drawCoin(pos);
                    break;

                default:
                    break;
            }
        }
    };
} // namespace Game

#endif
