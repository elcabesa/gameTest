#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include "SFML/Graphics.hpp"
#include "entt/src/entt/fwd.hpp"

void CD_init(entt::registry& reg);
void calcCollision(entt::registry& reg);
const std::vector<sf::RectangleShape>& getDebugRects();

#endif