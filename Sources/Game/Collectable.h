#ifndef GAME_COLLECTABLE
#define GAME_COLLECTABLE

#include "../Engine/Core.h"
#include "../Engine/Graphics.h"
#include "Collider.h"
#include "Net/EntityGenerated.h"
#include <Kore/Math/Vector.h>
#include <cmath>

namespace Game {
    enum class CollectableType {
        Coin
    };

    class Collectable : public CircleCollider {
    private:
        static const int radius = 40;

    public:
        // TODO: Clean up this conversion
        CollectableType getTypeFromNetType(Net::CollectableType netType) {
            if (netType == Net::CollectableType::Coin) {
                return CollectableType::Coin;
            }
            return CollectableType::Coin;
        }

        Collectable(Kore::vec2 pos, CollectableType type)
            : pos(pos), type(type){};

        Collectable(const Net::CollectableData *data)
            : pos(Kore::vec2(data->pos().x(), data->pos().y())),
              type(getTypeFromNetType(data->type())){};

        Kore::vec2 getColliderPos() { return pos; };
        float getColliderRadius() { return radius; };
        int health = 1;

        const Net::CollectableData getData() {
            // TODO: Clean up this conversion
            Net::CollectableType netType;
            if (type == CollectableType::Coin) {
                netType = Net::CollectableType::Coin;
            }

            return Game::Net::CollectableData{
                Game::Net::Vec2{
                    pos.x(),
                    pos.y()},
                netType};
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
