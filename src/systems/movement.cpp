#include "movement.h"
#include "parameters.h"
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

        if(p.x <= objectSize) { p.x = objectSize; v.dx *= -1;}
        if(p.x >= worldX - objectSize - epsilon) { p.x = worldX - objectSize - epsilon; v.dx *= -1;}
            
        if(p.y <= objectSize) { p.y = objectSize; v.dy *= -1;}
        if(p.y >= worldY - objectSize - epsilon) { p.y = worldY - objectSize - epsilon; v.dy *= -1;}
    }

}