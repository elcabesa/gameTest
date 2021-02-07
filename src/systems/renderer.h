#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SFML/Graphics.hpp"
#include "entt/src/entt/fwd.hpp"

void draw(sf::RenderWindow & w, entt::registry& reg);

void drawQuadTreeDebugInfo(sf::RenderWindow & w, const std::vector<sf::RectangleShape>& quadTreeRects);

#endif