#include "movement.h"
#include "parameters.h"
#include "components/collisionBox.h"
#include "components/position.h"
#include "components/velocity.h"
#include <entt/src/entt/entity/registry.hpp>


void updatePosition(entt::registry &reg) {
    auto view = reg.view<position, velocity>();
    for (const entt::entity e : view) {
        auto & p = view.get<position>(e); 
        auto & v = view.get<velocity>(e); 
        p.x += v.dx;
        p.y += v.dy;
    }
}

void worldBorderCollision(entt::registry &reg) {
    auto view = reg.view<position, velocity>();
    for (const entt::entity e : view) {
        auto & p =view.get<position>(e);
        auto & v = view.get<velocity>(e); 

        if(p.x <= 0) { p.x = 0; v.dx *= -1;}
        if(p.x >= dimX) { p.x = dimX; v.dx *= -1;}
            
        if(p.y <= 0) { p.y = 0; v.dy *= -1;}
        if(p.y >= dimY) { p.y = dimY; v.dy *= -1;}
    }
}

void updateCollisionBoxes(entt::registry &reg) {
    auto view = reg.view<position, collisionBox>();
    for (const entt::entity e : view) {
        auto & p = view.get<position>(e); 
        auto & c = view.get<collisionBox>(e); 
        c.left = p.x;
        c.top = p.y;
    }
}