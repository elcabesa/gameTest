#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <entt/src/entt/fwd.hpp>

void updatePosition(entt::registry &reg);
void worldBorderCollision(entt::registry &reg);
void updateCollisionBoxes(entt::registry &reg);

#endif