#include <iostream>
#include "renderer.h"
#include "components/position.h"
#include "components/illness.h"
#include "entt/src/entt/entity/registry.hpp"

void draw(sf::RenderTarget & w, entt::registry& reg) {
    sf::VertexArray va;

    for (auto view = reg.view<position, healty>(); const entt::entity e : view) {
        auto & p =view.get<position>(e);
        va.append(sf::Vertex(sf::Vector2f(p.x, p.y), sf::Color(255u,255u,255u)));   
    }
    for (auto view = reg.view<position, ill>(); const entt::entity e : view) {
        auto & p =view.get<position>(e);
        va.append(sf::Vertex(sf::Vector2f(p.x, p.y), sf::Color(255u,0,0)));   
    }
    for (auto view = reg.view<position, recovered>(); const entt::entity e : view) {
        auto & p =view.get<position>(e);
        va.append(sf::Vertex(sf::Vector2f(p.x, p.y), sf::Color(0,0,255)));   
    }
    w.draw(va);
}

void drawQuadTreeDebugInfo(sf::RenderTarget & w, const std::vector<sf::RectangleShape>& quadTreeRects) {
   // unsigned int i = 0;
    for(auto &r:quadTreeRects) {
        //std::cout<<r.getPosition().x<<" "<<r.getPosition().y<<" "<<r.getSize().x<<" "<<r.getSize().y<<std::endl;
        //if(++i>10) {exit(0);}
        w.draw(r);
    }
}