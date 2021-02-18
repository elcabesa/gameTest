#include "health.h"
#include "parameters.h"
#include "components/illness.h"
#include "entt/src/entt/entity/registry.hpp"

void updateHealth(entt::registry& reg, sf::Time elapsed) {
    auto view = reg.view<ill>();
    for (const entt::entity e : view) {
        auto & p = view.get<ill>(e);
        p.tempo -= elapsed.asSeconds();
        if (p.tempo < 0) {
            if ((std::rand() % 100) >= deathProbability) { 
                reg.remove<ill>(e);
                reg.emplace<recovered>(e);
            } else {
                reg.destroy(e);
                //TODO remove from quadtree!!
            }
        }
    }
}
