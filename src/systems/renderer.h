#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SFML/Graphics.hpp"
#include "entt/src/entt/fwd.hpp"

void draw(sf::RenderTarget & w, entt::registry& reg);

void drawQuadTreeDebugInfo(sf::RenderTarget & w, const std::vector<sf::RectangleShape>& quadTreeRects);

#endif