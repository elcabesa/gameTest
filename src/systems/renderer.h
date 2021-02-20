#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "entt/src/entt/fwd.hpp"

void draw(sf::RenderTarget & w, entt::registry& reg, sf::Sprite& sp, int zoomLevel);
void drawQuadTreeDebugInfo(sf::RenderTarget & w, const std::vector<sf::RectangleShape>& quadTreeRects);

#endif