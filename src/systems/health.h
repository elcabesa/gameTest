#ifndef HEALTH_H
#define HEALTH_H

#include <SFML/System/Time.hpp>
#include <entt/src/entt/fwd.hpp>

void updateHealth(entt::registry& reg, sf::Time elapsed);

#endif