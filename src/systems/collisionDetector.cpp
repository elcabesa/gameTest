#include <iostream>
#include "collisionDetector.h"
#include "parameters.h"
#include "components/position.h"
#include "components/velocity.h"
#include "components/illness.h"
#include "entt/src/entt/entity/registry.hpp"
//#include "quadtree/include/Quadtree.h"
#include "quadtree.h"

std::vector<sf::RectangleShape> quadTreeRects; // TODO remove global variable
auto quadtree = quadTree(Rect(0.0f, 0.0f, float(dimY+2.001), float(dimX+2.001))); // TODO remove global variable
 
const std::vector<sf::RectangleShape>& getDebugRects() {
    return quadTreeRects;
}

void CD_init(entt::registry& reg) {
    Node node;
    node.rect.height() = 2;
    node.rect.width() = 2;

    auto view = reg.view<position>();

    for (auto& en: view) {
        auto& pos = view.get<position>(en); 
        node.id = en;
        node.rect.top() = pos.y;
        node.rect.left() = pos.x;
        quadtree.add(node);   
    }
}

void CD_update(entt::registry& reg) {
    auto view = reg.view<position>();
    
    for (auto& en: view) {
        auto& pos = view.get<position>(en);
        //std::cout<<"check en "<<(int)en<<" pointer "<<pointer.node<<std::endl;
        quadtree.updatePosition(en, Rect(pos.y, pos.x, 2, 2));
    }
}

void CD_calcRects() {
    quadTreeRects.clear();
    for (auto&b : quadtree.getRects()) {
        sf::RectangleShape r(sf::Vector2(b.width(), b.height()));
        r.setPosition(b.left(), b.top());
        r.setFillColor(sf::Color::Transparent);
        r.setOutlineColor(sf::Color::Yellow);
        r.setOutlineThickness(1);
        quadTreeRects.push_back(r);
    }
}

void CD_updateVelocity(entt::registry& reg) {
    auto view = reg.view<velocity>();

    auto intersections = quadtree.findAllIntersections();
    //std::cout<<"intersection size:"<<intersections.size()<<std::endl;
    for(auto& inter: intersections) {
        auto & v1 = view.get<velocity>(inter.first);
        auto & v2 = view.get<velocity>(inter.second);
        //std::cout<<"COLLISION "<< (int)inter.first<<" "<< (int)inter.second<<std::endl;

        v1.dx = (std::rand() % 50)/100.0 - 0.245;
        v1.dy = (std::rand() % 50)/100.0 - 0.245;
        v2.dx = (std::rand() % 50)/100.0 - 0.245;
        v2.dy = (std::rand() % 50)/100.0 - 0.245;

        if (reg.has<ill>(inter.first) && reg.has<healty>(inter.second)) {
            if((std::rand() % 100) < illProbability) {
                reg.remove<healty>(inter.second);
                reg.emplace<ill>(inter.second);
            }
        }
        if (reg.has<ill>(inter.second) && reg.has<healty>(inter.first)) {
            if((std::rand() % 100) < illProbability) {
                reg.remove<healty>(inter.first);
                reg.emplace<ill>(inter.first);
            }
        }
    }
}

void calcCollision(entt::registry& reg) {
    //std::cout<<"update position"<<std::endl;
    CD_update(reg); // TODO move in in the movement system??

    //CD_calcRects();
    
    CD_updateVelocity(reg);
}